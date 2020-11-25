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
static void invalid_code_path(void)
{
    fprintf(stderr, "COMPILER INTERNAL ERROR --- Invalid code path");
    fflush(stderr);
    assert(0);
}
static char* type_as_str(int x)
{
    switch (x)
    {
        default:
        {
            invalid_code_path();
        }
        break;
        case TYPE_I32:
        {
            return "int";
        }
        break;
        case TYPE_F32:
        {
            return "float";
        }
        break;
        case TYPE_BOOL:
        {
            return "bool";
        }
        break;
    }
}
void typemismatch_check(ast_node_t *expected_type, ast_node_t *got_type)
{
    if (expected_type != NULL && got_type != NULL)
    {
        if (expected_type->md.type != got_type->md.type)
        {
            dpcc_log(DPCC_SEVERITY_ERROR, &expected_type->tok->loc, "Type mismatch" );
            yynerrs += 1;
            dpcc_log(DPCC_SEVERITY_INFO, &expected_type->tok->loc, "Expected `%s`",  type_as_str(expected_type->md.type));
            dpcc_log(DPCC_SEVERITY_INFO, &got_type->tok->loc, "But got `%s`",  type_as_str(got_type->md.type));
        }
    }
}
void check_and_optimize_ast(void)
{
    ast_traversal_t att = {0};
    ast_traversal_begin(&att);
    {
        ast_node_t *n = NULL;
        while ((n = ast_traverse_next(&att)) != NULL)
        {
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
            if ((n->kind == TOK_KW_FN) || (n->kind == TOK_KW_LET))
            {
                if (n->childs[0] && n->childs[0]->md.type == TYPE_NONE)
                {
                    switch (n->childs[0]->kind)
                    {
                        default:
                        {
                            invalid_code_path();
                        }
                        break;
                        case TOK_KW_INT:
                        {
                            n->childs[0]->md.type = TYPE_I32;
                        }
                        break;
                        case TOK_KW_FLOAT:
                        {
                            n->childs[0]->md.type = TYPE_F32;
                        }
                        break;
                        case TOK_KW_BOOL:
                        {
                            n->childs[0]->md.type = TYPE_BOOL;
                        }
                        break;
                    }
                }
            }
            // Deduce type of the variable declarations
            if ((n->kind == TOK_KW_FN) || (n->kind == TOK_KW_LET))
            {
                ast_node_t *var_decl_type = n->childs[0];
                ast_node_t *child_type = n->childs[2];
                if (var_decl_type != NULL && child_type != NULL)
                {
                    // We might have type mismatch
                    typemismatch_check(var_decl_type, child_type);
                }
                else if (child_type != NULL)
                {
                    // Type deduce from child
                    n->md.type = n->childs[1]->md.type;
                }
                else
                {
                    // Assume integer
                    n->md.type = TYPE_I32;
                }
            }
        }
    }
}
