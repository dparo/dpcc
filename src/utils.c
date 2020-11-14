#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>

#include "types.h"
#include "globals.h"


/// Equivalent to malloc
void* dallnew(mctx_t* ctx, size_t size)
{
    ctx->allocs = realloc(ctx->allocs, sizeof(void*) * (ctx->num_allocs + 1));
    ctx->allocs[ctx->num_allocs] = calloc(1, size);
    ctx->num_allocs += 1;
    return ctx->allocs[ctx->num_allocs - 1];
}

/// Equivalent to realloc
void* dallrsz(mctx_t* ctx, void* ptr, size_t new_size)
{
    i32 alloc_idx = -1;
    for (i32 i = 0; i < ctx->num_allocs; i++) {
        if (ctx->allocs[i] == ptr) {
            alloc_idx = i;
            break;
        }
    }

    if (alloc_idx == -1) {
        return dallnew(ctx, new_size);
    }

    ptr = realloc(ptr, new_size);
    if (ptr != NULL) {
        ctx->allocs[alloc_idx] = ptr;
    }
    return ptr;
}

/// Equivalent to free
void dalldel(mctx_t* ctx, void* ptr)
{
    if (!ptr) return;

    i32 alloc_idx = -1;
    for (i32 i = 0; i < ctx->num_allocs; i++) {
        if (ctx->allocs[i] == ptr) {
            alloc_idx = i;
            break;
        }
    }

    if (alloc_idx == -1) {
        return;
    }

    free(ptr);
    ctx->allocs[alloc_idx] = ctx->allocs[ctx->num_allocs - 1];
    ctx->num_allocs -= 1;
}

/// Clears all memory allocations
void dallclr(mctx_t* ctx)
{
    if (ctx->allocs) {
        for (i32 i = 0; i < ctx->num_allocs; i++) {
            free(ctx->allocs[i]);
        }

        free(ctx->allocs);
        ctx->allocs = NULL;
        ctx->num_allocs = 0;
    }
}


void* dallstl(mctx_t *ctx, void *ptr)
{
    ctx->allocs = realloc(ctx->allocs, sizeof(void*) * (ctx->num_allocs + 1));
    ctx->allocs[ctx->num_allocs] = ptr;
    ctx->num_allocs += 1;
    return ctx->allocs[ctx->num_allocs - 1];

}








bool str_to_i32(char* string, i32* out)
{
    size_t len = strlen(string);
    int base = 10;

    bool is_negated = false;
    if (len >= 1 && (string[0] == '-' || string[0] == '+')) {
        is_negated = string[0] == '-';

        len -= 1;
        string += 1;
    }

    if (len >= 2 && string[0] == '0' && string[1] == 'x') {
        base = 16;
        string += 2;
        len -= 2;
    } else if (len >= 2 && string[0] == '0' && string[1] == 'b') {
        base = 2;
        string += 2;
        len -= 2;
    }

    char* endptr = NULL;
    intmax_t conv_ret_val = strtoimax(string, &endptr, base);

    bool out_of_range = (errno == ERANGE);
    bool failed = len == 0 || out_of_range || endptr == string || endptr == NULL;
    bool result = !failed && *endptr == '\0';

    if (result) {
        *out = (is_negated ? (u32)-1 : (u32)1) * (u32)conv_ret_val;
    } else {
        *out = INT32_MIN;
    }

    return result;
}

bool str_to_f32(char* string, f32* out)
{
    size_t len = strlen(string);

    char* endptr = NULL;
    f32 conv_ret_val = strtof(string, &endptr);

    bool out_of_range = (errno == ERANGE);
    bool failed = len == 0 || out_of_range || endptr == string || endptr == NULL;
    bool result = !failed && *endptr == '\0';

    if (result) {
        *out = conv_ret_val;
    } else {
        *out = INT32_MIN;
    }

    return result;
}



void tokens_seq_clear(token_seq_t *tseq)
{
    dalldel(tseq->mctx, tseq->tokens);
    tseq->tokens = NULL;
    tseq->tokens_cnt = 0;
}


void ast_clear(ast_t *ast)
{
    dalldel(ast->mctx, ast->nodes);
    ast->nodes = NULL;
    ast->nodes_cnt = 0;
}


token_t* token_push(token_seq_t* tseq, YYLTYPE yylloc, char* lexeme, i32 kind, char* skind)
{
    tseq->mctx = &G_allctx;

    token_t token = {
        .lexeme = (char*) dallstl(tseq->mctx, strdup(lexeme)),
        .kind = kind,
        .skind = skind,
        .yylloc = yylloc,
    };

    void* ptr = dallrsz(tseq->mctx, tseq->tokens, (tseq->tokens_cnt + 1) * sizeof(*tseq->tokens));

    if (ptr == NULL) {
        fprintf(stderr, "ast_push :: Failed memory allocation\n");
        fflush(stderr);
        abort();
    }

    tseq->tokens = ptr;
    tseq->tokens_cnt += 1;

    memcpy(&tseq->tokens[tseq->tokens_cnt - 1], &token, sizeof(token));
    return &tseq->tokens[tseq->tokens_cnt - 1];

}


ast_node_t *ast_push(ast_t *ast, YYLTYPE yylloc, char *lexeme, i32 kind, char *skind)
{
    ast->mctx = &G_allctx;

    ast_node_t node = {
        .tok.lexeme = (char*) dallstl(ast->mctx, strdup(lexeme)),
        .tok.kind = kind,
        .tok.skind = skind,
        .tok.yylloc = yylloc,
    };

    void* ptr = dallrsz(ast->mctx, ast->nodes, (ast->nodes_cnt + 1) * sizeof(*ast->nodes));

    if (ptr == NULL) {
        fprintf(stderr, "ast_push :: Failed memory allocation\n");
        fflush(stderr);
        abort();
    }

    ast->nodes = ptr;
    ast->nodes_cnt += 1;

    memcpy(&ast->nodes[ast->nodes_cnt - 1], &node, sizeof(node));
    return &ast->nodes[ast->nodes_cnt - 1];
}
