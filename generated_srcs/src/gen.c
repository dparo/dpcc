//
// generated from {sys.argv[0]}
//

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>


#include "types.h"
#include "globals.h"
#include "utils.h"
#include "log.h"
#include "dpcc.h"

#include "parser.h"
extern int yynerrs;

void typemismatch_check(ast_node_t *expected_type, ast_node_t *got_type)
{
    if (expected_type != NULL && got_type != NULL)
    {
        if (expected_type->md.type != got_type->md.type)
        {
            dpcc_log(DPCC_SEVERITY_ERROR, &expected_type->tok->loc, "Type mismatch" );
            yynerrs += 1;
            dpcc_log(DPCC_SEVERITY_INFO, &expected_type->tok->loc, "Expected `%s`",  dpcc_type_as_str(expected_type->md.type));
            dpcc_log(DPCC_SEVERITY_INFO, &got_type->tok->loc, "But got `%s`",  dpcc_type_as_str(got_type->md.type));
        }
    }
}

static void deduce_array_type(ast_node_t *n)
{
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;
    // Deduce type of array variable declarations
    assert((c0->childs[1] == NULL) || (c0->childs[1]->kind == TOK_I32_LIT && c0->childs[1]->md.type == TYPE_I32));
    int32_t array_type_len = c0->childs[1] ? c0->childs[1]->val.as_i32 : 0;
    int32_t init_list_len = c2 != NULL ? c2->num_childs : 0;
    if(c0->childs[1] && array_type_len <= 0)
    {
        dpcc_log(DPCC_SEVERITY_ERROR, &c0->childs[1]->tok->loc, "The number of elements in an array must be a positive integer" );
        yynerrs += 1;
        dpcc_log(DPCC_SEVERITY_INFO, &c0->childs[1]->tok->loc, "Got `%d`",  array_type_len);
    }
    else
    {
        if (c2)
        {
            // Initializer list is provided
            enum DPCC_TYPE expected_type;

            switch (c0->md.type)
            {
                default:
                {
                    invalid_code_path();
                }
                break;
                case TYPE_I32_ARRAY:
                {
                    expected_type = TYPE_I32;
                }
                break;
                case TYPE_F32_ARRAY:
                {
                    expected_type = TYPE_F32;
                }
                break;
            }

            if (c0->childs[1] && init_list_len != array_type_len)
            {
                dpcc_log(DPCC_SEVERITY_ERROR, &c2->tok->loc, "Number of elements in initializer list do not match" );
                yynerrs += 1;
                dpcc_log(DPCC_SEVERITY_INFO, &c0->childs[1]->tok->loc, "Expected number of elements: `%d`",  array_type_len);
                dpcc_log(DPCC_SEVERITY_INFO, &c2->tok->loc, "Number of elements got: `%d`",  init_list_len);
            }
            else if (c2 && init_list_len <= 0)
            {
                dpcc_log(DPCC_SEVERITY_ERROR, &c2->tok->loc, "Number of elements in initializer list is invalid" );
                yynerrs += 1;
                dpcc_log(DPCC_SEVERITY_INFO, &c2->tok->loc, "Cannot create zero or negative sized array" );
            }
            // Now make sure that each type in the initializer list is correct

            for (int32_t i = 0; i < c2->num_childs; i++)
            {
                if (c2->childs[i] && c2->childs[i]->md.type != expected_type)
                {
                    dpcc_log(DPCC_SEVERITY_ERROR, &c2->childs[i]->tok->loc, "Type mismatch in array initializer list" );
                    yynerrs += 1;
                    dpcc_log(DPCC_SEVERITY_INFO, &c0->childs[0]->tok->loc, "Expected `%s`",  dpcc_type_as_str(c0->childs[0]->md.type));
                    dpcc_log(DPCC_SEVERITY_INFO, &c2->childs[i]->tok->loc, "Got `%s`",  dpcc_type_as_str(c2->childs[i]->md.type));
                }
            }
        }
        else
        {
            // We don't have an initializer list, so make sure that at least the type array is sized
            if (c0 == NULL || c0->childs[1] == NULL || c0->childs[1]->kind != TOK_I32_LIT || c0->childs[1]->md.type != TYPE_I32)
            {
                dpcc_log(DPCC_SEVERITY_ERROR, &c0->tok->loc, "Size of the array must be specified" );
                yynerrs += 1;
                dpcc_log(DPCC_SEVERITY_INFO, &c0->tok->loc, "Either specify the size inside the square brackets, or provide an initializer list" );
            }
        }
    }
    // Forward the same type to the keyword let
    n->md.type = c0->md.type;
    n->md.array_len = init_list_len;
}

