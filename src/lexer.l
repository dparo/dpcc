%option nodefault

%{
#include "parser.h"
#include "utils.h"
#include "yacc_utils.h"


#define STRIP() \
    do { \
        yylloc.column += yyprevcol; \
        yyprevcol = yyleng; \
    } while(0)

#define ERROR() \
    do { \
        STRIP(); \
        fprintf(stderr, "LEXER UNRECOGNIZED CHAR '%s'\n", yytext);\
        return TOK_YYUNDEF; \
    } while(0)

#define FWD(X) \
    do {  \
        STRIP();                    \
        token_t t = token_push(yylloc, yytext, TOK_##X, (#X)); \

        return (TOK_##X);   \
    } while(0)


/// yywrap: return 1 to stop the parser/lexer upon encountering EOF.
/// Return 0 and setup a new yyin in order to continue lexing from the new file
static inline int yywrap(void)
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


i32_lit         [+-]?({digit}+|((0[xXbB]({digit}|[abcdefABCDEF])+)))
f32_lit         [+-]?({digit}+([.]{digit}*)?([eE][+-]?{digit}+)?|[.]{digit}+([eE][+-]?{digit}+)?)(f|F)?
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


NASCII           { FWD(YYUNDEF); }
EOF              { FWD(YYEOF); }   /* Must return YYEOF upon EOF in order for bison to work */

{eol}             { yylloc.line += 1; yylloc.column = 0; yyprevcol = 0; }
{ws}              { STRIP(); }
{comment}         { STRIP(); }


{i32_lit}        { FWD(I32_LIT);  }
{f32_lit}        { FWD(F32_LIT); }
{char_lit}       { FWD(CHAR_LIT); }
{bool_lit}       { FWD(BOOL_LIT); }
{string_lit}     { FWD(STRING_LIT); }


":"              { FWD(COLON); }
";"              { FWD(SEMICOLON); }
"="              { FWD(ASSIGN); }
"+"              { FWD(ADD); }
"-"              { FWD(SUB); }
"*"              { FWD(MUL); }
"/"              { FWD(DIV); }
"%"              { FWD(MOD); }


"=="             { FWD(EQ); }
"!="             { FWD(NEQ); }
">"              { FWD(GT); }
">="             { FWD(GTEQ); }
"<"              { FWD(LT); }
"<="             { FWD(LTEQ); }

"++"             { FWD(INC); }
"--"             { FWD(DEC); }
"!"              { FWD(LNOT); }
"&&"             { FWD(LAND); }
"||"             { FWD(LOR); }

"~"              { FWD(BNOT); }
"&"              { FWD(BAND); }
"|"              { FWD(BOR); }
"^"              { FWD(BXOR); }

"<<"             { FWD(BLSHIFT); }
">>"             { FWD(BRSHIFT); }

"**"             { FWD(POW); }



"("              { FWD(OPEN_PAREN); }
")"              { FWD(CLOSE_PAREN); }

"{"              { FWD(OPEN_BRACE); }
"}"              { FWD(CLOSE_BRACE); }


"int"         { FWD(KW_INT); }
"float"       { FWD(KW_FLOAT); }
"bool"        { FWD(KW_BOOL); }

"let"         { FWD(KW_LET); }
"print"       { FWD(KW_PRINT); }
"if"          { FWD(KW_IF); }
"else"        { FWD(KW_ELSE); }
"do"          { FWD(KW_DO); }
"while"       { FWD(KW_WHILE); }
"for"         { FWD(KW_FOR);  }

{id}          { FWD(ID); }
.             { ERROR(); } /* Returning YYUNDEF tells the parser that some lexical error was found. It will emit an error message about an “invalid token”, and enter error-recovery */


%%
