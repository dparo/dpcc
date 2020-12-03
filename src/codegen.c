#include "codegen.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "lexer.h"
#include "types.h"
#include "globals.h"
#include "utils.h"
#include "log.h"
#include "dpcc.h"

#include "__gen.h"
#include "parser.h"

extern int yynerrs;
static str_t S_str = { 0 };

#define LOG(SEVERITY, N, ...) \
    dpcc_log((SEVERITY), &((N)->tok->loc), __VA_ARGS__)

#define ERR(N, ...)                                 \
    do {                                            \
        LOG(DPCC_SEVERITY_ERROR, (N), __VA_ARGS__); \
        yynerrs += 1;                               \
    } while (0)

#define INFO(N, ...)                               \
    do {                                           \
        LOG(DPCC_SEVERITY_INFO, (N), __VA_ARGS__); \
    } while (0)

#define WARN(N, ...)                                  \
    do {                                              \
        LOG(DPCC_SEVERITY_WARNING, (N), __VA_ARGS__); \
    } while (0)

#define EMIT(...) \
    sfcat(&G_allctx, &S_str, __VA_ARGS__)

static inline void typemismatch_check(ast_node_t *expected_type, ast_node_t *got_type)
{

    if ((expected_type != NULL && got_type != NULL) && (expected_type->md.type != got_type->md.type)) {
        ERR(expected_type, "Type Mismatch");
        INFO(expected_type, "Expected `%s`", dpcc_type_as_str(expected_type->md.type));
        INFO(got_type, "But got `%s`", dpcc_type_as_str(got_type->md.type));
    }
}

static void check_initializer_list(ast_node_t *n, enum DPCC_TYPE expected_type, int32_t array_type_len, int32_t init_list_len)
{
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;

    if ((c0->childs[1] && init_list_len != array_type_len)) {
        ERR(c2, "Number of elements in initializer list do not match");
        INFO(c0->childs[1], "Expected number of elements: `%d`", array_type_len);
        INFO(c2, "Number of elements got: `%d`", init_list_len);
    } else if ((c2 && init_list_len <= 0)) {
        ERR(c2, "Number of elements in initializer list is invalid");
        INFO(c2, "Cannot create zero or negative sized array");
    }

    for (int32_t i = 0; i < c2->num_childs; i++) {

        if ((c2->childs[i] && c2->childs[i]->md.type != expected_type)) {
            ERR(c2->childs[i], "Type mismatch in array initializer list");
            INFO(c0->childs[0], "Expected `%s`", dpcc_type_as_str(c0->childs[0]->md.type));
            INFO(c2->childs[i], "Got `%s`", dpcc_type_as_str(c2->childs[i]->md.type));
        }
    }
}

static void typecheck_array(ast_node_t *n)
{
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;

    (void)c0, (void)c1, (void)c2;

    // Deduce type of array variable declarations
    assert((c0->childs[1] == NULL) || (c0->childs[1]->kind == TOK_I32_LIT && c0->childs[1]->md.type == TYPE_I32));
    int32_t array_type_len = c0->childs[1] ? c0->childs[1]->val.as_i32 : 0;
    int32_t init_list_len = c2 != NULL ? c2->num_childs : 0;

    n->md.array_len = array_type_len;
    if (c0->childs[1] && array_type_len <= 0) {
        ERR(c0->childs[1], "The number of elements in an array must be a positive integer");
        INFO(c0->childs[1], "Got `%d`", array_type_len);
    } else if (c2) {
        enum DPCC_TYPE expected_type = c0->childs[0]->md.type;

        switch (expected_type) {
        default: {
            invalid_code_path();
        } break;
        case TYPE_I32: {
            c0->md.type = TYPE_I32_ARRAY;
        } break;
        case TYPE_F32: {
            c0->md.type = TYPE_F32_ARRAY;
        } break;
        }

        check_initializer_list(n, expected_type, array_type_len, init_list_len);
        n->md.array_len = init_list_len;
    } else if ((c0 == NULL || c0->childs[1] == NULL || c0->childs[1]->kind != TOK_I32_LIT || c0->childs[1]->md.type != TYPE_I32)) {
        ERR(c0, "Size of the array must be specified");
        INFO(c0, "Either specify the size inside the square brackets, or provide an initializer list");
    }
}

