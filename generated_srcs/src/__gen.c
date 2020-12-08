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



extern int yynerrs;




bool is_prefix_op(ast_node_t *n)
{
    return (((n->kind == ExprPos) || (n->kind == ExprNeg) || (n->kind == ExprBNot) || (n->kind == ExprLNot)));
}


bool is_postfix_op(ast_node_t *n)
{
    return (((n->kind == ExprInc) || (n->kind == ExprDec)));
}


bool is_expr_node(ast_node_t *n)
{
    return (((n->kind == ExprMod) || (n->kind == ExprBNot) || (n->kind == ExprBAnd) || (n->kind == ExprBOr) || (n->kind == ExprBXor) || (n->kind == Expr_BLShift) || (n->kind == Expr_BRShift) || (n->kind == ExprAssign) || (n->kind == ExprAdd) || (n->kind == ExprSub) || (n->kind == ExprMul) || (n->kind == ExprDiv) || (n->kind == ExprPow) || (n->kind == ExprInc) || (n->kind == ExprDec) || (n->kind == ExprPos) || (n->kind == ExprNeg) || (n->kind == ExprEq) || (n->kind == ExprNeq) || (n->kind == ExprLt) || (n->kind == ExprGt) || (n->kind == ExprGtEq) || (n->kind == ExprLtEq) || (n->kind == ExprLNot) || (n->kind == ExprLAnd) || (n->kind == ExprLOr) || (n->kind == ExprArraySubscript)));
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
            result = TYPE_I32;;
        }
        break;
        case TYPE_F32_ARRAY:
        {
            result = TYPE_F32;;
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

    if (((n->kind == ExprMod) || (n->kind == ExprBNot) || (n->kind == ExprBAnd) || (n->kind == ExprBOr) || (n->kind == ExprBXor) || (n->kind == Expr_BLShift) || (n->kind == Expr_BRShift) || (n->kind == ExprAssign) || (n->kind == ExprAdd) || (n->kind == ExprSub) || (n->kind == ExprMul) || (n->kind == ExprDiv) || (n->kind == ExprPow) || (n->kind == ExprInc) || (n->kind == ExprDec) || (n->kind == ExprPos) || (n->kind == ExprNeg) || (n->kind == ExprEq) || (n->kind == ExprNeq) || (n->kind == ExprLt) || (n->kind == ExprGt) || (n->kind == ExprGtEq) || (n->kind == ExprLtEq) || (n->kind == ExprLNot) || (n->kind == ExprLAnd) || (n->kind == ExprLOr) || (n->kind == ExprArraySubscript)))
    {

        if (((n->kind == ExprMod) || (n->kind == ExprBNot) || (n->kind == ExprBAnd) || (n->kind == ExprBOr) || (n->kind == ExprBXor) || (n->kind == Expr_BLShift) || (n->kind == Expr_BRShift)))
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

        if (((n->kind == ExprAssign)))
        {

            if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_I32) && (n->childs[1]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_I32;
            }
            else if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_F32) && (n->childs[1]->md.type == TYPE_F32)))
            {
                n->md.type = TYPE_F32;
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

        if (((n->kind == ExprAdd) || (n->kind == ExprSub) || (n->kind == ExprMul) || (n->kind == ExprDiv) || (n->kind == ExprPow) || (n->kind == ExprInc) || (n->kind == ExprDec) || (n->kind == ExprPos) || (n->kind == ExprNeg)))
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

        if (((n->kind == ExprEq)))
        {

            if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_I32) && (n->childs[1]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_BOOL;
            }
            else
            {
                dpcc_log(DPCC_SEVERITY_ERROR, &((n)->tok->loc), "Types composing this expression cannot be broadcasted");
                yynerrs += 1;
            }
        }

        if (((n->kind == ExprNeq) || (n->kind == ExprLt) || (n->kind == ExprGt) || (n->kind == ExprGtEq) || (n->kind == ExprLtEq)))
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

        if (((n->kind == ExprLNot) || (n->kind == ExprLAnd) || (n->kind == ExprLOr)))
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

        if (((n->kind == ExprArraySubscript)))
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

