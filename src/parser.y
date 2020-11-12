%define parse.error detailed

%code requires {
    /* This code block will be exported to the generated header file by bison */
    #include <stddef.h>
    #include <stdint.h>

    typedef struct YYLTYPE
    {
        int32_t line;
        int32_t column;
    } YYLTYPE;

    extern YYLTYPE yylloc;
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

%}


%token                  ID
%token                  ASSIGN
%token                  PLUS
%token                  NUMBER

%token                  OPEN_PAREN
%token                  CLOSE_PAREN

%left                   ASSIGN
%left                   PLUS

%%


statement:
                ID ASSIGN expr ";" { printf("statement\n"); }
                YYEOF
                ;

expr:
        |       expr PLUS expr { printf("OP PLUS\n"); }
        |       OPEN_PAREN expr CLOSE_PAREN { printf("EXPR BEGIN\n"); }
        |       ID { printf("ID\n");}
        |       NUMBER {printf("NUMBER\n"); }
                ;

%%

void yyerror (char const *s)
{
    extern int yylineno;
    fprintf(stderr, "Error at yylineno: %d, yylloc=(%d, %d)\n\t%s\n", yylineno, yylloc.line, yylloc.column, s);
}

// yywrap: return 1 to stop the parser/lexer upon encountering EOF
int yywrap(void)
{
    return 1;
}
