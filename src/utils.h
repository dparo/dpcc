#pragma once


#include <stddef.h>
#include <stdio.h>
#include "types.h"

#define ARRAY_LEN(A) (sizeof(A) / sizeof((A)[0]))

/// Equivalent to malloc
void *dallnew(mctx_t *ctx, size_t size);
/// Equivalent to realloc
void *dallrsz(mctx_t *ctx, void *ptr, size_t new_size);
/// Equivalent to free
void dalldel(mctx_t *ctx, void *ptr);
/// Clears all memory allocations
void dallclr(mctx_t *ctx);

/// Steal memory allocations from typical malloc
void* dallstl(mctx_t* ctx, void* ptr);


void ast_clear(ast_t* ast);
ast_node_t *ast_push(ast_t *ast, YYLTYPE yylloc, char *lexeme, i32 kind, char *skind);

void     tokens_seq_clear(token_seq_t *tseq);
token_t* token_push(token_seq_t* tseq, YYLTYPE yylloc, char* lexeme, i32 kind, char* skind);


bool str_to_i32(char* string, i32* out);
bool str_to_f32(char* string, f32* out);
