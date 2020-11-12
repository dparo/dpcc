#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "scanner.h"
#include "parser.h"
#include "dpcc.h"
#include <string.h>
// #include "scanner.c"

int main(int argc, char **argv)
{
    char *filepath = "in/input.txt";
    char *mode = "compile";


    if (argc >= 2) {
        if (argc >= 3) {
            mode = argv[1];
            filepath = argv[2];
        } else {
            filepath = argv[1];
        }
    }


    FILE *input_stream = open_file_for_reading(filepath);

    if (0 == strcmp(mode, "lex")) {
        lex(input_stream);
    } else if (0 == strcmp(mode, "parse")) {
        parse(input_stream);
    } else {
        parse(input_stream);
    }

    return 0;
}
