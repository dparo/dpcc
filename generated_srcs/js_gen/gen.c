//
// Generated from /home/dparo/develop/dpcc/generated_srcs/dist/main.js
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
                dpcc_log(DPCC_SEVERITY_ERROR, &((n)->tok->locfalse);
                yynerrs ++ 1;
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
                dpcc_log(DPCC_SEVERITY_ERROR, &((n)->tok->locfalse);
                yynerrs ++ 1;
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
                dpcc_log(DPCC_SEVERITY_ERROR, &((n)->tok->locfalse);
                yynerrs ++ 1;
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
                dpcc_log(DPCC_SEVERITY_ERROR, &((n)->tok->locfalse);
                yynerrs ++ 1;
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
                dpcc_log(DPCC_SEVERITY_ERROR, &((n)->tok->locfalse);
                yynerrs ++ 1;
            }
        }
    }
}
