#include "yacc_utils.h"

void yyerror(char const *s)
{
    extern int yylineno;
    fprintf(stderr, "Error at yylineno: %d, yylloc=(%d, %d)\n\t%s\n", yylineno, yylloc.line, yylloc.column, s);
}