static void type_deduce_expr_and_operators(ast_node_t *n)
{
    if (((n->kind == TOK_MOD) || (n->kind == TOK_BNOT) || (n->kind == TOK_BAND) || (n->kind == TOK_BOR) || (n->kind == TOK_BXOR) || (n->kind == TOK_BLSHIFT) || (n->kind == TOK_BRSHIFT) || (n->kind == TOK_ASSIGN) || (n->kind == TOK_ADD) || (n->kind == TOK_SUB) || (n->kind == TOK_MUL) || (n->kind == TOK_DIV) || (n->kind == TOK_POW) || (n->kind == TOK_INC) || (n->kind == TOK_DEC) || (n->kind == TOK_POS) || (n->kind == TOK_NEG) || (n->kind == TOK_EQ) || (n->kind == TOK_NEQ) || (n->kind == TOK_LT) || (n->kind == TOK_GT) || (n->kind == TOK_GTEQ) || (n->kind == TOK_LTEQ) || (n->kind == TOK_LNOT) || (n->kind == TOK_LAND) || (n->kind == TOK_LOR) || (n->kind == TOK_AR_SUBSCR)))
    {
        if (((n->kind == TOK_MOD) || (n->kind == TOK_BNOT) || (n->kind == TOK_BAND) || (n->kind == TOK_BOR) || (n->kind == TOK_BXOR) || (n->kind == TOK_BLSHIFT) || (n->kind == TOK_BRSHIFT)))
        {
            if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_I32) && (n->childs[1]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_I32;
            }
            else
            {
                dpcc_log(DPCC_SEVERITY_ERROR, &n->tok->loc, "Types composing this expression cannot be broadcasted." );
                yynerrs += 1;
            }
        }
        if (((n->kind == TOK_ASSIGN) || (n->kind == TOK_ADD) || (n->kind == TOK_SUB) || (n->kind == TOK_MUL) || (n->kind == TOK_DIV) || (n->kind == TOK_POW) || (n->kind == TOK_INC) || (n->kind == TOK_DEC) || (n->kind == TOK_POS) || (n->kind == TOK_NEG)))
        {
            if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_I32) && (n->childs[1]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_I32;
            }
            else if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_F32) && (n->childs[1]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_F32;
            }
            else if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_I32) && (n->childs[1]->md.type == TYPE_F32)))
            {
                n->md.type = TYPE_F32;
            }
            else if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_F32) && (n->childs[1]->md.type == TYPE_F32)))
            {
                n->md.type = TYPE_F32;
            }
            else if ((n->num_childs == 1) && ((n->childs[0]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_I32;
            }
            else if ((n->num_childs == 1) && ((n->childs[0]->md.type == TYPE_F32)))
            {
                n->md.type = TYPE_F32;
            }
            else
            {
                dpcc_log(DPCC_SEVERITY_ERROR, &n->tok->loc, "Types composing this expression cannot be broadcasted." );
                yynerrs += 1;
            }
        }
        if (((n->kind == TOK_EQ) || (n->kind == TOK_NEQ) || (n->kind == TOK_LT) || (n->kind == TOK_GT) || (n->kind == TOK_GTEQ) || (n->kind == TOK_LTEQ)))
        {
            if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_I32) && (n->childs[1]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_BOOL;
            }
            else if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_F32) && (n->childs[1]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_BOOL;
            }
            else if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_I32) && (n->childs[1]->md.type == TYPE_F32)))
            {
                n->md.type = TYPE_BOOL;
            }
            else if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_F32) && (n->childs[1]->md.type == TYPE_F32)))
            {
                n->md.type = TYPE_BOOL;
            }
            else
            {
                dpcc_log(DPCC_SEVERITY_ERROR, &n->tok->loc, "Types composing this expression cannot be broadcasted." );
                yynerrs += 1;
            }
        }
        if (((n->kind == TOK_LNOT) || (n->kind == TOK_LAND) || (n->kind == TOK_LOR)))
        {
            if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_BOOL) && (n->childs[1]->md.type == TYPE_BOOL)))
            {
                n->md.type = TYPE_BOOL;
            }
            else if ((n->num_childs == 1) && ((n->childs[0]->md.type == TYPE_BOOL)))
            {
                n->md.type = TYPE_BOOL;
            }
            else
            {
                dpcc_log(DPCC_SEVERITY_ERROR, &n->tok->loc, "Types composing this expression cannot be broadcasted." );
                yynerrs += 1;
            }
        }
        if (((n->kind == TOK_AR_SUBSCR)))
        {
            if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_I32_ARRAY) && (n->childs[1]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_I32;
            }
            else if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_F32_ARRAY) && (n->childs[1]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_F32;
            }
            else
            {
                dpcc_log(DPCC_SEVERITY_ERROR, &n->tok->loc, "Types composing this expression cannot be broadcasted." );
                yynerrs += 1;
            }
        }
    }
}