static void typecheck_casting_operator(ast_node_t *n)
{
    switch (n->kind) {
    default: {
        invalid_code_path();
    } break;
    case TOK_KW_INT: {
        n->md.type = TYPE_I32;
    } break;
    case TOK_KW_FLOAT: {
        n->md.type = TYPE_F32;
    } break;
    case TOK_KW_BOOL: {
        n->md.type = TYPE_BOOL;
    } break;
    }
}

static void typecheck_array_subscript(ast_node_t *n)
{
    int32_t subscript_idx = n->childs[1]->val.as_i32;
    int32_t array_len = n->childs[0]->decl->md.array_len;

    if (n->childs[0]->md.type != TYPE_I32_ARRAY && n->childs[0]->md.type != TYPE_F32_ARRAY) {
        ERR(n->childs[0], "Identifier is not an array");
        INFO(n->childs[0]->decl, "Previous declaration was here (type: %s)", dpcc_type_as_str(n->childs[0]->md.type));
    } else if (subscript_idx < 0 || (subscript_idx >= array_len)) {
        WARN(n->childs[1], "Invalid subscript constant");
        INFO(n->childs[0]->decl, "As specified from declaration index should be in [%d, %d)", 0, array_len);
        INFO(n->childs[1], "Got `%d` instead", subscript_idx);
    }
}

static void typecheck_vardecl(ast_node_t *n)
{
    assert(((n->kind == TOK_KW_FN) || (n->kind == TOK_KW_LET)));

    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;

    (void)c0, (void)c1, (void)c2;

    ast_node_t *type = c0 ? c0 : NULL;
    ast_node_t *rhs = c2 ? c2 : NULL;
    int32_t init_val_len = 1;

    bool var_decl_with_user_listed_type = c0;
    bool var_decl_no_user_listed_type = !c0;
    bool integral_var_decl = (!c0 || c0->kind != TOK_OPEN_BRACKET);
    bool var_decl_with_rhs = c2;
    bool array_var_decl = c0 && c0->kind == TOK_OPEN_BRACKET;

    if (array_var_decl) {

        if (var_decl_with_user_listed_type) {

            switch (c0->childs[0]->kind) {
            default: {
                invalid_code_path();
            } break;
            case TOK_KW_INT: {
                n->md.type = TYPE_I32_ARRAY;
            } break;
            case TOK_KW_FLOAT: {
                n->md.type = TYPE_F32_ARRAY;
            } break;
            }

            typecheck_array(n);
        } else if (var_decl_no_user_listed_type) {
            ERR(n, "Expected valid initializer list for array var decls");
        } else {
            invalid_code_path();
        }

    } else if (integral_var_decl) {

        if (var_decl_no_user_listed_type && !var_decl_with_rhs) {
            n->md.type = TYPE_I32;
        } else if (var_decl_with_user_listed_type) {

            switch (c0->kind) {
            default: {
                invalid_code_path();
            } break;
            case TOK_KW_INT: {
                n->md.type = TYPE_I32;
            } break;
            case TOK_KW_FLOAT: {
                n->md.type = TYPE_F32;
            } break;
            case TOK_KW_BOOL: {
                n->md.type = TYPE_BOOL;
            } break;
            }

        } else if (var_decl_with_user_listed_type && var_decl_with_rhs) {
            assert(rhs->md.type != TYPE_NONE);
            typemismatch_check(type, rhs);
            n->md.type = rhs->md.type;
        } else if (var_decl_no_user_listed_type && var_decl_with_rhs) {
            n->md.type = rhs->md.type;
        } else {
            invalid_code_path();
        }

        n->md.array_len = 1;

    } else {
        invalid_code_path();
    }
}

static void typecheck_print(ast_node_t *n)
{
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    assert(c0 && c0->md.type);

    if (!((c0->md.type == TYPE_I32) || (c0->md.type == TYPE_F32) || (c0->md.type == TYPE_BOOL))) {
        ERR(c0, "The provided element type (%s) is not printable", c0->tok->lexeme);
        INFO(c0, "Valid printable types are: %s %s %s ", dpcc_type_as_str(TYPE_I32), dpcc_type_as_str(TYPE_F32), dpcc_type_as_str(TYPE_BOOL));
    }
    n->md.type = c0->md.type;
}

