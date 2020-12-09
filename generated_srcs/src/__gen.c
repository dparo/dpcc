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
    return (((n->kind == ExprMod) || (n->kind == ExprBNot) || (n->kind == ExprBAnd) || (n->kind == ExprBOr) || (n->kind == ExprBXor) || (n->kind == ExprBLShift) || (n->kind == ExprBRShift) || (n->kind == ExprAssign) || (n->kind == ExprAdd) || (n->kind == ExprSub) || (n->kind == ExprMul) || (n->kind == ExprDiv) || (n->kind == ExprPow) || (n->kind == ExprInc) || (n->kind == ExprDec) || (n->kind == ExprPos) || (n->kind == ExprNeg) || (n->kind == ExprEq) || (n->kind == ExprNeq) || (n->kind == ExprLt) || (n->kind == ExprGt) || (n->kind == ExprGtEq) || (n->kind == ExprLtEq) || (n->kind == ExprLNot) || (n->kind == ExprLAnd) || (n->kind == ExprLOr) || (n->kind == ExprArraySubscript)));
}


void typecheck_expr_and_operators(ast_node_t *n)
{

    if (((n->kind == ExprMod) || (n->kind == ExprBNot) || (n->kind == ExprBAnd) || (n->kind == ExprBOr) || (n->kind == ExprBXor) || (n->kind == ExprBLShift) || (n->kind == ExprBRShift) || (n->kind == ExprAssign) || (n->kind == ExprAdd) || (n->kind == ExprSub) || (n->kind == ExprMul) || (n->kind == ExprDiv) || (n->kind == ExprPow) || (n->kind == ExprInc) || (n->kind == ExprDec) || (n->kind == ExprPos) || (n->kind == ExprNeg) || (n->kind == ExprEq) || (n->kind == ExprNeq) || (n->kind == ExprLt) || (n->kind == ExprGt) || (n->kind == ExprGtEq) || (n->kind == ExprLtEq) || (n->kind == ExprLNot) || (n->kind == ExprLAnd) || (n->kind == ExprLOr) || (n->kind == ExprArraySubscript)))
    {

        if (((n->kind == ExprMod) || (n->kind == ExprBNot) || (n->kind == ExprBAnd) || (n->kind == ExprBOr) || (n->kind == ExprBXor) || (n->kind == ExprBLShift) || (n->kind == ExprBRShift)))
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

            if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_BOOL_ARRAY) && (n->childs[1]->md.type == TYPE_I32)))
            {
                n->md.type = TYPE_BOOL;
            }
            else if ((n->num_childs == 2) && ((n->childs[0]->md.type == TYPE_I32_ARRAY) && (n->childs[1]->md.type == TYPE_I32)))
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

