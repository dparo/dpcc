#include "dpcc.h"
#include "scanner.h"
#include "parser.h"


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

FILE* open_file_for_reading(char *filepath)
{
    FILE *result = fopen(filepath, "r");
    if (result == NULL) {
        fprintf(stderr, "Failed to open file for reading (%s)\n", filepath);
    }

    return result;
}


void lex(FILE *input_stream)
{
    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::lex() --- NULL input_stream\n");
        abort();
    }
    yyin = input_stream;

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
    }
}


void parse(FILE *input_stream)
{
    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::parse() --- NULL input_stream\n");
    }

    yyin = input_stream;

    yyparse();
}
