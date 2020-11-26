//
// generated from /home/dparo/Dropbox/UniversityProjects/compilatori/generated_srcs/gen_all.py
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
static void type_deduce(ast_node_t *n)
{
    // Utilities vars t oeasily access childs
    ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;
    ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;
    ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;
    ast_node_t *c3 = (n->num_childs >= 4) ? n->childs[3] : NULL;
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
    }
    // Deduce type of expression and operators: This is the most demanding and difficult part
}
void check_and_optimize_ast(void)
{
    ast_traversal_t att = {0};
    ast_traversal_begin(&att);
    {
        ast_node_t *n = NULL;
        while ((n = ast_traverse_next(&att)) != NULL)
        {
            if (n->md.type == TYPE_NONE)
            {
                type_deduce(n);
            }
        }
    }
}
