#pragma once

#include "math.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef int8_t i8;
typedef uint8_t u8;

typedef int16_t i16;
typedef uint16_t u16;

typedef int32_t i32;
typedef uint32_t u32;

typedef int64_t i64;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef size_t usize;
typedef ptrdiff_t isize;

typedef char *cstr_t;

typedef struct {
    int32_t *buf;
    isize cnt;
} int32_array_t, i32_array_t, int32_t_array_t;

typedef struct YYLTYPE {
    i32 line;
    i32 column;
} YYLTYPE;

typedef struct mctx {
    void **allocs;
    i32 num_allocs;
} mctx_t;

typedef struct {
    i32 idx; /// Index withing the array containing all the tokens

    char *lexeme;
    i32 kind;
    char *skind;
    YYLTYPE yylloc;
} token_t;

typedef struct {
    token_t *tokens;
    i32 tokens_cnt;
} token_seq_t;

typedef struct ast_node {
    token_t *tok;
    i32 kind;
    char *skind;

    struct ast_node **childs;

    union {
        i32 i;
        f32 f;
        char c;
        bool b;
    } val;
} ast_node_t;

typedef struct {
    ast_node_t *nodes;
    i32 nodes_cnt;
} ast_t;

typedef struct {
    char *key;
    char *value;
} str_str_key_value_t;

#define YYSTYPE token_t *
