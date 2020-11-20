#pragma once

#include <stddef.h>
#include <stdio.h>
#include "types.h"
#include <stb_ds.h>

#define ARGS(...) __VA_ARGS__
#define ARRAY_LEN_SIMPLE(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#define GNUC_VERSION \
    (__GNUC__ << 16) + __GNUC_MINOR__
#define GNUC_PREREQ(maj, min) \
    (GNUC_VERSION >= ((maj) << 16) + (min))
#else
#define GNUC_PREREQ(maj, min) 0
#endif

#define BUILD_BUG_ON_ZERO(e) \
    (sizeof(struct { int : -!!(e)*1234; }))

#if GNUC_PREREQ(3, 1)
#define SAME_TYPE(a, b) \
    __builtin_types_compatible_p(typeof(a), typeof(b))
#define MUST_BE_ARRAY(a) \
    BUILD_BUG_ON_ZERO(SAME_TYPE((a), &(*a)))
#else
#define MUST_BE_ARRAY(a) \
    BUILD_BUG_ON_ZERO(sizeof(a) % sizeof(*a))
#endif

#ifdef __cplusplus
template <typename T, size_t N>
char (&ARRAY_LEN_HELPER(T (&array)[N]))[N];
#define ARRAY_LEN(array) \
    (sizeof(ARRAY_LEN_HELPER(array)))
#else
#define ARRAY_LEN(a) (       \
    (sizeof(a) / sizeof(*a)) \
    + MUST_BE_ARRAY(a))
#endif

/// @Example:
/// ```c
/// typedef struct { int32_t *buf, isize cnt; } int32_array_t;
/// ...
/// int32_array_t a = ARRAY_LIT(int32_t, {5, 6, 7});
/// a.buf[0] -> 5
/// a.cnt -> 3
/// ```
#define ARRAY_LIT(TYPE, ...)                                \
    (TYPE##_array_t)                                        \
    {                                                       \
        ((int32_t[])__VA_ARGS__),                           \
            ARRAY_LEN_SIMPLE((int32_t[])ARGS(__VA_ARGS__)), \
    }

/// String format concat
char *sfcat(char *string, int32_t string_len, char *fmt, ...)
    __attribute__((format(printf, 3, 4)));

/// Equivalent to malloc
void *dallnew(mctx_t *ctx, size_t size);
/// Equivalent to realloc
void *dallrsz(mctx_t *ctx, void *ptr, size_t new_size);
/// Equivalent to free
void dalldel(mctx_t *ctx, void *ptr);
/// Clears all memory allocations
void dallclr(mctx_t *ctx);
/// Alloc, realloc array to ensure the correct number of elements
bool dallarr(mctx_t *ctx, void **ptr, size_t num_elems, size_t sizeof_each_elem);

/// Steal memory allocations from typical malloc
void *dallstl(mctx_t *ctx, void *ptr);

/// Used for string interning
char *string_intern(char *yytext);

void tokens_seq_clear(token_seq_t *tseq);
token_t *token_new(tokloc_t loc, char *yytext, int yychar, char *yychar_str);

void print_token(FILE *f, token_t *t);
void print_node(FILE *f, ast_node_t *node, int32_t indetation_level);

ast_node_t *new_node(token_t *t, int32_t kind, char *skind);
void node_set_kind(ast_node_t *node, int32_t kind, char *skind);

void push_child(ast_node_t *parent, ast_node_t *child);
void push_childs(ast_node_t *parent, int32_t num_childs, ast_node_t **childs);

bool str_to_i32(char *string, i32 *out);
bool str_to_f32(char *string, f32 *out);
bool str_to_char(char *string, char *out);
bool str_to_bool(char *string, bool *out);

static inline int breakme(void)
{
    return 1;
}
