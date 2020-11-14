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
        (void)ast_push(yylloc, yytext, yykind, yyskind)

#define PARSER_FWD_I32(X, VAL) \
        (void)ast_push(yylloc, yytext, yykind, yyskind)->val.i = (VAL)

#define PARSER_FWD_F32(X, VAL) \
        (void)ast_push(yylloc, yytext, yykind, yyskind)->val.f = (VAL)

#define PARSER_FWD_BOOL(X, VAL) \
        (void)ast_push(yylloc, yytext, yykind, yyskind)->val.f = (VAL)


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
        |        YYEOF                   {      }
        ;

line:            statement
        ;

statement:      ID ASSIGN expr SEMICOLON { printf("# debug(STATEMENT): yytext: %s, yylval: %d\n", yytext, yylval); PARSER_FWD(STATEMENT); }
        |       SEMICOLON                { PARSER_FWD(SEMICOLON); }
        ;

expr:           expr PLUS expr { PARSER_FWD(PLUS); }
        |       expr MUL expr { PARSER_FWD(MUL); }
        |       OPEN_PAREN expr CLOSE_PAREN { PARSER_FWD(OPEN_PAREN); }
        |       ID { printf("# DEBUG: GOT TO ID (%s)\n", yytext); PARSER_FWD(ID); }
        |       I32_LIT { PARSER_FWD(I32_LIT); }
        |       F32_LIT { PARSER_FWD(F32_LIT); }
        ;

%%

void yyerror (char const *s)
{
    extern int yylineno;
    fprintf(stderr, "Error at yylineno: %d, yylloc=(%d, %d)\n\t%s\n", yylineno, yylloc.line, yylloc.column, s);
    yy_errored_out = true;
}

// yywrap: return 1 to stop the parser/lexer upon encountering EOF
int yywrap(void)
{
    return 1;
}
