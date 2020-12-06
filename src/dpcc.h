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

FILE *open_from_string(char *string);

void ast_traversal_begin(ast_traversal_t *t, ast_node_t *root);
ast_node_t *ast_traverse_next(ast_traversal_t *t, int32_t *match_idx);

/// Mostly used for internal reasons and for the code generation pass
void ast_traversal_push(ast_traversal_t *t, ast_node_t *parent, int32_t current_child);
bool ast_traversal_pop(ast_traversal_t *t);

void dpcc_reset(void);

FILE *dpcc_xfopen_w(char *filepath);

bool dpcc_lex(char *filepath, FILE *input_stream);
bool dpcc_parse(char *filepath, FILE *input_stream);
char *dpcc_3ac(char *filepath, FILE *input_stream);
bool dpcc_c(char *filepath, FILE *input_stream, char *output_filepath);
bool dpcc_gcc(char *filepath, FILE *input_stream, char *output_binary_path);
bool dpcc_run(char *filepath, FILE *input_stream);
