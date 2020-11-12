%{
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
void yyerror(char const *s);
int  yylex(void);
%}

%define parse.error detailed

%token                  END_OF_FILE
%token                  ID
%token                  ASSIGN
%token                  PLUS
%token                  NUMBER

%left                   ASSIGN
%left                   PLUS

%%


line:           expr
        |       END_OF_FILE
        ;


expr:           ID { printf("PARSER: Got ID\n");}
        |       NUMBER {printf("PARSER: Got Number\n"); }
                ;


%%

void yyerror (char const *s)
{
    fprintf (stderr, "%s\n", s);
}


// yywrap: return 1 to stop the parser/lexer upon encountering EOF
int yywrap(void)
{
    return 1;
}
