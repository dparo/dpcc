#include "dpcc.h"
#include "lexer.h"
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

static void lexer_reset(void)
{
    yyprevcol = 0;
    yylloc.line = 1;
    yylloc.column = 0;
    yylex_destroy();
}

int lex_once(FILE *input_stream)
{
    lexer_reset();

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::lex_once() --- NULL input_stream\n");
        abort();
    }

    yyin = input_stream;
    int retval = yylex();
    return retval;
}

int lex(FILE *input_stream)
{

    lexer_reset();

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::lex() --- NULL input_stream\n");
        abort();
    }
    yyin = input_stream;


    int result = 0;
    int c = 0;

    while ((c = yylex()) != YYEOF) {
        printf("Lex got: {%s}\t\t[retval = %s, yylval = %d, yylineno: %d, yylloc=(%d, %d)]\n",
               yytext,
               yydebugretval,
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


static void parser_reset(void)
{
    yyerror_occured = false;
}

int parse_once(FILE *input_stream)
{
    parser_reset();

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::parse_once() --- NULL input_stream\n");
        abort();
    }

    yyin = input_stream;

    return yyparse() != 0;
}

int parse(FILE *input_stream)
{
    parser_reset();

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::parse() --- NULL input_stream\n");
        abort();
    }

    yyin = input_stream;


    int result = 0;
    int c = 0;

    while ((c = yyparse()) != YYEOF) {
        if (c == YYerror || yyerror_occured) {
            c = YYerror;
            result = -1;
            break;
        }
    }

    return result;
}
