#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>

#include "types.h"
#include "globals.h"

void invalid_code_path(void)
{
    fprintf(stderr, "COMPILER INTERNAL ERROR --- Invalid code path\n");
    fflush(stderr);
    assert(0);
}

char *dpcc_type_as_str(enum DPCC_TYPE t)
{
    switch (t) {
    default: {
        invalid_code_path();
    } break;
    case TYPE_NONE: {
        return "<NONE>";
    } break;
    case TYPE_STR: {
        return "char *";
    } break;
    case TYPE_I32: {
        return "int";
    } break;
    case TYPE_F32: {
        return "float";
    } break;
    case TYPE_BOOL_ARRAY: {
        return "bool[]";
    } break;
    case TYPE_BOOL: {
        return "bool";
    } break;
    case TYPE_I32_ARRAY: {
        return "int[]";
    } break;
    case TYPE_F32_ARRAY: {
        return "float[]";
    } break;
    }
    return NULL;
}

char *dpcc_type_as_enum_str(enum DPCC_TYPE t)
{
    switch (t) {
    default: {
        invalid_code_path();
    } break;
    case TYPE_NONE: {
        return "TYPE_NONE";
    } break;
    case TYPE_I32: {
        return "TYPE_I32";
    } break;
    case TYPE_F32: {
        return "TYPE_F32";
    } break;
    case TYPE_BOOL: {
        return "TYPE_BOOL";
    } break;
    case TYPE_I32_ARRAY: {
        return "TYPE_I32_ARRAY";
    } break;
    case TYPE_F32_ARRAY: {
        return "TYPE_F32_ARRAY";
    } break;
    }
    return NULL;
}

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

bool dallarr(mctx_t *ctx, void **ptr, size_t num_elems, size_t sizeof_each_elem)
{
    void *temp = dallrsz(ctx, *ptr, num_elems * sizeof_each_elem);
    if (temp != NULL) {
        *ptr = temp;
        return true;
    }
    return false;
}

char *dallfmtv(mctx_t *ctx, char *fmt, va_list ap)
{
    va_list inner0;
    va_list inner1;
    va_copy(inner0, ap);
    va_copy(inner1, ap);

    int32_t chars_to_write = vsnprintf(NULL, 0, fmt, ap);
    va_end(inner0);

    char *result = dallnew(ctx, chars_to_write + 2);
    result[chars_to_write] = 0;
    result[chars_to_write + 1] = 0;

    vsnprintf(result, chars_to_write + 1, fmt, ap);
    va_end(inner1);
    return result;
}

char *dallfmt(mctx_t *ctx, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char *result = dallfmtv(ctx, fmt, ap);
    va_end(ap);
    return result;
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

    bool terminating_f = len >= 1 && (string[len - 1] == 'f' || string[len - 1] == 'F');

    char *endptr = NULL;

    errno = 0;
    f32 conv_ret_val = strtof(string, &endptr);

    bool out_of_range = (errno == ERANGE);
    bool failed = len == 0 || out_of_range || endptr == string || endptr == NULL;

    bool valid_endptr = *endptr == '\0' || (terminating_f && endptr == string + len - 1);
    bool result = !failed && valid_endptr;

    if (result) {
        *out = conv_ret_val;
    } else {
        *out = NAN;
    }

    return result;
}

char *string_intern(char *yytext)
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

void tokens_seq_clear(token_seq_t *tseq)
{
    dalldel(&G_allctx, tseq->tokens);
    tseq->tokens = NULL;
    tseq->tokens_cnt = 0;
}

token_t *token_new(loc_t loc, char *yytext, int yychar, char *yychar_str)
{
    token_t *result = dallnew(&G_allctx, sizeof(*result));
    if (result == NULL) {
        fprintf(stderr, "ast_push :: Failed memory allocation\n");
        fflush(stderr);
        abort();
    }

    result->lexeme = string_intern(yytext);
    result->kind = yychar;
    result->skind = string_intern(yychar_str);
    result->loc = loc;

    G_tok_seq.tokens = dallrsz(&G_allctx, G_tok_seq.tokens, (G_tok_seq.tokens_cnt + 1) * sizeof(token_t *));
    G_tok_seq.tokens[G_tok_seq.tokens_cnt++] = result;

    result->idx = G_tok_seq.tokens_cnt - 1;

    return result;
}