static void type_deduce(ast_node_t *n)
{
    // Utilities vars t oeasily access childs
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;
    // Base cases for type deduction

    switch (n->kind)
    {
        default: { /* EMPTY */ } break;
        case TOK_CHAR_LIT:
        {
            n->md.type = TYPE_I32;
        }
        break;
        case TOK_I32_LIT:
        {
            n->md.type = TYPE_I32;
        }
        break;
        case TOK_F32_LIT:
        {
            n->md.type = TYPE_F32;
        }
        break;
        case TOK_BOOL_LIT:
        {
            n->md.type = TYPE_BOOL;
        }
        break;
        case TOK_ID:
        {
            if (n->decl) n->md.type = n->decl->md.type;
        }
        break;
    }

    // Assign the correct type to each casting operator
    if (((n->kind == TOK_KW_INT) || (n->kind == TOK_KW_FLOAT) || (n->kind == TOK_KW_BOOL)) && !((n->parent->kind == TOK_KW_FN) || (n->parent->kind == TOK_KW_LET)))
    {

        switch (n->kind)
        {
            default:
            {
                invalid_code_path();
            }
            break;
            case TOK_KW_INT:
            {
                n->md.type = TYPE_I32;
            }
            break;
            case TOK_KW_FLOAT:
            {
                n->md.type = TYPE_F32;
            }
            break;
            case TOK_KW_BOOL:
            {
                n->md.type = TYPE_BOOL;
            }
            break;
        }

    }
    // Assign the correct type if the var declaration has a user listed type
    if (c0 && (((n->kind == TOK_KW_FN) || (n->kind == TOK_KW_LET))))
    {
        if (c0->md.type == TYPE_NONE && c0->kind != TOK_OPEN_BRACKET)
        {
            // Handle integral types

            switch (c0->kind)
            {
                default:
                {
                    invalid_code_path();
                }
                break;
                case TOK_KW_INT:
                {
                    c0->md.type = TYPE_I32;
                }
                break;
                case TOK_KW_FLOAT:
                {
                    c0->md.type = TYPE_F32;
                }
                break;
                case TOK_KW_BOOL:
                {
                    c0->md.type = TYPE_BOOL;
                }
                break;
            }

        }
        else if (c0->md.type == TYPE_NONE && c0->kind == TOK_OPEN_BRACKET)
        {
            // Handle array types

            switch (c0->childs[0]->kind)
            {
                default:
                {
                    invalid_code_path();
                }
                break;
                case TOK_KW_INT:
                {
                    c0->md.type = TYPE_I32_ARRAY;
                }
                break;
                case TOK_KW_FLOAT:
                {
                    c0->md.type = TYPE_F32_ARRAY;
                }
                break;
            }

        }
        // Forward the same type to the keyword let
        n->md.type = c0->md.type;
    }
    // Deduce type of variable declarations
    if ((n->kind == TOK_KW_FN) || (n->kind == TOK_KW_LET))
    {
        // Deduce type of integral variable declarations
        if ((c0 == NULL) || (c0->kind != TOK_OPEN_BRACKET))
        {
            ast_node_t *var_decl_type = c0;
            ast_node_t *child_type = c2;
            if (var_decl_type != NULL && child_type != NULL)
            {
                // We might have type mismatch
                typemismatch_check(var_decl_type, child_type);
            }
            else if (child_type != NULL)
            {
                // Type deduce from child
                n->md.type = child_type->md.type;
            }
            else
            {
                // Assume integer
                n->md.type = TYPE_I32;
            }
        }
        else
        {
            deduce_array_type(n);
        }
    }
    // Deduce the type of each identifier used
    if (n->kind == TOK_ID && n->parent && !((n->parent->kind == TOK_KW_FN) || (n->parent->kind == TOK_KW_LET)))
    {
        assert(n->decl != NULL);
        assert(n->decl->md.type != TYPE_NONE);
        n->md.type = n->decl->md.type;
    }
    // Deduce type of expression and operators: This is the most demanding and difficult part
    if (n->md.type == 0)
    {
        type_deduce_expr_and_operators(n);
    }
    // Check sensisble array subscript bounds
    if (n->kind == TOK_AR_SUBSCR && n->childs[1]->kind == TOK_I32_LIT && n->childs[1]->md.type == TYPE_I32)
    {
        int32_t subscript_idx = n->childs[1]->val.as_i32;
        int32_t array_len = n->childs[0]->decl->md.array_len;
        if (n->childs[0]->md.type != TYPE_I32_ARRAY && n->childs[0]->md.type != TYPE_F32_ARRAY)
        {
            dpcc_log(DPCC_SEVERITY_ERROR, &n->childs[0]->tok->loc, "Identifier is not an array" );
            yynerrs += 1;
            dpcc_log(DPCC_SEVERITY_INFO, &n->childs[0]->decl->tok->loc, "Previous declaration was here (type: %s)",  dpcc_type_as_str(n->childs[0]->md.type));
        }
        else if (subscript_idx < 0 || (subscript_idx >= array_len))
        {
            dpcc_log(DPCC_SEVERITY_ERROR, &n->childs[1]->tok->loc, "Invalid subscript constant" );
            yynerrs += 1;
            dpcc_log(DPCC_SEVERITY_INFO, &n->childs[0]->decl->tok->loc, "As specified from declaration index should be in [%d, %d)",  0, array_len);
            dpcc_log(DPCC_SEVERITY_INFO, &n->childs[1]->tok->loc, "Got `%d` instead",  subscript_idx);
        }
    }
}

