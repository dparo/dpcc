%define parse.error detailed

%code requires {
    /* This code block will be exported to the generated header file by bison */
    #include <stddef.h>
    #include <stdint.h>
    #include <stdbool.h>

    typedef struct YYLTYPE
    {
        int32_t line;
        int32_t column;
    } YYLTYPE;

    extern YYLTYPE yylloc;
    extern int32_t yyprevcol;
    extern char*   yylex_debug_ret_val;

    extern bool  yybis_error_occured;
    extern char* yybis_debug_ret_val;

 }
%{

#include "parser.h"
#define YYERROR_VERBOSE 1

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
void yyerror(char const *s);
int  yylex(void);

char *yylex_debug_ret_val = NULL;
int32_t yyprevcol = 0;
YYLTYPE yylloc = {1, 0};

bool  yybis_error_occured = false;
char *yybis_debug_ret_val = NULL;

#define PARSER_FWD(X) \
    do { \
        yybis_debug_ret_val = (#X); \
        printf("BISON: Got %s [id = %d]\n", (#X), (X)); \
    } while(0)

%}


%token                  ID
%token                  I32_LIT
%token                  F32_LIT

%token                  ASSIGN
%token                  PLUS
%token                  MUL


%token                  SEMICOLON
%token                  OPEN_PAREN
%token                  CLOSE_PAREN
%token                  STATEMENT

%left                   ASSIGN
%left                   PLUS
%left                   MUL

%%

lines:           line lines
        |        YYEOF                   { PARSER_FWD(YYEOF); }
        ;

line:            statement
        ;

statement:      ID ASSIGN expr SEMICOLON { PARSER_FWD(STATEMENT); }
        |       SEMICOLON                { PARSER_FWD(STATEMENT); }
        ;

expr:           expr PLUS expr { PARSER_FWD(PLUS); }
        |       expr MUL expr { PARSER_FWD(MUL); }
        |       OPEN_PAREN expr CLOSE_PAREN { PARSER_FWD(OPEN_PAREN); }
        |       ID { PARSER_FWD(ID); }
        |       I32_LIT { PARSER_FWD(I32_LIT); }
        |       F32_LIT { PARSER_FWD(F32_LIT); }
        ;

%%

void yyerror (char const *s)
{
    extern int yylineno;
    fprintf(stderr, "Error at yylineno: %d, yylloc=(%d, %d)\n\t%s\n", yylineno, yylloc.line, yylloc.column, s);
    yybis_error_occured = true;
}

// yywrap: return 1 to stop the parser/lexer upon encountering EOF
int yywrap(void)
{
    return 1;
}