void dbg_print_node(FILE *f, ast_node_t *node, int32_t indentation_level)
{
    assert(node);

    str_t hdr_str = { 0 };
    for (int32_t i = 0; i < indentation_level; i++) {
        sfcat(&G_allctx, &hdr_str, "  |");
    }

    sfcat(&G_allctx, &hdr_str, "%s%s", indentation_level != 0 ? "--" : "", node->skind);

    fprintf(f,
        " %-48s {\"%s\", md.type: %s, md.sym: %s, md.array_len: %d, tok->loc=[[%d,%d], [%d,%d]] }\n",
        hdr_str.cstr,
        node->tok ? node->tok->lexeme : "",
        dpcc_type_as_str(node->md.type),
        node->md.sym,
        node->md.array_len,
        node->tok ? node->tok->loc.first_line : 0,
        node->tok ? node->tok->loc.first_column : 0,
        node->tok ? node->tok->loc.last_line : 0,
        node->tok ? node->tok->loc.last_column : 0);
    dalldel(&G_allctx, hdr_str.cstr);
}

void print_node(FILE *f, ast_node_t *node, int32_t indentation_level)
{
    assert(node);

    str_t hdr_str = { 0 };
    for (int32_t i = 0; i < indentation_level; i++) {
        sfcat(&G_allctx, &hdr_str, "  |");
    }

    sfcat(&G_allctx, &hdr_str, "%s%s", indentation_level != 0 ? "--" : "", node->skind);

    fprintf(f,
        " %-48s {%d, \"%s\", tok->loc=[[%d,%d], [%d,%d]] }\n",
        hdr_str.cstr,
        node->kind,
        node->tok ? node->tok->lexeme : "",
        node->tok ? node->tok->loc.first_line : 0,
        node->tok ? node->tok->loc.first_column : 0,
        node->tok ? node->tok->loc.last_line : 0,
        node->tok ? node->tok->loc.last_column : 0);
    dalldel(&G_allctx, hdr_str.cstr);
}

void print_token(FILE *f, token_t *t)
{
    str_t hdr_str = { 0 };
    sfcat(&G_allctx, &hdr_str, "%-8d (%s)", t->idx, t->skind);
    fprintf(f, "%-48s { lexeme = '%s', yylloc = [[%d, %d], [%d, %d]] }\n",
        hdr_str.cstr,
        t->lexeme,
        t->loc.first_line, t->loc.first_column,
        t->loc.last_line, t->loc.last_column);

    dalldel(&G_allctx, hdr_str.cstr);
}

ast_node_t *new_node(token_t *t, int32_t kind, char *skind)
{
    ast_node_t *result = result = dallnew(&G_allctx, sizeof(*result));

    if (result) {
        result->tok = t;
        node_set_kind(result, kind, skind);
    }

    return result;
}

void node_set_kind(ast_node_t *node, int32_t kind, char *skind)
{
    node->kind = kind;
    node->skind = string_intern(skind);
}

void push_child(ast_node_t *parent, ast_node_t *child)
{
    assert(parent);

    dallarr(&G_allctx, (void **)&parent->childs, parent->num_childs + 1, sizeof(child));
    parent->childs[parent->num_childs++] = child;
    if (child) {
        child->parent = parent;
    }
}

void push_childs(ast_node_t *parent, int32_t num_childs, ast_node_t **childs)
{
    dallarr(&G_allctx, (void **)&parent->childs, parent->num_childs + num_childs, sizeof(childs[0]));

    for (int32_t i = 0; i < num_childs; i++) {
        parent->childs[parent->num_childs++] = childs[i];
        if (childs[i]) {
            childs[i]->parent = parent;
        }
    }
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
void sfcat(mctx_t *ctx, str_t *s, char *fmt, ...)
{
    assert(s);

    va_list ap;
    va_start(ap, fmt);
    int32_t chars_to_write = vsnprintf(NULL, 0, fmt, ap);
    s->cstr = dallrsz(ctx, s->cstr, s->len + chars_to_write + 1);
    s->cstr[s->len + chars_to_write] = 0;

    char *bumped = s->cstr + s->len;
    va_start(ap, fmt);
    vsnprintf(bumped, chars_to_write + 1, fmt, ap);
    va_end(ap);

    s->len += chars_to_write;
}
void sfcatv(mctx_t *ctx, str_t *s, char *fmt, va_list ap)
{
    va_list cp;
    va_copy(cp, ap);
    int32_t chars_to_write = vsnprintf(NULL, 0, fmt, ap);
    s->cstr = dallrsz(ctx, s->cstr, s->len + chars_to_write + 1);
    s->cstr[s->len + chars_to_write] = 0;

    char *bumped = s->cstr + s->len;
    vsnprintf(bumped, chars_to_write + 1, fmt, cp);
    va_end(cp);

    s->len += chars_to_write;
}

char *get_current_working_dir(void)
{
    size_t s = 4096;
    char *current_dir = malloc(s);
    char *success = NULL;

    while ((success = getcwd(current_dir, s)) == NULL) {
        s += 4096;
        current_dir = realloc(current_dir, s);
    }

    return current_dir;
}
