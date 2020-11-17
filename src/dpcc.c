#include "dpcc.h"
#include "globals.h"
#include "lexer.h"
#include "parser.h"
#include "utils.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FILE *open_from_string(char *string)
{
    FILE *result = fmemopen(string, strlen(string), "r");
    if (result == NULL) {
        fprintf(stderr, "Failed create FILE* stream from string\n");
        abort();
    }

    return result;
}

FILE *open_file_for_reading(char *filepath)
{
    FILE *result = fopen(filepath, "r");
    if (result == NULL) {
        fprintf(stderr, "Failed to open file for reading (%s)\n", filepath);
        abort();
    }

    return result;
}

static void reset(void)
{
    yylex_destroy();
    clear_all_global_vars();
}

bool lex(FILE *input_stream)
{
    reset();

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::lex() --- NULL input_stream\n");
        abort();
    }

#if 1
    yyrestart(input_stream);
#else
    yyin = input_stream;
#endif

    int result = 0;
    int kind = 0;
    while ((kind = yylex()) != YYEOF) {
        printf("lex kind returned %d for string (%s)\n", kind, yytext);
        if (kind == YYUNDEF || kind == YYerror) {
            result = kind;
            break;
        }
    }

    return result == 0;
}

bool parse(FILE *input_stream)
{
    reset();

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::parse() --- NULL input_stream\n");
        abort();
    }

    yyin = input_stream;

    int result = yyparse();

    if (result == 0) {
        assert(yy_errored_out == false);
    } else if (result == 1) {
        assert(yy_errored_out == true);
    }

    return result == 0;
}
