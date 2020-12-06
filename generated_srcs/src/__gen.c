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


#include "__gen.h"
#include "parser.h"



extern int yynerrs;




bool is_prefix_op(ast_node_t *n)
{
    return (((n->kind == TOK_POS) || (n->kind == TOK_NEG) || (n->kind == TOK_BNOT) || (n->kind == TOK_LNOT)));
}


bool is_postfix_op(ast_node_t *n)
{
    return (((n->kind == TOK_INC) || (n->kind == TOK_DEC)));
}


bool is_expr_node(ast_node_t *n)
{
    return (((n->kind == TOK_MOD) || (n->kind == TOK_BNOT) || (n->kind == TOK_BAND) || (n->kind == TOK_BOR) || (n->kind == TOK_BXOR) || (n->kind == TOK_BLSHIFT) || (n->kind == TOK_BRSHIFT) || (n->kind == TOK_ASSIGN) || (n->kind == TOK_ADD) || (n->kind == TOK_SUB) || (n->kind == TOK_MUL) || (n->kind == TOK_DIV) || (n->kind == TOK_POW) || (n->kind == TOK_INC) || (n->kind == TOK_DEC) || (n->kind == TOK_POS) || (n->kind == TOK_NEG) || (n->kind == TOK_EQ) || (n->kind == TOK_NEQ) || (n->kind == TOK_LT) || (n->kind == TOK_GT) || (n->kind == TOK_GTEQ) || (n->kind == TOK_LTEQ) || (n->kind == TOK_LNOT) || (n->kind == TOK_LAND) || (n->kind == TOK_LOR) || (n->kind == TOK_AR_SUBSCR)));
}


char *new_tmp_var(ast_node_t *n)
{
    str_t s = {0};

    switch (n->md.type)
    {
        default:
        {
            invalid_code_path();
        }
        break;
        case TYPE_I32:
        {
            sfcat(&G_allctx, &s, "_vi%d", G_codegen_i32_cnt++);
        }
        break;
        case TYPE_F32:
        {
            sfcat(&G_allctx, &s, "_vf%d", G_codegen_f32_cnt++);
        }
        break;
        case TYPE_BOOL:
        {
            sfcat(&G_allctx, &s, "_vb%d", G_codegen_bool_cnt++);
        }
        break;
    }

    return s.cstr;
}


char *new_tmp_label(void)
{
    str_t s = {0};
    sfcat(&G_allctx, &s, "_lbl%d", G_codegen_jmp_cnt++);
    return s.cstr;
}


char *get_type_label(enum DPCC_TYPE t)
{
    char *result = 0;

    switch (t)
    {
        default:
        {
            invalid_code_path();
        }
        break;
        case TYPE_I32:
        {
            result = "_kI32";
        }
        break;
        case TYPE_F32:
        {
            result = "_kF32";
        }
        break;
        case TYPE_BOOL:
        {
            result = "_kBOOL";
        }
        break;
        case TYPE_I32_ARRAY:
        {
            result = "_kI32";
        }
        break;
        case TYPE_F32_ARRAY:
        {
            result = "_kF32";
        }
        break;
    }

    return result;
}


enum DPCC_TYPE deref_type(enum DPCC_TYPE in)
{
    enum DPCC_TYPE result = TYPE_NONE;

    switch (in)
    {
        default:
        {
            invalid_code_path();
        }
        break;
        case TYPE_I32_ARRAY:
        {
            result = TYPE_I32_ARRAY;;
        }
        break;
        case TYPE_F32_ARRAY:
        {
            result = TYPE_F32_ARRAY;;
        }
        break;
    }

    assert(result != TYPE_NONE);
    return result;
}


enum DPCC_TYPE unref_type(enum DPCC_TYPE in)
{
    enum DPCC_TYPE result = TYPE_NONE;

    switch (in)
    {
        default:
        {
            invalid_code_path();
        }
        break;
        case TYPE_I32:
        {
            result = TYPE_I32_ARRAY;;
        }
        break;
        case TYPE_F32:
        {
            result = TYPE_F32_ARRAY;;
        }
        break;
    }

    assert(result != TYPE_NONE);
    return result;
}


void typecheck_expr_and_operators(ast_node_t *n)
{

    if (((n->kind == TOK_MOD) || (n->kind == TOK_BNOT) || (n->kind == TOK_BAND) || (n->kind == TOK_BOR) || (n->kind == TOK_BXOR) || (n->kind == TOK_BLSHIFT) || (n->kind == TOK_BRSHIFT) || (n->kind == TOK_ASSIGN) || (n->kind == TOK_ADD) || (n->kind == TOK_SUB) || (n->kind == TOK_MUL) || (n->kind == TOK_DIV) || (n->kind == TOK_POW) || (n->kind == TOK_INC) || (n->kind == TOK_DEC) || (n->kind == TOK_POS) || (n->kind == TOK_NEG) || (n->kind == TOK_EQ) || (n->kind == TOK_NEQ) || (n->kind == TOK_LT) || (n->kind == TOK_GT) || (n->kind == TOK_GTEQ) || (n->kind == TOK_LTEQ) || (n->kind == TOK_LNOT) || (n->kind == TOK_LAND) || (n->kind == TOK_LOR) || (n->kind == TOK_AR_SUBSCR)))
    {

        if (((n->kind == TOK_MOD) || (n->kind == TOK_BNOT) || (n->kind == TOK_BAND) || (n->kind == TOK_BOR) || (n->kind == TOK_BXOR) || (n->kind == TOK_BLSHIFT) || (n->kind == TOK_BRSHIFT)))
        {

            if ((n->num_childs == 1) && ((n->childs[0]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_I32;
            }
            else if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_I32) && (n->childs[1]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_I32;
            }
            else
            {
                dpcc_log(DPCC_SEVERITY_ERROR, &((n)->tok->loc), "Types composing this expression cannot be broadcasted");
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
                dpcc_log(DPCC_SEVERITY_ERROR, &((n)->tok->loc), "Types composing this expression cannot be broadcasted");
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
                dpcc_log(DPCC_SEVERITY_ERROR, &((n)->tok->loc), "Types composing this expression cannot be broadcasted");
                yynerrs += 1;
            }
        }

        if (((n->kind == TOK_LNOT) || (n->kind == TOK_LAND) || (n->kind == TOK_LOR)))
        {

            if ((n->num_childs == 1) && ((n->childs[0]->md.type == TYPE_BOOL)))
            {
                n->md.type = TYPE_BOOL;
            }
            else if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_BOOL) && (n->childs[1]->md.type == TYPE_BOOL)))
            {
                n->md.type = TYPE_BOOL;
            }
            else
            {
                dpcc_log(DPCC_SEVERITY_ERROR, &((n)->tok->loc), "Types composing this expression cannot be broadcasted");
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
                dpcc_log(DPCC_SEVERITY_ERROR, &((n)->tok->loc), "Types composing this expression cannot be broadcasted");
                yynerrs += 1;
            }
        }
    }
}

