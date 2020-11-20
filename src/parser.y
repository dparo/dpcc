        // Required for %define parse.error {custom,detailed}
        // Ubuntu 20.10 (Tested):
        //      sudo apt install bison   # should be enough
        // Ubuntu 18.04 (Tested), Ubuntu: 20.04:
        //      # We need to pull a deb package from the official ubuntu launchpad repos, and then install it
        //      wget 'https://launchpad.net/ubuntu/+source/bison/2:3.7+dfsg-1/+build/19640339/+files/bison_3.7+dfsg-1_amd64.deb'
        //      sudo apt install ./bison_3.7+dfsg-1_amd64.deb

%require "3.6"
%language "C"

        // Used for getting a trace of the parsing. Usefull for debugging
        // a particular input string to the parser. The custom printer function
        // is used to specify how to print the given token for the trace.
        // Also you must setup the global var `yydebug` to a non zero value
        // to enable the traces at runtime. Also the ill-named `%verbose` is required
        // in this case
%verbose
%define parse.trace
%initial-action {
    yydebug = 1;
}
%printer {
        fprintf(yyo, "$$ = %p", $$);
} <>


        // If you want to use glr-parse enable this 2 down below
        //     The second line specifies the expected number of reduce-reduce conflicts.
        //     Even though bison still reports conflicts. GLR parsers are meant to
        //     solve this type of conflicts any way. Thus it is possible to disable
//%glr-parser
//%expect-rr 1


        // Bison track locations. In the documentation setting this flag, or
        // using any of @n for getting the locations enables bison to track
        // in its context the locations of the tokens. Bison manual
        // says that enabling this feature makes the parser considerably
        // slower. Thus Since we don't use this in the grammar,
        // and we do our location tracking externally anyway we leave this flag
        // commented out and we avoid using any `@n` locations refering in the actions
        // of the grammar
// %locations
// %define api.location.type {tokloc_t}


        // As from the Bison MANUAL using LAC parser instead of the default LALR parser
        // table implementations can lead to better error messages provided.
        // Also user actions associated with tokens lookahead are not executed in case
        // of syntax errors. Also since it stores stack frame of the parser state
        // prior to do expolarion it can lead to better identification of
        // the token causing the syntax error
%define parse.lac   full
%define parse.error detailed       // custom

%define api.symbol.prefix {YY_}
%define api.token.prefix  {TOK_}
%define api.value.type    {ast_node_t*}

        // This code block will be exported to the generated header file by bison

