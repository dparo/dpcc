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

FILE *open_file_for_reading(char *filepath);
FILE *open_from_string(char *string);

void ast_traversal_begin(ast_traversal_t *t);
ast_node_t *ast_traverse_next(ast_traversal_t *t);

void dpcc_reset(void);

bool lex(char *filepath, FILE *input_stream);
bool parse(char *filepath, FILE *input_stream);
bool compile(char *filepath, FILE *input_stream, FILE *output_stream);

static inline bool lex_string(char *string)
{
    return lex("", open_from_string(string));
}

static inline bool parse_string(char *string)
{
    return parse("", open_from_string(string));
}