static char *get_tmp_var(enum DPCC_TYPE type)
{
    str_t s = {0};

    switch (type)
    {
        default:
        {
            invalid_code_path();
        }
        break;
        case TYPE_I32:
        {
            sfcat(&G_allctx, &s, "__i%d", G_codegen_i32_cnt++);
        }
        break;
        case TYPE_F32:
        {
            sfcat(&G_allctx, &s, "__f%d", G_codegen_f32_cnt++);
        }
        break;
        case TYPE_BOOL:
        {
            sfcat(&G_allctx, &s, "__b%d", G_codegen_bool_cnt++);
        }
        break;
    }

    return s.cstr;
}

static char *get_tmp_label()
{
    str_t s = {0};
    sfcat(&G_allctx, &s, "__l%d", G_codegen_jmp_cnt++);
    return s.cstr;
}

static void setup_addrs_and_jmp_tables(ast_node_t *n)
{
    if (!n->md.addr)
    {
        // Generate address for all temporary computations performed by operators
        if (n->md.type != TYPE_NONE && ((n->kind == TOK_MOD) || (n->kind == TOK_BNOT) || (n->kind == TOK_BAND) || (n->kind == TOK_BOR) || (n->kind == TOK_BXOR) || (n->kind == TOK_BLSHIFT) || (n->kind == TOK_BRSHIFT) || (n->kind == TOK_ASSIGN) || (n->kind == TOK_ADD) || (n->kind == TOK_SUB) || (n->kind == TOK_MUL) || (n->kind == TOK_DIV) || (n->kind == TOK_POW) || (n->kind == TOK_INC) || (n->kind == TOK_DEC) || (n->kind == TOK_POS) || (n->kind == TOK_NEG) || (n->kind == TOK_EQ) || (n->kind == TOK_NEQ) || (n->kind == TOK_LT) || (n->kind == TOK_GT) || (n->kind == TOK_GTEQ) || (n->kind == TOK_LTEQ) || (n->kind == TOK_LNOT) || (n->kind == TOK_LAND) || (n->kind == TOK_LOR) || (n->kind == TOK_AR_SUBSCR)))
        {
            assert(n->md.type != TYPE_I32_ARRAY);
            assert(n->md.type != TYPE_F32_ARRAY);
            n->md.addr = get_tmp_var(n->md.type);
        }
    }
    if (((n->kind == TOK_KW_IF) || (n->kind == TOK_KW_DO) || (n->kind == TOK_KW_WHILE) || (n->kind == TOK_KW_FOR)))
    {
        if (n->kind == TOK_KW_WHILE && n->md.jmp_bot == NULL)
        {
            n->md.jmp_bot = get_tmp_label();
        }
        else if (n->kind == TOK_KW_DO && n->md.jmp_top == NULL)
        {
            n->md.jmp_top = get_tmp_label();
        }
        else if (n->kind == TOK_KW_FOR && n->md.jmp_top == NULL)
        {
            n->md.jmp_top = get_tmp_label();
        }
        else if ((n->kind == TOK_KW_IF) && (n->md.jmp_next == NULL || n->md.jmp_bot == NULL))
        {
            n->md.jmp_next = get_tmp_label();
            n->md.jmp_bot = get_tmp_label();
        }
        else
        {
            invalid_code_path();
        }
    }
}

