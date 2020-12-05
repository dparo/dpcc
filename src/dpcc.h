#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "types.h"
#include "globals.h"
#include "utils.h"
#include "log.h"

void symtable_clear(void);
ast_node_t *symtable_lookup(token_t *tok);
void symtable_begin_block(void);
void symtable_end_block(void);
ast_node_t *symtable_push_sym(ast_node_t *sym_var_decl);

FILE *open_file_for_reading(char *filepath);
FILE *open_from_string(char *string);

void ast_traversal_begin(ast_traversal_t *t, ast_node_t *root);
ast_node_t *ast_traverse_next(ast_traversal_t *t, int32_t *match_idx);

/// Mostly used for internal reasons and for the code generation pass
void ast_traversal_push(ast_traversal_t *t, ast_node_t *parent, int32_t current_child);
bool ast_traversal_pop(ast_traversal_t *t);

void dpcc_reset(void);

bool lex(char *filepath, FILE *input_stream);
bool parse(char *filepath, FILE *input_stream);
char *threeac_gen(char *filepath, FILE *input_stream);
bool compile(char *filepath, FILE *input_stream, FILE *output_stream);
bool run(char *filepath, FILE *input_stream);

static inline bool lex_string(char *string)
{
    return lex("", open_from_string(string));
}

static inline bool parse_string(char *string)
{
    return parse("", open_from_string(string));
}
