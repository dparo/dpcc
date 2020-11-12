#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "parser.h"
// #include "scanner.c"

int main(int argc, char **argv)
{
    yyparse();
    return 0;
}