void check_and_optimize_ast(void)
{
    ast_traversal_t att = {0};
    ast_traversal_begin(&att, &G_root_node, false, true);
    {
        ast_node_t *n = NULL;

        while ((n = ast_traverse_next(&att, NULL)) != NULL)
        {
            if (n->md.type == TYPE_NONE)
            {
                type_deduce(n);
            }
            setup_addrs_and_jmp_tables(n);
        }
    }
}

void codegen_expr(str_t *str, ast_node_t *root)
{
    (void) str, (void) root;
    assert(((root->kind == TOK_MOD) || (root->kind == TOK_BNOT) || (root->kind == TOK_BAND) || (root->kind == TOK_BOR) || (root->kind == TOK_BXOR) || (root->kind == TOK_BLSHIFT) || (root->kind == TOK_BRSHIFT) || (root->kind == TOK_ASSIGN) || (root->kind == TOK_ADD) || (root->kind == TOK_SUB) || (root->kind == TOK_MUL) || (root->kind == TOK_DIV) || (root->kind == TOK_POW) || (root->kind == TOK_INC) || (root->kind == TOK_DEC) || (root->kind == TOK_POS) || (root->kind == TOK_NEG) || (root->kind == TOK_EQ) || (root->kind == TOK_NEQ) || (root->kind == TOK_LT) || (root->kind == TOK_GT) || (root->kind == TOK_GTEQ) || (root->kind == TOK_LTEQ) || (root->kind == TOK_LNOT) || (root->kind == TOK_LAND) || (root->kind == TOK_LOR) || (root->kind == TOK_AR_SUBSCR)));
    ast_traversal_t att = {0};
    ast_traversal_begin(&att, root, false, true);
    ast_node_t *n = NULL;

    while ((n = ast_traverse_next(&att, NULL)) != NULL)
    {
        if (n->kind == TOK_ASSIGN)
        {
            // Extra care here, we need to generate the proper code to assign to user declared identifiers
        }
        else if (n->kind == TOK_OPEN_BRACE)
        {
        }
        else
        {
        }
    }
}

