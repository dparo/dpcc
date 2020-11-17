%define parse.error detailed

%code requires {
    /* This code block will be exported to the generated header file by bison */
    #include "globals.h"
 }
%{

#include "utils.h"
#include "lexer.h"
#include "parser.h"
#define YYERROR_VERBOSE 1

void yyerror(char const *s);
int  yylex(void);


#define PARSER_FWD(X) \
        (void) ast_push(yylval, (isize) 0, NULL)

%}


%token                  ID
%token                  I32_LIT
%token                  F32_LIT
%token                  CHAR_LIT
%token                  STRING_LIT

%token                  ASSIGN
%token                  PLUS
%token                  MINUS
%token                  MUL

%token                  SEMICOLON
%token                  OPEN_PAREN
%token                  CLOSE_PAREN
%token                  STATEMENT


// Precedence of the operators
// From top to bottom:
// - TOP: Lowest precedence
// - Bottom: Higher precedence
// Spaces can be used to separate operators on the same line,
//  and assign the same precedence
%left                   ASSIGN
%left                   PLUS MINUS
%left                   MUL
%right                  NEG

%%

lines:           line lines
        |        YYEOF                   {      }
        ;

line:            statement
        ;

statement:      ID ASSIGN expr SEMICOLON { PARSER_FWD(STATEMENT); }
        |       SEMICOLON                { PARSER_FWD(SEMICOLON); }
        ;

expr:           expr PLUS expr { PARSER_FWD(PLUS); }
        |       expr MINUS expr { PARSER_FWD(MINUS); }
        |       expr MUL expr { PARSER_FWD(MUL); }
        |       MINUS expr  %prec NEG { PARSER_FWD(NEG); }
        |       OPEN_PAREN expr CLOSE_PAREN { PARSER_FWD(OPEN_PAREN); }
        |       ID { PARSER_FWD(ID); }
        |       I32_LIT { PARSER_FWD(I32_LIT); }
        |       F32_LIT { PARSER_FWD(F32_LIT); }
        |       CHAR_LIT { PARSER_FWD(CHAR_LIT); }
        ;

%%

void yyerror (char const *s)
{
    extern int yylineno;
    fprintf(stderr, "Error at yylineno: %d, yylloc=(%d, %d)\n\t%s\n", yylineno, yylloc.line, yylloc.column, s);
    yy_errored_out = true;
}
