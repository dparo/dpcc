#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "globals.h"
#include "utils.h"

#include <stdarg.h>
#include "log.h"

FILE *open_file_for_reading(char *filepath);
FILE *open_from_string(char *string);

bool lex(FILE *input_stream);
bool parse(FILE *input_stream);
bool compile(FILE *input_stream, FILE *output_stream);

static inline bool lex_string(char *string)
{
    return lex(open_from_string(string));
}

static inline bool parse_string(char *string)
{
    return parse(open_from_string(string));
}
