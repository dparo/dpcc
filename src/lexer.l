%option yylineno

%{
#include "parser.h"


#define COLUMN_UPDATE() \
    do { yylloc.column += yyprevcol; yyprevcol = yyleng; } while(0)

#define LEX_FWD(X) \
        do {  \
            COLUMN_UPDATE();                    \
            yykind  = (X); \
            yyskind = (#X); \
            return (X);   \
    } while(0)

%}


eol      (\r\n)|\n
 
ws       [\t\v" "]
comment  "//".*

digit    [0-9]
alpha    [a-zA-Z_]
alphanum {alpha}|{digit}
id       {alpha}{alphanum}*
assign   "="
plus     "+"
mul      "*"


i32_lit      [+-]?(0[xX])?(0[bB])?({digit}|[abcdefABCDEF])+
f32_lit      [+-]?({digit}+([.]{digit}*)?([eE][+-]?{digit}+)?|[.]{digit}+([eE][+-]?{digit}+)?)


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

{eol}             { yylloc.line += 1; yylloc.column = 0; yyprevcol = 0; }
{ws}              { COLUMN_UPDATE(); }
{comment}         { COLUMN_UPDATE(); }


{id}             { LEX_FWD(ID); }

{i32_lit}        { LEX_FWD(I32_LIT); }
{f32_lit}        { LEX_FWD(F32_LIT); }


";"              { LEX_FWD(SEMICOLON); }
"="              { LEX_FWD(ASSIGN); }
"+"              { LEX_FWD(PLUS); }
"*"              { LEX_FWD(MUL); }

"("              { LEX_FWD(OPEN_PAREN); }
")"              { LEX_FWD(CLOSE_PAREN); }


.                { COLUMN_UPDATE(); printf("LEXER UNRECOGNIZED CHAR {%s}\n", yytext); return YYUNDEF; }


%%
