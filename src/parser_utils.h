#pragma once

#include "types.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>

typedef struct {
    char *lexeme;

    i32 kind;          /// look at parser.c :: enum yysymbol_kind_t
    char *skind;       /// same as kind, but it's string representation

    union {
        i32 i;
        f32 f;
        bool b;
    } val;
} ast_node_t;


typedef struct {
    ast_node_t *nodes;
    i32         nodes_cnt;
} ast_t;


ast_node_t *ast_push(mctx_t *mctx, ast_t *ast, char *lexeme, i32 kind, char *skind);


bool str_to_i32(char* string, i32* out);
bool str_to_f32(char* string, f32* out);