static void typecheck(ast_node_t *n)
{
    bool is_casting_operator = ((n->kind == TOK_KW_INT) || (n->kind == TOK_KW_FLOAT) || (n->kind == TOK_KW_BOOL)) && !((n->parent->kind == TOK_KW_FN) || (n->parent->kind == TOK_KW_LET));
    bool var_decl = (((n->kind == TOK_KW_FN) || (n->kind == TOK_KW_LET)));
    bool is_typable_identifier = n->kind == TOK_ID && n->parent && !((n->parent->kind == TOK_KW_FN) || (n->parent->kind == TOK_KW_LET));
    bool is_array_subscript = n->kind == TOK_AR_SUBSCR && n->childs[1]->kind == TOK_I32_LIT && n->childs[1]->md.type == TYPE_I32;
    bool is_print = n->kind == TOK_KW_PRINT;

    if (n->kind == TOK_CHAR_LIT) {
        n->md.type = TYPE_I32;
    } else if (n->kind == TOK_I32_LIT) {
        n->md.type = TYPE_I32;
    } else if (n->kind == TOK_F32_LIT) {
        n->md.type = TYPE_F32;
    } else if (n->kind == TOK_BOOL_LIT) {
        n->md.type = TYPE_BOOL;
    } else if (n->kind == TOK_ID) {
        if (n->decl) {
            n->md.type = n->decl->md.type;
        }
    } else if (n->md.type == TYPE_NONE) {
        typecheck_expr_and_operators(n);

        if (is_casting_operator) {
            typecheck_casting_operator(n);
        } else if (var_decl) {
            typecheck_vardecl(n);
        } else if (is_typable_identifier) {
            assert(n->decl != NULL);
            assert(n->decl->md.type != TYPE_NONE);
            n->md.type = n->decl->md.type;
        } else if (is_array_subscript) {
            typecheck_array_subscript(n);
        } else if (is_print) {
            typecheck_print(n);
        }
    }
}

static char *gen_sym(ast_node_t *n)
{
    assert(n->md.type != TYPE_NONE);

    str_t s = { 0 };

    if (n->md.sym) {
        sfcat(&G_allctx, &s, "%s", n->md.sym);
    } else if (n->kind == TOK_ID && !n->md.sym) {
        assert(n->decl);
        sfcat(&G_allctx, &s, "_var_get(\"%s\", %s, 0)", n->tok->lexeme, get_type_label(n->md.type));
    } else if (n->kind == TOK_I32_LIT || n->kind == TOK_F32_LIT || n->kind == TOK_CHAR_LIT || n->kind == TOK_BOOL_LIT) {
        if (n->md.type == TYPE_I32) {
            sfcat(&G_allctx, &s, "%d", n->val.as_i32);
        } else if (n->md.type == TYPE_F32) {
            sfcat(&G_allctx, &s, "%f", n->val.as_f32);
        } else if (n->md.type == TYPE_BOOL) {
            sfcat(&G_allctx, &s, "%i", n->val.as_bool);
        } else {
            invalid_code_path();
        }
    }
    return s.cstr;
}

static void setup_addrs_and_jmp_tables(ast_node_t *n)
{

    if (!n->md.sym && (n->md.type != TYPE_NONE && is_expr_node(n))) {

        if (n->kind == TOK_ID) {
            assert(n->decl->md.type == n->md.type);
        }
        assert(n->md.type != TYPE_I32_ARRAY);
        assert(n->md.type != TYPE_F32_ARRAY);
        n->md.sym = new_tmp_var(n->md.type);
    } else if (n->kind == TOK_KW_WHILE && n->md.jmp_bot == NULL) {
        n->md.jmp_bot = new_tmp_label();
    } else if (n->kind == TOK_KW_DO && n->md.jmp_top == NULL) {
        n->md.jmp_top = new_tmp_label();
    } else if (n->kind == TOK_KW_FOR && n->md.jmp_top == NULL) {
        n->md.jmp_top = new_tmp_label();
    } else if ((n->kind == TOK_KW_IF) && (n->md.jmp_next == NULL || n->md.jmp_bot == NULL)) {
        n->md.jmp_next = new_tmp_label();
        n->md.jmp_bot = new_tmp_label();
    } else if (n->md.type != TYPE_NONE) {
        n->md.sym = gen_sym(n);
    }
}