%code requires {
#include "globals.h"
}
%{

#include "yacc_utils.h"
#define YYERROR_VERBOSE 1


#define PUSH(X) \
    ast_push(yyltoken, YY_##X, #X, (isize)0, NULL)

#define INIT_I32(node)                           \
    do {                                      \
        if (!(yyval = yacc_from_str_to_i32(node))) { \
            YYERROR;                          \
        }                                     \
    } while (0)

#define INIT_F32(node)                           \
    do {                                      \
        if (!(yyval = yacc_from_str_to_f32(node))) { \
            YYERROR;                          \
        }                                     \
    } while (0)

#define INIT_CHAR(node)                           \
    do {                                       \
        if (!(yyval = yacc_from_str_to_char(node))) { \
            YYERROR;                           \
        }                                      \
    } while (0)

#define INIT_BOOL(node)                           \
    do {                                       \
        if (!(yyval = yacc_from_str_to_bool(node))) { \
            YYERROR;                           \
        }                                      \
    } while (0)


%}

%token YYEOF 0 "end of file"

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
%token                  ADD "+"
%token                  SUB "-"
%token                  MUL "*"
%token                  DIV "/"
%token                  MOD "%"

%token                  EQ      "=="
%token                  NEQ     "!="
%token                  GT      ">"
%token                  GTEQ    ">="
%token                  LT      "<"
%token                  LTEQ    "<="

%token                  INC     "++"
%token                  DEC     "--"


%token                  LNOT    "!"
%token                  LAND    "&&"
%token                  LOR     "||"

%token                  BNOT    "~"
%token                  BAND    "&"
%token                  BOR     "|"
%token                  BXOR    "^"

%token                  BLSHIFT "<<"
%token                  BRSHIFT ">>"

%token                  POW     "**"


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
%token KW_PRINT    "print"
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
%precedence             OPEN_PAREN CLOSE_PAREN

        // When error recovery, bison may want to discard some symbols. So
        // it is generally good practice to free any allocated memory here.

// %destructor { printf ("Discarding TAG-FILLED symbol\n"); if(0) free ($$); } <*>
// %destructor { printf ("Discarding TAG-LESS symbol\n"); if(0) free($$); } <>


// Starting symbol. If this is not used bison assumes that the starting symbol is the
// first one declared in the grammar
%start root


%%

root: stmts | %empty ;

        // Bison MANUAL says to prefer left recursion where possible. Better memory footprint (bounded stack space)

stmts:          stmts stmt                        { $$ = NULL; }
        |       stmt
        ;


stmt:           assignment
        |       print_stmt
        |       var_decl
        |       if_stmt
        |       for_stmt
        |       while_stmt
        |       do_while_stmt
        |       ";"                               { $$ = NULL; }
        |       error ";"                         { yyerrok; } /* Upon syntax error synchronize to next ";". yyerrok: Resume generating error messages immediately for subsequent syntax errors. */
        ;

assignment:     ID "=" expr ";"                           { $$ = PUSH(STATEMENT); }
print_stmt:     "print" "(" expr ")" ";"                  { $$ = PUSH(STATEMENT); }

var_decl:       "let" ID ";"                              { $$ = PUSH(STATEMENT); }
        |       "let" ID "=" expr ";"                     { $$ = PUSH(STATEMENT); }
        |       "let" ID ":" type ";"                     { $$ = PUSH(STATEMENT); }
        |       "let" ID ":" type "=" expr ";"            { $$ = PUSH(STATEMENT); }
        ;

type:           "int"
        |       "float"
        |       "bool"
        ;

code_block:    "{" stmts "}"                                  { $$ = $2; }
        |      "{" "}"                                        { $$ = NULL; }
        |      "{" error "}"                                  { yyerrok; }
        ;

for_1:          var_decl | assignment | %empty ;
for_2:          expr | %empty ;
for_3:          expr | %empty ;

else_if_stmt:   "else" "if" "(" expr ")" code_block
        |       "else" "if" "(" error ")" code_block          { yyerrok; }

else_if_stmts:  else_if_stmts else_if_stmt
        |       %empty;

if_stmt:        "if" "(" expr ")" code_block else_if_stmts "else" code_block
                "if" "(" error ")" code_block else_if_stmts "else" code_block           { yyerrok; }
        ;

for_stmt:       "for" "(" for_1 ";" for_2 ";" for_3 ")" code_block
        |       "for" "(" error ")" code_block            { yyerrok; }
        ;
while_stmt:     "while" "(" expr ")" code_block
        |       "while" "(" error ")" code_block          { yyerrok; }
        ;
do_while_stmt:  "do" code_block "while" "(" expr ")" ";"
        |       "do" code_block "while" "(" error ")" ";" { yyerrok; }
        ;




expr:          "(" error ")"                              { yyerrok; }
        |      "(" expr[e] ")"                            { $$ = PUSH(OPEN_PAREN); }
        |       "+" expr[e]              %prec POS        { $$ = PUSH(POS); }
        |       "-" expr[e]              %prec NEG        { $$ = PUSH(NEG); }
        |       expr[lhs] "+" expr[rhs]  %prec ADD        { $$ = PUSH(ADD); }
        |       expr[lhs] "-" expr[rhs]  %prec SUB        { $$ = PUSH(SUB); }
        |       expr[lhs] "*" expr[rhs]  %prec MUL        { $$ = PUSH(MUL); }
        |       expr[lhs] "/" expr[rhs]  %prec DIV        { $$ = PUSH(DIV); }
        |       expr[lhs] "%" expr[rhs]  %prec MOD        { $$ = PUSH(MOD); }
        |       expr[lhs] "==" expr[rhs] %prec EQ         { $$ = PUSH(EQ); }
        |       expr[lhs] "!=" expr[rhs] %prec NEQ        { $$ = PUSH(NEQ); }
        |       expr[lhs] "<" expr[rhs]  %prec LT         { $$ = PUSH(LT); }
        |       expr[lhs] "<=" expr[rhs] %prec LTEQ       { $$ = PUSH(LTEQ); }
        |       expr[lhs] ">" expr[rhs]  %prec GT         { $$ = PUSH(GT); }
        |       expr[lhs] ">=" expr[rhs] %prec GTEQ       { $$ = PUSH(GTEQ); }
        |       ID "++"                  %prec INC        { $$ = PUSH(INC); }
        |       ID "--"                  %prec DEC        { $$ = PUSH(DEC); }

        |       ID                                        { $$ = PUSH(ID);            }
        |       I32_LIT                                   { $$ = PUSH(I32_LIT);  INIT_I32($$); }
        |       F32_LIT                                   { $$ = PUSH(F32_LIT);  INIT_F32($$); }
        |       CHAR_LIT                                  { $$ = PUSH(CHAR_LIT); INIT_CHAR($$); }
        |       BOOL_LIT                                  { $$ = PUSH(BOOL_LIT); INIT_BOOL($$); }
        ;

%%

void yyerror(char const *s)
{
    extern int yylineno;

    fprintf(stderr, "errors thus far: %d\n", yynerrs);
    fprintf(stderr, "Error at yylineno: %d, yylloc=(%d, %d)\n\t%s\n", yylineno, yylloc.line, yylloc.column, s);
}


#if 0
int yyreport_syntax_error (const yypcontext_t *ctx)
{

}

#endif
