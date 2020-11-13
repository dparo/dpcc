%define parse.error detailed

%code requires {
    /* This code block will be exported to the generated header file by bison */
    #include "globals.h"
 }
%{

#include "parser.h"
#define YYERROR_VERBOSE 1

#include "parser_utils.h"
void yyerror(char const *s);
int  yylex(void);



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