static void first_ast_pass(void)
{
    ast_traversal_t att = { 0 };
    ast_traversal_begin(&att, &G_root_node, false, true);
    ast_node_t *n = NULL;

    while ((n = ast_traverse_next(&att, NULL)) != NULL) {
        typecheck(n);
        setup_addrs_and_jmp_tables(n);
    }
}

static void emit_indexing(ast_node_t *n)
{
    assert(n->kind == TOK_AR_SUBSCR);
    assert(n->md.type != TYPE_NONE);
    assert(n->md.sym);
    assert(n->num_childs == 2);
    assert(n->childs[0] && n->childs[1]);

    ast_node_t *lhs = n->childs[0];
    ast_node_t *rhs = n->childs[1];

    assert(lhs->kind == TOK_ID || lhs->kind == TOK_AR_SUBSCR);
    assert(lhs->md.type != TYPE_NONE);
    assert(rhs->md.type != TYPE_NONE);

    assert(lhs->decl);
    assert(unref_type(lhs->md.type) == n->md.type);

    EMIT("%s = _var_get(\"%s\", %s, %s);\n",
        n->md.sym,
        lhs->tok->lexeme,
        get_type_label(lhs->md.type),
        rhs->md.sym);
}

static void emit_array_subscript(ast_node_t *n)
{
    assert(n->kind == TOK_AR_SUBSCR);
    assert(n->num_childs == 2);
    assert(n->childs[0]);
    assert(n->childs[0]->md.type);
    assert(n->childs[0]->md.sym);

    ast_node_t *lhs = n->childs[0];
    ast_node_t *rhs = n->childs[1];

    assert(lhs->md.type != TYPE_NONE);
    assert(rhs->md.type != TYPE_NONE);

    char *index = rhs->md.sym;

    EMIT("%s = _var_get(\"%s\", %s, %s);\n",
        n->md.sym,
        lhs->tok->lexeme,
        get_type_label(n->md.type),
        index);
}

static void emit_assign(ast_node_t *n)
{
    assert(n->kind == TOK_ASSIGN);
    assert(n->num_childs == 2);
    assert(n->childs[0] && n->childs[1]);
    assert(n->md.type);
    assert(n->md.sym);

    ast_node_t *lhs = n->childs[0];
    ast_node_t *rhs = n->childs[1];

    assert(lhs->md.type != TYPE_NONE);
    assert(rhs->md.type != TYPE_NONE);

    if (lhs->kind == TOK_AR_SUBSCR) {
        assert(lhs->num_childs == 2);
        assert(lhs->childs[0]);
        assert(lhs->childs[0]->md.type);
        assert(lhs->childs[0]->md.sym);
        char *index = lhs->childs[1]->md.sym;
        EMIT("%s = _var_set(\"%s\", %s, %s, %s);\n",
            lhs->md.sym,
            lhs->childs[0]->tok->lexeme,
            get_type_label(lhs->md.type),
            index,
            rhs->md.sym);

    } else {
        assert(lhs->decl);
        assert(lhs->md.type);
        assert(lhs->md.sym);

        EMIT("%s = _var_set(\"%s\", %s, 1, %s);\n",
            n->md.sym,
            lhs->tok->lexeme,
            get_type_label(lhs->md.type),
            rhs->md.sym);
    }
}

static void emit_pre_inc_dec(ast_node_t *n)
{
    assert(n->kind == TOK_INC || n->kind == TOK_DEC);
    assert(n->num_childs == 1);
    assert(n->childs[0]);
    assert(n->md.type);
    assert(n->md.sym);

    ast_node_t *lhs = n->childs[0];

    assert(lhs->md.type != TYPE_NONE);
    assert(lhs->decl);

    EMIT("%s = %s;\n", n->md.sym, n->childs[0]->md.sym);
    if (n->kind == TOK_INC) {
        EMIT("_vspcIncDec = %s + 1;\n", lhs->md.sym);
    } else {
        EMIT("_vspcIncDec = %s - 1;\n", lhs->md.sym);
    }
    EMIT("_var_set(\"%s\", %s, 1, _vspcIncDec);\n",
        lhs->tok->lexeme,
        get_type_label(lhs->md.type));
}

