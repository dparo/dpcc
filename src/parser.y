
// If you want to use glr-parse enable this 2 down below
//     The second line specifies the expected number of reduce-reduce conflicts.
//     Even though bison still reports conflicts. GLR parsers are meant to 
//     solve this type of conflicts any way. Thus it is possible to disable
//%glr-parser
//%expect-rr 1

// Track locations
%locations


// Write an extra output file containing verbose descriptions of the parser states
//  and what is done for each type of lookahead token in that state
%verbose

%define parse.lac   full
%define parse.error detailed

%define api.symbol.prefix {YY_}
%define api.token.prefix  {TOK_}
%define api.value.type    {ast_node_t*}
%define api.location.type {loc_t}

%code requires {
    /* This code block will be exported to the generated header file by bison */
    #include "globals.h"
}
%{

#include "yacc_utils.h"
#define YYERROR_VERBOSE 1

%}


%token                  ID
%token                  I32_LIT
%token                  F32_LIT
%token                  CHAR_LIT
%token                  BOOL_LIT
%token                  STRING_LIT


// The string representation of the tokens allows to show in the error message it's 
// string representation rather than it's associated token kind name. And also
// it allows us to refer to this tokens in the grammar using their string representation
%token                  ASSIGN "="
%token                  PLUS "+"
%token                  MINUS "-"
%token                  MUL "*"
%token                  DIV "/"
%token                  MOD "%"



%token                  EQ "=="
%token                  NEQ "!="
%token                  GT ">"
%token                  GTEQ ">="
%token                  LT "<"
%token                  LTEQ "<="

%token                  INC "++"
%token                  DEC "--"


%token                  LNOT "!"
%token                  LAND "&&"
%token                  LOR "||"

%token                  BNOT "~"
%token                  BAND "&"
%token                  BOR "|"
%token                  BXOR "^"

%token                  BLSHIFT "<<"
%token                  BRSHIFT ">>"

%token                  POW "**"


%token                  COLON ":"
%token                  SEMICOLON ";"
%token                  OPEN_PAREN "("
%token                  CLOSE_PAREN ")"
%token                  OPEN_BRACE "{"
%token                  CLOSE_BRACE "}"


// Derived ast nodes kind
%token                  STATEMENT


%token KW_INT     "int"
%token KW_FLOAT   "float"
%token KW_BOOL    "bool"

%token KW_LET      "let"
%token KW_IF       "if"
%token KW_ELSE     "else"
%token KW_WHILE    "while"
%token KW_DO       "do"
%token KW_FOR      "for"


// Precedence of the operators
// From top to bottom:
// - TOP: Lowest precedence
// - Bottom: Higher precedence
// Spaces can be used to separate operators on the same line,
//  and assign the same precedence
// %precedence can be used to declare a token with no associatvity (eg for unary operators)
// --- Most of this precedence table is C derived.
// --- Differences:
// --- 1. The bitwise and, or, xor are pushed higher in precedence compared to the "==", "!=" operators
// ---    the reasons in mainly for convenience. Most modern languaes applied the same form of change
// --- 2. Equality comparison operators are non associative by default. This avoids possible logical errors
// --- 3. A right associative power operator
%left                   ASSIGN
%left                   LOR
%left                   LAND
%nonassoc               EQ NEQ
%nonassoc               LT LTEQ GT GTEQ
%left                   BXOR
%left                   BOR
%left                   BAND
%left                   BLSHIFT BRSHIFT
%left                   ADD SUB
%left                   MUL DIV MOD
%right                  POW
%right                  POS NEG
%left                   INC DEC


// Printers are usefull for generating traces of the debugger
%printer { fprintf (yyo, "{lexeme: \"%s\"skind: %s, yylloc=[%d, %d]}", $$->tok->lexeme, $$->tok->skind, $$->tok->yylloc.line, $$->tok->yylloc.column); } <>


/* When error recovery, bison may want to discard some symbols. So
   it is generally good practice to free any allocated memory here. */
// %destructor { printf ("Discarding TAG-FILLED symbol\n"); if(0) free ($$); } <*> 
// %destructor { printf ("Discarding TAG-LESS symbol\n"); if(0) free($$); } <>


// Starting symbol. If this is not used bison assumes that the starting symbol is the
// first one declared in the grammar
%start root


%%

root: stmts ;


/* Bison MANUAL says to prefer left recursion where possible (bounded stack space) */
stmts:          stmts stmt
        |       YYEOF                             { }
        ;


stmt:           assignment
        |       decl
        |       if_statement
        |       ";"                               { /* Eat empty statements */ }
        ;

decl:           "let" ID ";"
        |       "let" ID "=" expr ";"
        |       "let" ID ":" type ";"
        |       "let" ID ":" type "=" expr ";"
        ;

type:           "int"
        |       "float"
        |       "bool"
        ;




if_statement: "if" "(" expr ")" "{" stmts "}"
        ;


assignment: ID "=" expr ";"                           { PUSH(STATEMENT); }
        ;

expr:           expr[lhs] "+" expr[rhs] %prec ADD     { PUSH(PLUS); }
        |       expr[lhs] "-" expr[rhs] %prec SUB     { PUSH(MINUS); }
        |       expr[lhs] "*" expr[rhs] %prec MUL     { PUSH(MUL); }
        |       expr[lhs] "/" expr[rhs] %prec DIV     { PUSH(DIV); }
        |       expr[lhs] "%" expr[rhs] %prec MOD     { PUSH(MOD); }
        |       "+" expr[e]             %prec POS     { PUSH(POS); }
        |       "-" expr[e]             %prec NEG     { PUSH(NEG); }
        |       "(" expr[e] ")"                       { PUSH(OPEN_PAREN); }
        |       expr[lhs] "==" expr[rhs] %prec EQ     { PUSH(EQ); }
        |       expr[lhs] "!=" expr[rhs] %prec NEQ    { PUSH(NEQ); }
        |       expr[lhs] "<" expr[rhs] %prec LT      { PUSH(LT); }
        |       expr[lhs] "<=" expr[rhs] %prec LTEQ   { PUSH(LTEQ); }
        |       expr[lhs] ">" expr[rhs] %prec GT      { PUSH(GT); }
        |       expr[lhs] ">=" expr[rhs] %prec GTEQ   { PUSH(GTEQ); }
        |       ID "++"                  %prec INC    { PUSH(INC); }
        |       ID "--"                  %prec DEC    { PUSH(DEC); }
        |       ID                                    { PUSH(ID); }
        |       I32_LIT                               { PUSH_I32(I32_LIT); }
        |       F32_LIT                               { PUSH_F32(F32_LIT); }
        |       CHAR_LIT                              { PUSH_CHAR(CHAR_LIT); }
        |       BOOL_LIT                              { PUSH_BOOL(BOOL_LIT); }
        ;

%%

