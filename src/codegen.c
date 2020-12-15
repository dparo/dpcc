#include "codegen.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/cdefs.h>

#include "lexer.h"
#include "types.h"
#include "globals.h"
#include "utils.h"
#include "log.h"
#include "dpcc.h"

#include "__gen.h"

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

static int32_t S_indentation = 0;

static void _emit(bool indent, char *fmt, ...)
    __attribute__((format(printf, 2, 3)));

static void _emit(bool indent, char *fmt, ...)
{
    if (indent) {
        for (int32_t i = 0; i < S_indentation; i++) {
            sfcat(&G_allctx, &S_str, "    ");
        }
    }

    va_list ap;
    va_start(ap, fmt);
    sfcatv(&G_allctx, &S_str, fmt, ap);
    va_end(ap);
}

#define EMIT(...) _emit(true, __VA_ARGS__)
#define EMITN(...) _emit(false, __VA_ARGS__)

char *new_tmp_var(ast_node_t *n)
{
    str_t s = { 0 };

    switch (n->md.type) {
    default: {
        invalid_code_path();
    } break;
    case TYPE_I32: {
        sfcat(&G_allctx, &s, "_vi%d", G_codegen_i32_cnt++);
    } break;
    case TYPE_F32: {
        sfcat(&G_allctx, &s, "_vf%d", G_codegen_f32_cnt++);
    } break;
    case TYPE_BOOL: {
        sfcat(&G_allctx, &s, "_vb%d", G_codegen_bool_cnt++);
    } break;
    }

    return s.cstr;
}

char *new_tmp_label(void)
{
    str_t s = { 0 };
    sfcat(&G_allctx, &s, "_lbl%d", G_codegen_jmp_cnt++);
    return s.cstr;
}

char *get_type_label(enum DPCC_TYPE t)
{
    char *result = 0;

    switch (t) {

    default: {
        invalid_code_path();
    } break;

    case TYPE_I32:
    case TYPE_I32_ARRAY: {
        result = "_kI32";
    } break;

    case TYPE_F32:
    case TYPE_F32_ARRAY: {
        result = "_kF32";
    } break;

    case TYPE_BOOL:
    case TYPE_BOOL_ARRAY: {
        result = "_kBOOL";
    } break;
    }

    return result;
}

enum DPCC_TYPE deref_type(enum DPCC_TYPE in)
{
    enum DPCC_TYPE result = TYPE_NONE;

    switch (in) {
    default: {
        invalid_code_path();
    } break;
    case TYPE_BOOL_ARRAY: {
        result = TYPE_BOOL;
    } break;
    case TYPE_I32_ARRAY: {
        result = TYPE_I32;
    } break;
    case TYPE_F32_ARRAY: {
        result = TYPE_F32;
    } break;
    }

    assert(result != TYPE_NONE);
    return result;
}

enum DPCC_TYPE unref_type(enum DPCC_TYPE in)
{
    enum DPCC_TYPE result = TYPE_NONE;

    switch (in) {
    default: {
        invalid_code_path();
    } break;

    case TYPE_BOOL: {
        result = TYPE_BOOL_ARRAY;
    } break;
    case TYPE_I32: {
        result = TYPE_I32_ARRAY;
    } break;
    case TYPE_F32: {
        result = TYPE_F32_ARRAY;
    } break;
    }

    assert(result != TYPE_NONE);
    return result;
}

static void emit_scope_begin(void)
{
    assert(S_indentation >= 0);
    EMIT("_scope_begin();\n");

    S_indentation += 1;
}

