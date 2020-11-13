#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

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
