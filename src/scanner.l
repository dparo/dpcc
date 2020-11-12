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

/* Handling utf-8 characters: specifying illegal chars */
/* UTF-8 might be: */
/*  - 1 byte: [0-127]     (valid ascii) */
/*  - 2 byte: [0b111x_xxxx] [0b10xx_xxxx] */
/*  - 3 byte: [0b1110_xxxx] [0b10xx_xxxx] [0b10xx_xxxx] */
/*  - 4 byte: [0b1111_0xxx] [0b10xx_xxxx] [0b10xx_xxxx] [0b10xx_xxxx] */

UTF8_P      [\x80-\xbf]
UTF8_H2     [\xc2-\xdf]{UTF8_P}
UTF8_H3     [\xe0-\xef]{UTF8_P}{UTF8_P}
UTF8_H4     [\xf0-\xf4]{UTF8_P}{UTF8_P}{UTF8_P}

NASCII      {UTF8_H2}|{UTF8_H3}|{UTF8_H4}


%%


NASCII           { LEX_FWD(YYUNDEF); }
EOF              { LEX_FWD(YYEOF); }


{ws}             { COLUMN_UPDATE(); }
{id}             { LEX_FWD(ID); }
{number}         { LEX_FWD(NUMBER); }


";"              { LEX_FWD(SEMICOLON); }
"="              { LEX_FWD(ASSIGN); }
"+"              { LEX_FWD(PLUS); }
"("              { LEX_FWD(OPEN_PAREN); }
")"              { LEX_FWD(CLOSE_PAREN); }


.                { COLUMN_UPDATE(); printf("LEXER UNRECOGNIZED CHAR {%s}\n", yytext); return YYUNDEF; }


%%