static void emit_scope_end(void)
{
    assert(S_indentation > 0);
    S_indentation -= 1;
    EMIT("_scope_end();\n");
}

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

    if (c0->num_childs >= 1 && c0->childs[1] && init_list_len != array_type_len) {
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
    int32_t array_type_len = (c0->num_childs >= 1 && c0->childs[1]) ? c0->childs[1]->val.as_i32 : 0;
    int32_t init_list_len = c2 != NULL ? c2->num_childs : 0;

    n->md.array_len = array_type_len;
    if (c0->num_childs >= 1 && c0->childs[1] && array_type_len <= 0) {
        ERR(c0->childs[1], "The number of elements in an array must be a positive integer");
        INFO(c0->childs[1], "Got `%d`", array_type_len);
    } else if (c2) {
        enum DPCC_TYPE expected_type = deref_type(n->md.type);

        switch (expected_type) {
        default: {
            invalid_code_path();
        } break;
        case TYPE_BOOL: {
            c0->md.type = TYPE_BOOL_ARRAY;
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
    } else if ((c0 == NULL || c0->childs[1] == NULL || c0->childs[1]->kind != IntLit || c0->childs[1]->md.type != TYPE_I32)) {
        ERR(c0, "Size of the array must be specified");
        INFO(c0, "Either specify the size inside the square brackets, or provide an initializer list");
    }
}

static void typecheck_casting_operator(ast_node_t *n)
{
    if (n->md.type) {
        return;
    }

    assert(n->kind == ExprCast);
    assert(n->num_childs == 2);

    switch (n->childs[0]->kind) {
    default: {
        invalid_code_path();
    } break;
    case TypeInfoInt: {
        n->md.type = TYPE_I32;
    } break;
    case TypeInfoFloat: {
        n->md.type = TYPE_F32;
    } break;
    case TypeInfoBool: {
        n->md.type = TYPE_BOOL;
    } break;
    }
}

static void typecheck_array_subscript(ast_node_t *n)
{
    int32_t subscript_idx = n->childs[1]->val.as_i32;
    int32_t array_len = n->childs[0]->decl->md.array_len;

    if (n->childs[0]->md.type != TYPE_BOOL_ARRAY && n->childs[0]->md.type != TYPE_I32_ARRAY && n->childs[0]->md.type != TYPE_F32_ARRAY) {
        ERR(n->childs[0], "Identifier is not an array");
        INFO(n->childs[0]->decl, "Previous declaration was here (type: %s)", dpcc_type_as_str(n->childs[0]->md.type));
    } else if (subscript_idx < 0 || (subscript_idx >= array_len)) {
        WARN(n->childs[1], "Invalid subscript constant");
        INFO(n->childs[0]->decl, "As specified from declaration index should be in [%d, %d)", 0, array_len);
        INFO(n->childs[1], "Got `%d` instead", subscript_idx);
    }

    n->md.type = deref_type(n->childs[0]->md.type);
}

static void typecheck_vardecl(ast_node_t *n)
{
    assert(n->kind == VarDeclStmt);

    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;

    (void)c0, (void)c1, (void)c2;

    ast_node_t *type = c0 ? c0 : NULL;
    ast_node_t *rhs = c2 ? c2 : NULL;

    bool var_decl_with_user_listed_type = c0;
    bool var_decl_no_user_listed_type = !c0;
    bool var_decl_with_rhs = c2;
    bool array_var_decl = (c2 && c2->kind == InitializerList) || (c0 && c0->kind == TypeInfoArray);

    if (array_var_decl) {

        if (var_decl_with_user_listed_type) {
            if (c0->num_childs == 0 || c0->childs[0] == NULL) {
                // Assume integer array
                n->md.type = TYPE_I32_ARRAY;
            } else {
                switch (c0->childs[0]->kind) {
                case TypeInfoBool: {
                    n->md.type = TYPE_BOOL_ARRAY;
                } break;
                default: {
                    invalid_code_path();
                } break;
                case TypeInfoInt: {
                    n->md.type = TYPE_I32_ARRAY;
                } break;
                case TypeInfoFloat: {
                    n->md.type = TYPE_F32_ARRAY;
                } break;
                }
            }

            typecheck_array(n);
        } else if (var_decl_no_user_listed_type) {
            ERR(n, "Expected valid initializer list for array var decls");
        } else {
            invalid_code_path();
        }

    } else {

        if (var_decl_no_user_listed_type && !var_decl_with_rhs) {
            n->md.type = TYPE_I32;
        } else if (var_decl_with_user_listed_type) {

            switch (c0->kind) {
            default: {
                invalid_code_path();
            } break;
            case TypeInfoInt: {
                n->md.type = TYPE_I32;
            } break;
            case TypeInfoFloat: {
                n->md.type = TYPE_F32;
            } break;
            case TypeInfoBool: {
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
    }
}

static void typecheck_print(ast_node_t *n)
{
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    if (c0->md.type) {

        bool is_valid_print = ((c0->md.type == TYPE_I32) || (c0->md.type == TYPE_F32) || (c0->md.type == TYPE_BOOL)
            || (c0->md.type == TYPE_BOOL_ARRAY) || (c0->md.type == TYPE_I32_ARRAY) || (c0->md.type == TYPE_F32_ARRAY)
            || (c0->md.type == TYPE_STR));

        if (!is_valid_print) {
            ERR(c0, "The provided element type (%s) is not printable", c0->tok->lexeme);
            INFO(c0, "Valid printable types are: %s %s %s %s", dpcc_type_as_str(TYPE_I32), dpcc_type_as_str(TYPE_F32), dpcc_type_as_str(TYPE_BOOL), dpcc_type_as_str(TYPE_STR));
        }

        n->md.type = c0->md.type;
    }
}

static void typecheck(ast_node_t *n)
{
    bool is_casting_operator = (n->kind == ExprCast) && (n->num_childs == 2);
    bool var_decl = n->kind == VarDeclStmt;
    bool is_typable_identifier = n->kind == Ident && n->parent && !(n->parent->kind == VarDeclStmt);
    bool is_array_subscript = n->kind == ExprArraySubscript && n->childs[1]->kind == IntLit && n->childs[1]->md.type == TYPE_I32;
    bool is_print = n->kind == PrintStmt;

    if (n->kind == CharLit) {
        n->md.type = TYPE_I32;
    } else if (n->kind == IntLit) {
        n->md.type = TYPE_I32;
    } else if (n->kind == FloatLit) {
        n->md.type = TYPE_F32;
    } else if (n->kind == BoolLit) {
        n->md.type = TYPE_BOOL;
    } else if (n->kind == StringLit) {
        n->md.type = TYPE_STR;
    } else if (n->kind == TypeInfoInt) {
        n->md.type = TYPE_I32;
    } else if (n->kind == TypeInfoFloat) {
        n->md.type = TYPE_F32;
    } else if (n->kind == TypeInfoBool) {
        n->md.type = TYPE_BOOL;
    } else if (n->kind == Ident) {
        if (n->decl) {
            n->md.type = n->decl->md.type;
        }
    } else if (n->md.type == TYPE_NONE) {
        if (is_expr_node(n)) {
            typecheck_expr_and_operators(n);
        }

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
    } else {
    }
}

static char *gen_sym(ast_node_t *n)
{
    assert(n->md.type != TYPE_NONE);
    assert(n->kind != WhileStmt && n->kind != DoWhileStmt && n->kind != ForStmt && n->kind != IfStmt);

    str_t s = { 0 };

    if (n->md.sym) {
        return n->md.sym;
    } else if (n->kind == Ident && !n->md.sym && (n->md.type == TYPE_BOOL_ARRAY || n->md.type == TYPE_I32_ARRAY || n->md.type == TYPE_F32_ARRAY)) {
        sfcat(&G_allctx, &s, "%s", n->tok->lexeme);
    } else if (n->kind == Ident && !n->md.sym) {
        assert(n->decl);
        assert(n->md.type == TYPE_I32 || n->md.type == TYPE_F32 || n->md.type == TYPE_BOOL);
        sfcat(&G_allctx, &s, "_var_get%s(\"%s\", 0)", get_type_label(n->md.type), n->tok->lexeme);
    } else if (n->kind == IntLit || n->kind == FloatLit || n->kind == CharLit || n->kind == BoolLit || n->kind == StringLit) {
        if (n->md.type == TYPE_I32) {
            sfcat(&G_allctx, &s, "%d", n->val.as_i32);
        } else if (n->md.type == TYPE_F32) {
            sfcat(&G_allctx, &s, "%f", n->val.as_f32);
        } else if (n->md.type == TYPE_BOOL) {
            sfcat(&G_allctx, &s, "%i", n->val.as_bool);
        } else if (n->md.type == TYPE_STR) {
            sfcat(&G_allctx, &s, "%s", n->tok->lexeme);
        } else {
            invalid_code_path();
        }
    }
    return s.cstr;
}

static void setup_addrs_and_jmp_tables(ast_node_t *n)
{
    bool is_casting_operator = (n->kind == ExprCast && n->md.type != TYPE_NONE && n->num_childs == 2);
    bool is_expr = (n->md.type != TYPE_NONE && is_expr_node(n));
    if (!n->md.sym && (is_expr || is_casting_operator)) {

        if (n->kind == Ident) {
            assert(n->decl->md.type == n->md.type);
        }
        assert(n->md.type != TYPE_I32_ARRAY);
        assert(n->md.type != TYPE_F32_ARRAY);
        assert(n->kind != WhileStmt && n->kind != DoWhileStmt && n->kind != ForStmt && n->kind != IfStmt);
        n->md.sym = new_tmp_var(n);
    } else if (n->kind == WhileStmt) {
        if (n->md.jmp_bot == NULL || n->md.jmp_top == NULL) {
            n->md.jmp_top = new_tmp_label();
            n->md.jmp_bot = new_tmp_label();
        }
    } else if (n->kind == DoWhileStmt) {
        if (n->md.jmp_top == NULL) {
            n->md.jmp_top = new_tmp_label();
        }
    } else if (n->kind == ForStmt) {
        if (n->md.jmp_top == NULL || n->md.jmp_bot == NULL) {
            n->md.jmp_top = new_tmp_label();
            n->md.jmp_bot = new_tmp_label();
        }
    } else if (n->kind == IfStmt) {
        if (n->md.jmp_next == NULL || n->md.jmp_bot == NULL) {
            n->md.jmp_next = new_tmp_label();
            n->md.jmp_bot = new_tmp_label();
        }
    } else if (n->md.type != TYPE_NONE) {
        assert(n->kind != WhileStmt && n->kind != DoWhileStmt && n->kind != ForStmt && n->kind != IfStmt);
        n->md.sym = gen_sym(n);
    }
}

static void first_ast_pass(void)
{
    ast_traversal_t att = { 0 };
    ast_traversal_begin(&att, &G_root_node);
    ast_node_t *n = NULL;

    while ((n = ast_traverse_next(&att, NULL)) != NULL) {
        typecheck(n);
        setup_addrs_and_jmp_tables(n);
    }
}

static void emit_array_subscript(ast_node_t *n)
{
    assert(n->kind == ExprArraySubscript);
    assert(n->md.type != TYPE_NONE);
    assert(n->md.sym);
    assert(n->num_childs == 2);
    assert(n->childs[0] && n->childs[1]);

    ast_node_t *lhs = n->childs[0];
    ast_node_t *rhs = n->childs[1];

    assert(lhs->kind == Ident);
    assert(lhs->md.type != TYPE_NONE);
    assert(rhs->md.type != TYPE_NONE);

    assert(lhs->decl);
    assert(deref_type(lhs->md.type) == n->md.type);

    EMIT("%s = _var_get%s(\"%s\", %s);\n",
        n->md.sym,
        get_type_label(lhs->md.type),
        lhs->tok->lexeme,
        rhs->md.sym);
}

static void emit_assign(ast_node_t *n)
{
    assert(n->kind == ExprAssign);
    assert(n->num_childs == 2);
    assert(n->childs[0] && n->childs[1]);
    assert(n->md.type);
    assert(n->md.sym);

    ast_node_t *lhs = n->childs[0];
    ast_node_t *rhs = n->childs[1];

    assert(lhs->md.type != TYPE_NONE);
    assert(rhs->md.type != TYPE_NONE);

    if (lhs->kind == ExprArraySubscript) {
        assert(lhs->num_childs == 2);
        assert(lhs->childs[0]);
        assert(lhs->childs[0]->md.type);
        assert(lhs->childs[0]->md.sym);
        assert(lhs->md.sym);

        char *index = lhs->childs[1]->md.sym;

        if (n->md.sym) {
            EMIT("%s = _var_set%s(\"%s\", %s, %s);\n",
                n->md.sym,
                get_type_label(lhs->md.type),
                lhs->childs[0]->tok->lexeme,
                index,
                rhs->md.sym);
        } else {
            EMIT("_var_set%s(\"%s\", %s, %s);\n",
                get_type_label(lhs->md.type),
                lhs->childs[0]->tok->lexeme,
                index,
                rhs->md.sym);
        }

    } else {
        assert(lhs->decl);
        assert(lhs->md.type);
        assert(lhs->md.sym);

        if (lhs->md.sym) {
            EMIT("%s = _var_set%s(\"%s\", 0, %s);\n",
                n->md.sym,
                get_type_label(lhs->md.type),
                lhs->tok->lexeme,
                rhs->md.sym);
        } else {
            EMIT("_var_set%s(\"%s\", 0, %s);\n",
                get_type_label(lhs->md.type),
                lhs->tok->lexeme,
                rhs->md.sym);
        }
    }
}

static void emit_pre_inc_dec(ast_node_t *n)
{
    assert(n->kind == ExprInc || n->kind == ExprDec);
    assert(n->num_childs == 1);
    assert(n->childs[0]);
    assert(n->md.type);
    assert(n->md.sym);

    ast_node_t *lhs = n->childs[0];

    assert(lhs->md.type != TYPE_NONE);
    assert(lhs->decl);

    EMIT("%s = %s;\n", n->md.sym, n->childs[0]->md.sym);
    if (n->kind == ExprInc) {
        EMIT("_vspcIncDec = %s + 1;\n", lhs->md.sym);
    } else {
        EMIT("_vspcIncDec = %s - 1;\n", lhs->md.sym);
    }
    EMIT("_var_set%s(\"%s\", 0, _vspcIncDec);\n",
        get_type_label(lhs->md.type),
        lhs->tok->lexeme);
}

static void emit_expr(ast_node_t *n)
{
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;
    ast_node_t *c3 = (n->num_childs >= 4) ? n->childs[3] : NULL;

    (void)c0, (void)c1, (void)c2, (void)c3;

    bool is_casting_op = n->kind == ExprCast && n->md.type != TYPE_NONE && n->num_childs == 2;

    assert(n->md.sym);

    if (n->num_childs == 0) {
        // base case just stop
        return;
    } else if (n->kind == ExprArraySubscript) {
        emit_array_subscript(n);
    } else if (n->kind == ExprAssign) {
        emit_assign(n);
    } else if (n->kind == ExprInc || n->kind == ExprDec) {
        emit_pre_inc_dec(n);
    } else if (is_casting_op) {
        char *cast = n->childs[0]->tok->lexeme;
        EMIT("%s = (%s) %s;\n", n->md.sym,
            cast, n->childs[1]->md.sym);
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
        } else {
            assert(n->num_childs == 2);
            if (n->kind == ExprPow) {
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
    assert(n->kind == VarDeclStmt);
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;

    assert(c1);
    (void)c0, (void)c1, (void)c2;

    EMIT("_var_decl(\"%s\", %s, %d);\n", n->childs[1]->tok->lexeme, get_type_label(n->md.type), n->md.array_len);
}

static void emit_var_init(ast_node_t *n)
{
    assert(n->kind == VarDeclStmt);
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;

    assert(c1);
    (void)c0, (void)c1, (void)c2;

    bool is_array = n->md.type == TYPE_BOOL_ARRAY || n->md.type == TYPE_I32_ARRAY || n->md.type == TYPE_F32_ARRAY;
    bool has_rhs = c2 != NULL;

    if (has_rhs) {
        char *ctype = "int32_t";
        if (n->md.type == TYPE_F32 || n->md.type == TYPE_F32_ARRAY) {
            ctype = "float";
        } else if ( n->md.type == TYPE_BOOL || n->md.type == TYPE_BOOL_ARRAY) {
            ctype = "bool";
        }

        EMIT("_var_init(\"%s\", %s, %d, (%s[]) {", c1->tok->lexeme, get_type_label(n->md.type), n->md.array_len, ctype);
        if (is_array) {
            assert(c2->kind == InitializerList);
            assert(c2->num_childs >= 1);
            for (int32_t i = 0; i < n->md.array_len; i++) {
                EMITN("%s", c2->childs[i]->md.sym);
                if (i != n->md.array_len - 1) {
                    EMITN(", ");
                }
            }
        } else {
            EMITN("%s", c2->md.sym);
        }

        EMITN("});\n");
    }
}

static void emit_print(ast_node_t *n)
{
    assert(n->kind == PrintStmt);
    assert(n->num_childs == 1);
    assert(n->childs[0]);

    assert(!n->md.sym);
    assert(n->md.type);

    ast_node_t *c0 = n->childs[0];

    assert(c0->md.type);
    assert(c0->md.sym);

    assert(n->md.type == c0->md.type);

    if (c0->kind == Ident) {
        EMIT("print_sym(\"%s\");\n", c0->tok->lexeme);
    } else {
        switch (n->md.type) {
        default: {
            invalid_code_path();
        } break;
        case TYPE_BOOL:
        case TYPE_I32: {
            EMIT("printf(\"%%d\\n\", %s);\n", c0->md.sym);
        } break;
        case TYPE_F32: {
            EMIT("printf(\"%%f\\n\", %s);\n", c0->md.sym);
        } break;
        case TYPE_STR: {
            EMIT("printf(\"%%s\", %s);\n", c0->tok->lexeme);
        } break;
        }
    }
}

static void emit_if(ast_node_t *n, int32_t match_idx)
{
    bool has_else = n->childs[2];

    assert(n->childs[0]);
    assert(n->childs[1]);
    assert(n->num_childs == 3);

    if (match_idx == 0) {
        // Still needs to compute the condition
    } else if (match_idx == 1) {
        ast_node_t *cond = n->childs[0];
        assert(cond->md.sym);
        // condition was just computed
        EMIT("_vspcNeg = !%s;\n", cond->md.sym);
        if (has_else) {
            EMIT("if (_vspcNeg) goto %s;\n", n->md.jmp_next);
        } else {
            EMIT("if (_vspcNeg) goto %s;\n", n->md.jmp_bot);
        }
    } else if (match_idx == 2) {
        // Positive if block has terminated
        // IF --- positive block is terminated
        if (has_else) {
            EMIT("goto %s;\n", n->md.jmp_bot);
            EMIT("%s:\n", n->md.jmp_next);
        } else {
            EMIT("%s:\n", n->md.jmp_bot);
        }
    } else if (match_idx == 3) {
        assert(has_else);
        // IF --- Entire if statement (including the else is terminated)
        EMIT("%s:\n", n->md.jmp_bot);
    } else {
        invalid_code_path();
    }
}

static void emit_for(ast_node_t *n, int32_t match_idx)
{
    assert(n->kind == ForStmt);
    assert(n->num_childs == 4);

    if (match_idx == 0) {
        // FOR --- code block begin (for declaring vars)
        emit_scope_begin();
    } else if (match_idx == 1) {
        // FOR --- compute checking condition
        EMIT("%s:\n", n->md.jmp_top);
    } else if (match_idx == 2) {
        ast_node_t *cond = n->childs[1];
        // Checking condition is optional
        if (cond) {
            assert(cond);
            assert(cond->md.sym);
            // FOR --- terminate if checking condition is false
            EMIT("_vspcNeg = !%s;\n", cond->md.sym);
            EMIT("if (_vspcNeg) goto %s;\n", n->md.jmp_bot);
        }
        // FOR --- Body
        // After the checking condition is computed
    } else if (match_idx == 3) {
        // FOR --- Compute Inc/Dec/Update step and loop
    } else if (match_idx == n->num_childs) {
        // Uncoditionally jump to the top
        EMIT("goto %s;\n", n->md.jmp_top);
        EMIT("%s:\n", n->md.jmp_bot);
        // FOR --- code block end
        emit_scope_end();
    }
}

static void emit_while(ast_node_t *n, int32_t match_idx)
{
    assert(n->kind == WhileStmt);
    assert(n->num_childs == 2);
    assert(n->childs[0]);
    assert(n->childs[1]);

    assert(n->childs[0]->md.type != TYPE_NONE);
    assert(n->childs[0]->md.sym);

    assert(match_idx <= 2);

    if (match_idx == 0) {
        // Before even evaluating the expression
        EMIT("%s:\n", n->md.jmp_top);
    } else if (match_idx == 1) {
        // After the expression evaluation,
        // and before evaluating the entire block
        ast_node_t *cond = n->childs[0];
        EMIT("_vspcNeg = !%s;\n", cond->md.sym);
        EMIT("if (_vspcNeg) goto %s;\n", n->md.jmp_bot);
    } else if (match_idx == 2) {
        EMIT("goto %s;\n", n->md.jmp_top);
        EMIT("%s:\n", n->md.jmp_bot);
    } else {
        invalid_code_path();
    }
}

static void emit_do_while(ast_node_t *n, int32_t match_idx)
{
    assert(n->kind == DoWhileStmt);
    assert(n->num_childs == 2);
    assert(n->childs[0]);
    assert(n->childs[1]);

    assert(n->childs[1]->md.type != TYPE_NONE);
    assert(n->childs[1]->md.sym);

    assert(match_idx <= 2);

    if (match_idx == 0) {
        // Before entering the block
        EMIT("%s:\n", n->md.jmp_top);
    } else if (match_idx == 1) {
        // After the do {} while block, and before the computation of loop check
    } else {
        EMIT("if (%s) goto %s;\n", n->childs[1]->md.sym, n->md.jmp_top);
    }
}

static bool is_control_flow_node(ast_node_t *n)
{
    if (!n) {
        return false;
    }
    bool is_if = n->kind == IfStmt;
    bool is_for = n->kind == ForStmt;
    bool is_while = n->kind == WhileStmt;
    bool is_do = n->kind == DoWhileStmt;

    bool is_control_flow = is_if || is_for || is_while || is_do;

    return is_control_flow;
}

static void emit_dispatch(ast_node_t *n, int32_t match_idx)
{
    bool is_var_decl = n && n->kind == VarDeclStmt;
    bool is_print = n && n->kind == PrintStmt;

    if (n->kind == Stmt) {
        return;
    }
    bool parent_is_control_flow = is_control_flow_node(n->parent);

    if (match_idx == 0) {
        // TOP DOWN ENCOUNTERS
        if (is_var_decl && (n->parent->kind == Stmt || parent_is_control_flow)) {
            emit_var_decl(n);
        }
    } else if (match_idx == n->num_childs) {
        // BOTTOM UP ENCOUNTERS
        if (is_var_decl && (n->parent->kind == Stmt || parent_is_control_flow)) {
            emit_var_init(n);
        } else if (is_print) {
            emit_print(n);
        } else if (n->md.sym) {
            emit_expr(n);
        }
    }

    if (is_control_flow_node(n)) {
        if (n->kind == IfStmt) {
            emit_if(n, match_idx);
        } else if (n->kind == ForStmt) {
            emit_for(n, match_idx);
        } else if (n->kind == WhileStmt) {
            emit_while(n, match_idx);
        } else if (n->kind == DoWhileStmt) {
            emit_do_while(n, match_idx);
        }
    }
}

static void second_ast_pass(void)
{
    memset(&S_str, 0, sizeof(S_str));
    sfcat(&G_allctx, &S_str, "\n");
    S_indentation = 0;

    ast_traversal_t att = { 0 };
    ast_traversal_begin(&att, &G_root_node);
    ast_node_t *n = NULL;
    int32_t match_cnt = 0;

    EMIT("// Special variable used to implemenent INC (x++) and dec (x--)\n");
    EMIT("// It is used to temporary hold the result of the INC/DEC in order to perform the side effect\n");
    EMIT("int32_t _vspcIncDec;\n");
    EMIT("// Special variable used for the negation of control statements (if, for, ...)\n");
    EMIT("// As an example the for loop needs to negate the user provided condition\n");
    EMIT("bool    _vspcNeg;\n");
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

    while ((n = ast_traverse_next(&att, &match_cnt)) != NULL) {
        ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
        ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
        ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;
        ast_node_t *c3 = (n->num_childs >= 4) ? n->childs[3] : NULL;

        (void)c0, (void)c1, (void)c2, (void)c3;

        if (n->kind == CodeBlock && n->parent && (n->parent->kind == RootNode || n->parent->kind == Stmt || is_control_flow_node(n->parent))) {
            if (n->childs == 0) {
                // NOTE
                // Do not generate _scope_begin, _scope_end pair if there's nothing
                // to put inside this scope
            } else {
                if (match_cnt == 0) {
                    emit_scope_begin();
                } else if (match_cnt == n->num_childs) {
                    emit_scope_end();
                }
            }

        } else {
            emit_dispatch(n, match_cnt);
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
