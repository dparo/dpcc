#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>

#include "types.h"
#include "globals.h"

#include <stdarg.h>

/// Equivalent to malloc
void *dallnew(mctx_t *ctx, size_t size)
{
    ctx->allocs = realloc(ctx->allocs, sizeof(void *) * (ctx->num_allocs + 1));
    ctx->allocs[ctx->num_allocs] = calloc(1, size);
    ctx->num_allocs += 1;
    return ctx->allocs[ctx->num_allocs - 1];
}

/// Equivalent to realloc
void *dallrsz(mctx_t *ctx, void *ptr, size_t new_size)
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
void dalldel(mctx_t *ctx, void *ptr)
{
    if (!ptr)
        return;

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
void dallclr(mctx_t *ctx)
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

void *dallstl(mctx_t *ctx, void *ptr)
{
    ctx->allocs = realloc(ctx->allocs, sizeof(void *) * (ctx->num_allocs + 1));
    ctx->allocs[ctx->num_allocs] = ptr;
    ctx->num_allocs += 1;
    return ctx->allocs[ctx->num_allocs - 1];
}

bool str_to_i32(char *string, i32 *out)
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

    char *endptr = NULL;
    errno = 0;
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

bool str_to_f32(char *string, f32 *out)
{
    size_t len = strlen(string);

    char *endptr = NULL;

    errno = 0;
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
    dalldel(&G_allctx, tseq->tokens);
    tseq->tokens = NULL;
    tseq->tokens_cnt = 0;
}

void ast_clear(ast_t *ast)
{
    dalldel(&G_allctx, ast->nodes);
    ast->nodes = NULL;
    ast->nodes_cnt = 0;
}

char *lexeme_intern(char *yytext)
{
    char *intern = NULL;
    ptrdiff_t index = shgeti(G_str_intern, yytext);
    if (index < 0) {
        char *key = dallstl(&G_allctx, strdup(yytext));
        char *value = key;
        shput(G_str_intern, key, value);
        intern = value;
    } else {
        intern = G_str_intern[index].value;
    }

    return intern;
}

token_t *token_push(tokloc_t loc, char *yytext, int yychar, char *yychar_str)
{
    token_seq_t *tseq = &G_tok_seq;

    token_t token = {
        .lexeme = (char *)lexeme_intern(yytext),
        .kind = yychar,
        .skind = yychar_str,
        .loc = loc,
    };

    token.idx = G_tok_seq.tokens_cnt;

    void *ptr = dallrsz(&G_allctx, tseq->tokens, (tseq->tokens_cnt + 1) * sizeof(*tseq->tokens));

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

ast_node_t *ast_push(token_t *t, int32_t kind, char *skind, isize num_childs, ast_node_t **childs)
{
    ast_t *ast = &G_ast;

    ast_node_t node = {
        .tok = t,
        .kind = kind,
        .skind = skind,
    };

    void *ptr = dallrsz(&G_allctx, ast->nodes, (ast->nodes_cnt + 1) * sizeof(*ast->nodes));

    if (ptr == NULL) {
        fprintf(stderr, "ast_push :: Failed memory allocation\n");
        fflush(stderr);
        abort();
    }

    for (isize i = 0; i < num_childs; i++) {
        arrpush(node.childs, childs[i]);
    }

    ast->nodes = ptr;
    ast->nodes_cnt += 1;

    memcpy(&ast->nodes[ast->nodes_cnt - 1], &node, sizeof(node));
    return &ast->nodes[ast->nodes_cnt - 1];
}

bool str_to_char(char *string, char *out)
{
    size_t len = strlen(string);
    bool is_escaped = len >= 1 && string[0] == '\\';

    if (len == 1) {
        *out = string[0];
        return true;
    } else if (is_escaped && len == 2) {
        char c = string[1];
        char allowed_chars[] = {
            '\0', '\a', '\b', '\t', '\n', '\v', '\f', '\r'
        };
        for (usize i = 0; i < ARRAY_LEN(allowed_chars); i++) {
            if (c == allowed_chars[i]) {
                *out = allowed_chars[i];
                return true;
            }
        }
    } else if (is_escaped && len == 5 && string[1] == '0' && string[2] == 'x') {
        int32_t i;
        bool conversion_success = str_to_i32(string + 1, &i);
        bool in_range = i < 255 && i >= 0;
        if (!in_range || !conversion_success) {
            return false;
        }
        *out = (char)i;
        return true;
    } else {
        return false;
    }
    return false;
}

bool str_to_bool(char *string, bool *out)
{
    if (0 == strcmp(string, "true")) {
        *out = true;
        return true;
    } else if (0 == strcmp(string, "false")) {
        *out = false;
        return true;
    } else {
        return false;
    }
}

/// String format concat
char *sfcat(char *string, int32_t string_len, char *fmt, ...)
{
    if (string == NULL || string[0] == '\0') {
        string = calloc(1, 1);
        string_len = 0;
    } else if (string_len == 0) {
        string_len = strlen(string);
    }

    va_list ap;

    va_start(ap, fmt);
    int32_t chars_to_write = vsnprintf(NULL, 0, fmt, ap);
    char *result = realloc(string, string_len + chars_to_write + 1);
    result[string_len + chars_to_write] = 0;

    char *bumped = string + string_len;
    va_start(ap, fmt);
    vsnprintf(bumped, chars_to_write, fmt, ap);
    va_end(ap);
    return result;
}
