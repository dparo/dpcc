%{
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

#define LEX_FWD(X) \
    {                 \
        printf("LEX :: Got %s\n", #X); \
        yylval = (X); \
        return (X);   \
    }

%}

ws     [\t\n\v" "]
digit  [0-9]
alpha  [a-zA-Z_]
alphanum {alpha}|{digt}
number {digit}*


id {alpha}{alphanum}*
assign  "="
plus    "+"

%%

ws:             { printf("Whitespace fag\n"); }
id:             { LEX_FWD(ID); }
assign:         { LEX_FWD(ASSIGN); }
plus:           { LEX_FWD(PLUS); }
number:         { LEX_FWD(NUMBER); }

EOF: { LEX_FWD(END_OF_FILE); }

%%
