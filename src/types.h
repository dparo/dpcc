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
    char *cstr;
    int32_t len;
} str_t;

typedef struct {
    int32_t *buf;
    isize cnt;
} int32_array_t, i32_array_t, int32_t_array_t;

typedef struct mctx {
    void **allocs;
    i32 num_allocs;
} mctx_t;

typedef struct {
    char *key;
    char *value;
} str_str_key_value_t;

typedef struct {
    i32 first_line;
    i32 first_column;
    i32 last_line;
    i32 last_column;
} loc_t;

typedef struct {
    i32 idx; /// Index withing the array containing all the tokens

    char *lexeme;
    i32 kind;
    char *skind;
    loc_t loc;
} token_t;

typedef struct {
    token_t **tokens;
    i32 tokens_cnt;
} token_seq_t;

enum DPCC_TYPE {
    TYPE_NONE,
    TYPE_BOOL,
    TYPE_I32,
    TYPE_F32,
    TYPE_I32_ARRAY,
    TYPE_F32_ARRAY,
};

typedef struct {
    enum DPCC_TYPE type;
    int32_t array_len;

    union {
        char *addr;
        struct {
            char *jmp_top;
            char *jmp_next;
            char *jmp_bot;
        };
    };
} codegen_metadata_t;

typedef struct ast_node {
    token_t *tok;

    loc_t loc;
    i32 kind;
    char *skind;

    codegen_metadata_t md;

    struct ast_node *parent;

    int32_t num_childs;
    struct ast_node **childs;

    /// Pointer to the declaration of this variable
    /// Only used on identifiers in statements, expressions, etc...
    struct ast_node *decl;

    union {
        i32 as_i32;
        f32 as_f32;
        bool as_bool;
    } val;
} ast_node_t;

typedef struct {
    int32_t stack_cnt;
    ast_node_t **stack_nodes;
    int32_t *stack_childs;
    bool down_dir;
} ast_traversal_t;

typedef struct {
    ast_node_t *node;
    int32_t num_usages;
} symtable_record_t;

typedef struct {
    int32_t num_records;
    symtable_record_t *records;
} symlist_t;

typedef struct {
    int32_t num_lists;
    symlist_t *lists;
} symtable_t;

typedef struct {
    int32_t num_temp_vars;
    int32_t num_labels;
} codegen_ctx_t;