static void emit_expr(ast_node_t *n)
{
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;
    ast_node_t *c3 = (n->num_childs >= 4) ? n->childs[3] : NULL;

    (void)c0, (void)c1, (void)c2, (void)c3;

    assert(n->md.sym);

    if (n->num_childs == 0) {
        // base case just stop
        return;
    } else if (n->kind == TOK_OPEN_BRACKET) {
        emit_indexing(n);
    } else if (n->kind == TOK_ASSIGN) {
        emit_assign(n);
    } else if (n->kind == TOK_INC || n->kind == TOK_DEC) {
        emit_pre_inc_dec(n);
    } else if (n->kind == TOK_AR_SUBSCR) {
        emit_array_subscript(n);
    } else if (is_expr_node(n)) {
        if (n->num_childs == 1) {
            if (is_prefix_op(n)) {
                EMIT("%s = %s %s;\n",
                    n->md.sym,
                    n->tok->lexeme,
                    n->childs[0]->md.sym);
            } else if (is_postfix_op(n)) {

                EMIT("%s = %s %s;\n",
                    n->md.sym,
                    n->childs[0]->md.sym,
                    n->tok->lexeme);
            } else {
                invalid_code_path();
            }
        } else if (is_postfix_op(n)) {
            // NOTE We don't have postfix operators yet
            EMIT("%s = %s %s;",
                n->md.sym,
                n->childs[0]->md.sym,
                n->tok->lexeme);
        } else {
            assert(n->num_childs == 2);
            if (n->kind == TOK_POW) {
                EMIT("%s = pow%s(%s, %s);\n",
                    n->md.sym,
                    n->md.type == TYPE_F32 ? "" : "l",
                    n->childs[0]->md.sym,
                    n->childs[1]->md.sym);
            } else {
                EMIT("%s = %s %s %s;\n",
                    n->md.sym,
                    n->childs[0]->md.sym,
                    n->tok->lexeme,
                    n->childs[1]->md.sym);
            }
        }

    } else {
        invalid_code_path();
    }
}

static void emit_var_decl(ast_node_t *n)
{
    assert(n->kind == TOK_KW_LET);
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;

    assert(c1);
    (void)c0, (void)c1, (void)c2;

    EMIT("_var_decl(\"%s\", %s, %d);\n", n->childs[1]->tok->lexeme, get_type_label(n->md.type), n->md.array_len);
}

static void emit_var_init(ast_node_t *n)
{
    assert(n->kind == TOK_KW_LET);
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;

    assert(c1);
    (void)c0, (void)c1, (void)c2;

    bool is_array = n->md.type == TYPE_I32_ARRAY || n->md.type == TYPE_F32_ARRAY;
    bool has_rhs = c2 != NULL;

    if (has_rhs) {
        char *ctype = "int32_t";
        if (n->md.type == TYPE_F32_ARRAY) {
            ctype = "float";
        }

        EMIT("_var_init(\"%s\", %s, %d, (%s[]) {", c1->tok->lexeme, get_type_label(n->md.type), n->md.array_len, ctype);
        if (is_array) {
            assert(c2->kind == TOK_OPEN_BRACE);
            assert(c2->num_childs >= 1);
            for (int32_t i = 0; i < n->md.array_len; i++) {
                EMIT("%s", c2->childs[i]->md.sym);
                if (i != n->md.array_len - 1) {
                    EMIT(", ");
                }
            }
        } else {
            EMIT("%s", c2->md.sym);
        }

        EMIT("};\n\n");
    }
}

