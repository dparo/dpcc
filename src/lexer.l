%option yylineno

%{
#include "parser.h"
#include "utils.h"


#define STRIP() \
    do { \
        yylloc.column += yyprevcol; \
        yyprevcol = yyleng; \
    } while(0)

#define ERROR() \
    do { \
        STRIP(); \
        fprintf(stderr, "LEXER UNRECOGNIZED CHAR {%s}\n", yytext);\
        return YYUNDEF; \
    } while(0)

#define FWD(X) \
    do {  \
        STRIP();                    \
        yylval = token_push(yylloc, yytext, (X), (#X)); \
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


NASCII           { FWD(YYUNDEF); }
EOF              { FWD(YYEOF); }

{eol}             { yylloc.line += 1; yylloc.column = 0; yyprevcol = 0; }
{ws}              { STRIP(); }
{comment}         { STRIP(); }


{id}             { FWD(ID); }

{i32_lit}        { FWD(I32_LIT); }
{f32_lit}        { FWD(F32_LIT); }


";"              { FWD(SEMICOLON); }
"="              { FWD(ASSIGN); }
"+"              { FWD(PLUS); }
"*"              { FWD(MUL); }

"("              { FWD(OPEN_PAREN); }
")"              { FWD(CLOSE_PAREN); }


.                { ERROR(); }


%%