char *codegen(void)
{
    str_t str = {0};
    ast_traversal_t att = {0};
    ast_traversal_begin(&att, &G_root_node, true, true);
    sfcat(&G_allctx, &str, "\n");
    ast_node_t *n = NULL;
    bool is_top_down_encounter = false;

    while ((n = ast_traverse_next(&att, &is_top_down_encounter)) != NULL)
    {
        ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
        ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
        ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;
        ast_node_t *c3 = (n->num_childs >= 4) ? n->childs[3] : NULL;
        (void) c0; (void) c1; (void) c2; (void) c3;
        if (n->kind == TOK_KW_MAIN && n->childs[0]->kind == TOK_OPEN_BRACE)
        {
            // Don't need to do anything, can just skip
        }
        else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_IF)
        {
        }
        else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_WHILE)
        {
        }
        else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_DO)
        {
        }
        else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_FOR)
        {
        }
        else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_LET)
        {
            if (is_top_down_encounter)
            {
                ast_node_t *self = n->childs[0];
                ast_node_t *c0 = (self->num_childs >= 1) ? self->childs[0] : NULL;
                ast_node_t *c1 = (self->num_childs >= 2) ? self->childs[1] : NULL;
                ast_node_t *c2 = (self->num_childs >= 3) ? self->childs[2] : NULL;
                ast_node_t *c3 = (self->num_childs >= 4) ? self->childs[3] : NULL;
                (void) c0; (void) c1; (void) c2; (void) c3;
                sfcat(&G_allctx, &str,"decl_var(%s, \"%s\", %d, ", dpcc_type_as_enum_str(self->md.type), c1->tok->lexeme, c1->md.array_len);

                switch (self->md.type)
                {
                    default:
                    {
                        invalid_code_path();
                    }
                    break;
                    case TYPE_I32:
                    {
                        sfcat(&G_allctx, &str, "(int32_t[]) {");
                    }
                    break;
                    case TYPE_I32_ARRAY:
                    {
                        sfcat(&G_allctx, &str, "(int32_t[]) {");
                    }
                    break;
                    case TYPE_F32:
                    {
                        sfcat(&G_allctx, &str, "(float[]) {");
                    }
                    break;
                    case TYPE_F32_ARRAY:
                    {
                        sfcat(&G_allctx, &str, "(float[]) {");
                    }
                    break;
                    case TYPE_BOOL:
                    {
                        sfcat(&G_allctx, &str, "(bool[]) {");
                    }
                    break;
                }

                sfcat(&G_allctx, &str, "});\n");
            }
        }
        else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_PRINT)
        {
        }
        else if (n->kind == TOK_SEMICOLON)
        {
        }
        else if (n->kind == TOK_OPEN_BRACE)
        {
            if (is_top_down_encounter)
            {
                sfcat(&G_allctx, &str, "push();\n");
            }
            else
            {
                sfcat(&G_allctx, &str, "pop();\n");
            }
        }
        else if (((n->kind == TOK_MOD) || (n->kind == TOK_BNOT) || (n->kind == TOK_BAND) || (n->kind == TOK_BOR) || (n->kind == TOK_BXOR) || (n->kind == TOK_BLSHIFT) || (n->kind == TOK_BRSHIFT) || (n->kind == TOK_ASSIGN) || (n->kind == TOK_ADD) || (n->kind == TOK_SUB) || (n->kind == TOK_MUL) || (n->kind == TOK_DIV) || (n->kind == TOK_POW) || (n->kind == TOK_INC) || (n->kind == TOK_DEC) || (n->kind == TOK_POS) || (n->kind == TOK_NEG) || (n->kind == TOK_EQ) || (n->kind == TOK_NEQ) || (n->kind == TOK_LT) || (n->kind == TOK_GT) || (n->kind == TOK_GTEQ) || (n->kind == TOK_LTEQ) || (n->kind == TOK_LNOT) || (n->kind == TOK_LAND) || (n->kind == TOK_LOR) || (n->kind == TOK_AR_SUBSCR)))
        {
            codegen_expr(&str, n);
            ast_traversal_pop(&att);
        }
        else
        {
            // invalid_code_path();
        }
    }
    sfcat(&G_allctx, &str, "\n");
    return str.cstr;
}
