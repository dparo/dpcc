#pragma once

#include <stddef.h>
#include <stdio.h>
#include "types.h"

#include <stb_ds.h>

#define ARGS(...) __VA_ARGS__
#define ARRAY(type, ...) \
    (TYPE[]) { __VA_ARGS__ }
#define ARRAY_LEN(A) (sizeof(A) / sizeof((A)[0]))

/// @Example:
/// ```c
/// typedef struct { int32_t *buf, isize cnt; } int32_array_t;
/// ...
/// int32_array_t a = ARRAY_LIT(int32_t, {5, 6, 7});
/// a.buf[0] -> 5
/// a.cnt -> 3
/// ```
#define ARRAY_LIT(TYPE, ...)                         \
    (TYPE##_array_t)                                 \
    {                                                \
        ((int32_t[])__VA_ARGS__),                    \
            ARRAY_LEN((int32_t[])ARGS(__VA_ARGS__)), \
    }

/// Equivalent to malloc
void *dallnew(mctx_t *ctx, size_t size);
/// Equivalent to realloc
void *dallrsz(mctx_t *ctx, void *ptr, size_t new_size);
/// Equivalent to free
void dalldel(mctx_t *ctx, void *ptr);
/// Clears all memory allocations
void dallclr(mctx_t *ctx);

/// Steal memory allocations from typical malloc
void *dallstl(mctx_t *ctx, void *ptr);

/// Used for string interning
char *lexeme_intern(char *yytext);

void tokens_seq_clear(token_seq_t *tseq);
token_t *token_push(YYLTYPE yylloc, char *yytext, int yychar, char *yychar_str);

void ast_clear(ast_t *ast);
ast_node_t *ast_push(token_t *t, isize num_childs, ast_node_t **childs);

bool str_to_i32(char *string, i32 *out);
bool str_to_f32(char *string, f32 *out);
