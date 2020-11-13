#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dpcc.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char** argv)
{
    char* filepath = "in/input.txt";
    char* mode = "compile";

    if (argc >= 2) {
        if (argc >= 3) {
            mode = argv[1];
            filepath = argv[2];
        } else {
            filepath = argv[1];
        }
    }

    FILE* input_stream = open_file_for_reading(filepath);

    if (0 == strcmp(mode, "lex")) {
        return lex(input_stream);
    } else if (0 == strcmp(mode, "parse")) {
        return parse(input_stream);
    } else {
        return parse(input_stream);
    }

    fprintf(stderr, "Invalid code path\n");
    fflush(stderr);
    return -1;
}
