#pragma once

#define ARRAY_LEN(A) (sizeof(A) / sizeof((A)[0]))


#include "types.h"

/// Equivalent to malloc
void *dallnew(mctx_t *ctx, size_t size);
/// Equivalent to realloc
void *dallrsz(mctx_t *ctx, void *ptr, size_t new_size);
/// Equivalent to free
void dalldel(mctx_t *ctx, void *ptr);
/// Clears all memory allocations
void dallclr(mctx_t *ctx);