static void emit_print(ast_node_t *n)
{
    assert(n->kind == TOK_KW_PRINT);
    assert(n->num_childs == 1);
    assert(n->childs[0]);

    assert(!n->md.sym);
    assert(n->md.type);

    ast_node_t *c0 = n->childs[0];

    assert(c0->md.type);
    assert(c0->md.sym);

    assert(n->md.type == c0->md.type);

    switch (n->md.type) {
    default: {
        invalid_code_path();
    } break;
    case TYPE_BOOL:
    case TYPE_I32: {
        EMIT("printf(\"%%d\", %s);\n\n", c0->md.sym);
    } break;
    case TYPE_F32: {
        EMIT("printf(\"%%f\", %s);\n\n", c0->md.sym);
    } break;
    }
}

static void emit_if(ast_node_t *n, bool is_top_down_encounter)
{
}

static void emit_for(ast_node_t *n, bool is_top_down_encounter)
{
}

static void emit_while(ast_node_t *n, bool is_top_down_encounter)
{
}

static void emit_do(ast_node_t *n, bool is_top_down_encounter)
{
}

static void emit(ast_node_t *n, bool is_top_down_encounter)
{
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;

    bool is_var_decl = c0 && c0->kind == TOK_KW_LET;
    bool is_print = c0 && c0->kind == TOK_KW_PRINT;

    bool is_if = c0 && c0->kind == TOK_KW_IF;
    bool is_for = c0 && c0->kind == TOK_KW_IF;
    bool is_while = c0 && c0->kind == TOK_KW_IF;
    bool is_do = c0 && c0->kind == TOK_KW_IF;

    if (is_top_down_encounter) {
        // TOP DOWN ENCOUNTERS
        if (is_var_decl && c0->parent->kind == TOK_SEMICOLON) {
            emit_var_decl(c0);
        }
    } else {
        // BOTTOM UP ENCOUNTERS
        if (is_var_decl && c0->parent->kind == TOK_SEMICOLON) {
            emit_var_init(c0);
        } else if (is_print) {
            emit_print(c0);
        } else if (n->md.sym) {
            emit_expr(n);
        }
    }

    // Top, down and bottom up encounters
    if (is_if) {
        emit_if(c0, is_top_down_encounter);
    } else if (is_for) {
        emit_for(c0, is_top_down_encounter);
    } else if (is_while) {
        emit_while(c0, is_top_down_encounter);
    } else if (is_do) {
        emit_do(c0, is_top_down_encounter);
    }
}

static void second_ast_pass(void)
{
    memset(&S_str, 0, sizeof(S_str));
    sfcat(&G_allctx, &S_str, "\n");

    ast_traversal_t att = { 0 };
    ast_traversal_begin(&att, &G_root_node, true, true);
    ast_node_t *n = NULL;
    bool is_top_down_encounter = false;

    EMIT("// Special variable used to implemenent INC (x++) and dec (x--)\n");
    EMIT("// It is used to temporary hold the result of the INC/DEC in order to perform the side effect\n");
    EMIT("int32_t _vspcIncDec;\n");
    EMIT("\n");

    EMIT("// 3AC Var decls\n");

    for (int32_t i = 0; i < G_codegen_i32_cnt; i++) {
        EMIT("int32_t _vi%d = 0;\n", i);
    }

    for (int32_t i = 0; i < G_codegen_f32_cnt; i++) {
        EMIT("float   _vf%d = 0.0;\n", i);
    }

    for (int32_t i = 0; i < G_codegen_bool_cnt; i++) {
        EMIT("bool    _vb%d = false;\n", i);
    }

    EMIT("\n");

    while ((n = ast_traverse_next(&att, &is_top_down_encounter)) != NULL) {
        ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
        ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
        ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;
        ast_node_t *c3 = (n->num_childs >= 4) ? n->childs[3] : NULL;

        (void)c0, (void)c1, (void)c2, (void)c3;

        if (n->kind == TOK_OPEN_BRACE && (!n->parent || n->parent->kind != TOK_KW_LET)) {

            if (is_top_down_encounter) {
                EMIT("\n_scope_begin();\n");
            } else {
                EMIT("_scope_end();\n");
            }

        } else {
            emit(n, is_top_down_encounter);
        }
    }
}

char *codegen(void)
{
    first_ast_pass();

    if (yynerrs == 0) {
        second_ast_pass();
        if (yynerrs == 0) {
            return S_str.cstr;
        } else {
            return NULL;
        }
    }

    return NULL;
}
