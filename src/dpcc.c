#include "dpcc.h"
#include "scanner.h"
#include "parser.h"


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>


FILE* open_from_string(char *string)
{
    FILE *result = fmemopen(string, strlen(string), "r");
    if (result == NULL) {
        fprintf(stderr, "Failed create FILE* stream from string\n");
        abort();
    }

    return result;
}

FILE* open_file_for_reading(char *filepath)
{
    FILE *result = fopen(filepath, "r");
    if (result == NULL) {
        fprintf(stderr, "Failed to open file for reading (%s)\n", filepath);
        abort();
    }

    return result;
}


int lex(FILE *input_stream)
{
    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::lex() --- NULL input_stream\n");
        abort();
    }
    yyin = input_stream;


    int result = 0;
    int c = 0;

    while ((c = yylex()) != YYEOF) {
        printf("Lex got: %s (id = %d, yylval = %d, yylineno: %d, yylloc=(%d, %d)\n",
               yytext,
               c,
               yylval,
               yylineno,
               yylloc.line,
               yylloc.column
            );
        if (c == YYUNDEF || c == YYerror) {
            result = c;
            break;
        }
    }

    return result;
}


int parse(FILE *input_stream)
{
    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::parse() --- NULL input_stream\n");
        abort();
    }

    yyin = input_stream;

    return yyparse() != 0;
}
