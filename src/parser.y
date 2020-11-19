%define parse.error detailed
/* Sets YYSTYPE used for semantic values */
%define api.value.type {ast_node_t*}

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
// --- Most of this precedence table is C derived
// --- The only difference is that the bitwise and, or, xor are pushed higher in precedence compared to the "==", "!=" operators
// --- the reasons in mainly for convenience. Most modern languaes applied the same form of change
%left                   ASSIGN
%left                   LOR
%left                   LAND
%left                   EQ NEQ
%left                   BXOR
%left                   BOR
%left                   BAND
%left                   BLSHIFT BRSHIFT
%left                   ADD SUB
%left                   MUL DIV MOD
%left                   POW
%right                  POS NEG
%left                   INC DEC


/* When error recovery, bison may want to discard some symbols. So
   it is generally good practice to free any allocated memory here. */
// %destructor { printf ("Discarding TAG-FILLED symbol\n"); if(0) free ((void*) $$); } <*>
// %destructor { printf ("Discarding TAG-LESS symbol\n"); if(0) free((void*) $$); } <>

%%


/* Bison MANUAL says to prefer left recursion where possible (bounded stack space) */
car:            car stmt
        |       car decl
        |       YYEOF                             {  }
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


stmts:          stmts stmt
        ;

stmt:           assignment
        |       if_statement
        |       ";"                               { /* Eat empty statements */ }
        ;

if_statement: "if" "(" expr ")" "{" stmts "}"
        ;


assignment: ID "=" expr ";"                       { PUSH(STATEMENT); }
        ;

expr:           expr[lhs] "+" expr[rhs] %prec ADD { PUSH(PLUS); }
        |       expr[lhs] "-" expr[rhs] %prec SUB { PUSH(MINUS); }
        |       expr[lhs] "*" expr[rhs] %prec MUL { PUSH(MUL); }
        |       "+" expr[e]             %prec POS { PUSH(NEG); }
        |       "-" expr[e]             %prec NEG { PUSH(NEG); }
        |       "(" expr[e] ")"                   { PUSH(OPEN_PAREN); }
        |       ID                                { PUSH(ID); }
        |       I32_LIT                           { PUSH_I32(I32_LIT); }
        |       F32_LIT                           { PUSH_F32(F32_LIT); }
        |       CHAR_LIT                          { PUSH_CHAR(CHAR_LIT); }
        |       BOOL_LIT                          { PUSH_BOOL(BOOL_LIT); }
        ;

%%

