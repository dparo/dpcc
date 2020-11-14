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

typedef struct {
    char* cstr;
    i32 length;
} str;



typedef struct YYLTYPE {
    i32 line;
    i32 column;
} YYLTYPE;


typedef struct mctx {
    void **allocs;
    i32  num_allocs;
} mctx_t;



typedef struct {
    char *lexeme;
    i32  kind;
    char *skind;
    YYLTYPE yylloc;
} token_t;

typedef struct {
    mctx_t  *mctx;
    token_t *tokens;
    i32      tokens_cnt;
} token_seq_t;


typedef struct {
    token_t *tok;

    union {
        i32 i;
        f32 f;
        bool b;
    } val;
} ast_node_t;

typedef struct {
    mctx_t     *mctx;
    ast_node_t *nodes;
    i32         nodes_cnt;
} ast_t;



#define YYSTYPE token_t*
