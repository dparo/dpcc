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
    extern bool yyerror_occured;
 }
%{

#include "parser.h"
#define YYERROR_VERBOSE 1

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
void yyerror(char const *s);
int  yylex(void);

YYLTYPE yylloc = {0};
bool yyerror_occured = false;
%}


%token                  ID
%token                  I32_LIT
%token                  F32_LIT



%token                  ASSIGN
%token                  PLUS


%token                  SEMICOLON
%token                  OPEN_PAREN
%token                  CLOSE_PAREN

%left                   ASSIGN
%left                   PLUS


%%

lines:          statement YYEOF
        |       statement statement
        ;

statement:
        |       ID ASSIGN expr SEMICOLON { printf("statement\n"); }
        |       SEMICOLON
                ;

expr:
        |       expr PLUS expr { printf("OP PLUS\n"); }
        |       OPEN_PAREN expr CLOSE_PAREN { printf("EXPR BEGIN\n"); }
        |       ID { printf("ID\n");}
        |       I32_LIT {printf("I32_LIT\n"); }
        |       F32_LIT {printf("F32_LIT\n"); }
                ;

%%

void yyerror (char const *s)
{
    extern int yylineno;
    fprintf(stderr, "Error at yylineno: %d, yylloc=(%d, %d)\n\t%s\n", yylineno, yylloc.line, yylloc.column, s);
    yyerror_occured = false;
}

// yywrap: return 1 to stop the parser/lexer upon encountering EOF
int yywrap(void)
{
    return 1;
}
