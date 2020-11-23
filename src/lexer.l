%option nodefault

%{
#include "parser.h"
#include "utils.h"
#include "yacc_utils.h"

/// yywrap: return 1 to stop the parser/lexer upon encountering EOF.
/// Return 0 and setup a new yyin in order to continue lexing from the new file
int yywrap(void)
{
    return 1;
}

%}


eol      (\r\n?)|\n

ws       [\t\v" "]
comment  "//".*

digit       [0-9]
hex         [0-9a-fA-F]
alpha       [a-zA-Z_]
alnum       {alpha}|{digit}

space       " "
punct       "!\"#$%&'()*+,-./:;<=>?@[\\]^`{|}~"
escape_seq  \\0|\\a|\\b|\\t|\\n|\\v|\\f|\\r

f32_leading     [+-]?
f32_integral    {digit}+
f32_fractional  [.]{digit}*
f32_trailing    ([eE][+-]?{digit}+)?
f32_markup      [fF]

i32_lit         [+-]?({digit}+|((0[xXbB]({digit}|[abcdefABCDEF])+)))
f32_lit         {f32_leading}{f32_integral}{f32_trailing}|{f32_leading}{f32_integral}{f32_trailing}?{f32_markup}|{f32_leading}{f32_integral}?{f32_fractional}{f32_trailing}?{f32_markup}?
char_lit        "'"({alnum}|{punct}|{space}|{escape_seq}|(\\0x{hex}{2}))"'"
bool_lit        true|false
string_lit      ["](([\\]["])|([^"]))*["]


id       {alpha}{alnum}*


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
EOF              { LEX_FWD(YYEOF); }   /* Must return YYEOF upon EOF in order for bison to work */

{eol}             { yylloc.line += 1; yylloc.column = 0; yyprevcol = 0; }
{ws}              { LEX_STRIP(); }
{comment}         { LEX_STRIP(); }


{i32_lit}        { LEX_FWD(I32_LIT);  }
{f32_lit}        { LEX_FWD(F32_LIT); }
{char_lit}       { LEX_FWD(CHAR_LIT); }
{bool_lit}       { LEX_FWD(BOOL_LIT); }
{string_lit}     { LEX_FWD(STRING_LIT); }



"."              { LEX_FWD(DOT); }
"->"             { LEX_FWD(ARROW); }

":"              { LEX_FWD(COLON); }
";"              { LEX_FWD(SEMICOLON); }
"="              { LEX_FWD(ASSIGN); }
"+"              { LEX_FWD(ADD); }
"-"              { LEX_FWD(SUB); }
"*"              { LEX_FWD(MUL); }
"/"              { LEX_FWD(DIV); }
"%"              { LEX_FWD(MOD); }


"=="             { LEX_FWD(EQ); }
"!="             { LEX_FWD(NEQ); }
">"              { LEX_FWD(GT); }
">="             { LEX_FWD(GTEQ); }
"<"              { LEX_FWD(LT); }
"<="             { LEX_FWD(LTEQ); }

"++"             { LEX_FWD(INC); }
"--"             { LEX_FWD(DEC); }
"!"              { LEX_FWD(LNOT); }
"&&"             { LEX_FWD(LAND); }
"||"             { LEX_FWD(LOR); }

"~"              { LEX_FWD(BNOT); }
"&"              { LEX_FWD(BAND); }
"|"              { LEX_FWD(BOR); }
"^"              { LEX_FWD(BXOR); }

"<<"             { LEX_FWD(BLSHIFT); }
">>"             { LEX_FWD(BRSHIFT); }

"**"             { LEX_FWD(POW); }



"("              { LEX_FWD(OPEN_PAREN); }
")"              { LEX_FWD(CLOSE_PAREN); }

"{"              { LEX_FWD(OPEN_BRACE); }
"}"              { LEX_FWD(CLOSE_BRACE); }


"int"         { LEX_FWD(KW_INT); }
"float"       { LEX_FWD(KW_FLOAT); }
"bool"        { LEX_FWD(KW_BOOL); }

"fn"          { LEX_FWD(KW_FN); }
"main"        { LEX_FWD(KW_MAIN); }
"let"         { LEX_FWD(KW_LET); }
"print"       { LEX_FWD(KW_PRINT); }
"if"          { LEX_FWD(KW_IF); }
"else"        { LEX_FWD(KW_ELSE); }
"do"          { LEX_FWD(KW_DO); }
"while"       { LEX_FWD(KW_WHILE); }
"for"         { LEX_FWD(KW_FOR);  }

{id}          { LEX_FWD(ID); }
.             { LEX_ERROR(); } /* Returning YYUNDEF tells the parser that some lexical error was found. It will emit an error message about an “invalid token”, and enter error-recovery */


%%
