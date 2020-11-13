#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>



FILE* open_file_for_reading(char *filepath);

FILE* open_from_string(char *string);


int lex_once(FILE *input_stream);
int lex(FILE *input_stream);

int parse_once(FILE *input_stream);
int parse(FILE *input_stream);
