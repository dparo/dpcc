#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>


FILE* open_file_for_reading(char *filepath);
void lex(FILE *input_stream);
void parse(FILE *input_stream);
