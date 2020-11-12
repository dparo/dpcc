%option yylineno

%{
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"


#define COLUMN_UPDATE() \
    do { /* EMPTY FOR NOW */ } while(0)

#define LEX_FWD(X) \
        do {  \
            COLUMN_UPDATE();                    \
            yylval = (X);                       \
        return (X);   \
    } while(0)

%}

ws       [\t\n\v" "]
digit    [0-9]
alpha    [a-zA-Z_]
alphanum {alpha}|{digit}
number   {digit}*
id       {alpha}{alphanum}*
assign  "="
plus    "+"

%%


{ws}             { COLUMN_UPDATE(); }
{id}             { LEX_FWD(ID); }
{number}         { LEX_FWD(NUMBER); }

"="              { LEX_FWD(ASSIGN); }
"+"              { LEX_FWD(PLUS); }
"("              { LEX_FWD(OPEN_PAREN); }
")"              { LEX_FWD(CLOSE_PAREN); }


EOF { LEX_FWD(YYEOF); }
. { COLUMN_UPDATE(); }


%%
