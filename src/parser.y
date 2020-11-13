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
    extern int32_t yyprevcol;

    extern char *yydebugretval;
 }
%{

#include "parser.h"
#define YYERROR_VERBOSE 1

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
void yyerror(char const *s);
int  yylex(void);

char *yydebugretval = NULL;
int32_t yyprevcol = 0;
YYLTYPE yylloc = {1, 0};
bool yyerror_occured = false;

#define PARSER_FWD(X) \
    do { return (X); } while(0)

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

%left                   ASSIGN
%left                   PLUS
%left                   MUL

%%

lines:          statement YYEOF
        |       statement statement
        |       debug_temporary_delete_me
        ;

debug_temporary_delete_me:
                leaf
        ;

leaf:           ID      { PARSER_FWD(ID); }
        |       I32_LIT { PARSER_FWD(I32_LIT); }
        |       F32_LIT { PARSER_FWD(F32_LIT); }
        ;

statement:
        |       ID ASSIGN expr SEMICOLON {  }
        |       SEMICOLON
                ;

expr:
        |       expr PLUS expr {  }
        |       expr MUL expr {  }
        |       OPEN_PAREN expr CLOSE_PAREN {  }
        |       ID {  }
        |       I32_LIT {  }
        |       F32_LIT {  }
                ;

%%

void yyerror (char const *s)
{
    extern int yylineno;
    fprintf(stderr, "Error at yylineno: %d, yylloc=(%d, %d)\n\t%s\n", yylineno, yylloc.line, yylloc.column, s);
    yyerror_occured = true;
}

// yywrap: return 1 to stop the parser/lexer upon encountering EOF
int yywrap(void)
{
    return 1;
}
