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
// NOT USED:
//      Enabled only when testing
// %verbose
// %define parse.trace
// %initial-action {
//     yydebug = 1;
// }


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
#define CAST (ast_node_t*[])
}
%{

#include "yacc_utils.h"
#define YYERROR_VERBOSE 1



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
%token                  PRINT
%token                  VAR_DECL



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
%precedence             POS NEG
%precedence             INC DEC LNOT BNOT
%precedence             OPEN_PAREN CLOSE_PAREN

        // When error recovery, bison may want to discard some symbols. So
        // it is generally good practice to free any allocated memory here.

// NOT USED: We track our allocations by pushing into an allocator that can be
//           freed all at once in demand. After a parse. We just clear the entire
//           allocator
// %destructor { printf ("Discarding TAG-FILLED symbol\n"); if(0) free ($$); } <*>
// %destructor { printf ("Discarding TAG-LESS symbol\n"); if(0) free($$); } <>


// Starting symbol. If this is not used bison assumes that the starting symbol is the
// first one declared in the grammar
%start root


%%

root:    stmts                                   { NODE_KIND($$, YYEOF); push_childs(&G_root_node, 1, CAST { $1 }); }
       | %empty
       ;

        // Bison MANUAL says to prefer left recursion where possible. Better memory footprint (bounded stack space)

stmts:          stmts[car] stmt[self]                        { $$ = $car; push_child($car, $self); }
        |       stmt[self]                                   { $$ = NEW_NODE(NULL, STATEMENT); push_child($$, $self); }
        ;


stmt:           assignment ";"
        |       print_stmt
        |       var_decl
        |       if_stmt
        |       for_stmt
        |       while_stmt
        |       do_while_stmt
        |       ";"                               { $$ = NULL; }
        |       error ";"                         { yyerrok; } /* Upon syntax error synchronize to next ";". yyerrok: Resume generating error messages immediately for subsequent syntax errors. */
        ;

assignment:     ID[lhs] "="[op] expr[rhs]                     { $$ = NEW_NODE($op->tok, ASSIGN); push_childs($$, 2, CAST {$lhs, $rhs}); }
print_stmt:     "print"[op] "(" expr[e] ")" ";"               { $$ = NEW_NODE($op->tok, PRINT); push_child($$, $e); }

var_decl:       "let"[op] ID[id] ";"                          { $$ = NEW_NODE($op->tok, VAR_DECL); push_childs($$, 3, CAST {NULL, $id, NULL}); }
        |       "let"[op] ID[id] "=" expr[e] ";"              { $$ = NEW_NODE($op->tok, VAR_DECL); push_childs($$, 3, CAST {NULL, $id, $e}); }
        |       "let"[op] ID[id] ":" type[t] ";"              { $$ = NEW_NODE($op->tok, VAR_DECL); push_childs($$, 3, CAST {$t, $id, NULL}); }
        |       "let"[op] ID[id] ":" type[t] "=" expr[e] ";"  { $$ = NEW_NODE($op->tok, VAR_DECL); push_childs($$, 3, CAST {$t, $id, $e}); }
        ;

type:           "int"      { $$ = $1; $$->type = TYPE_I32; }
        |       "float"    { $$ = $1; $$->type = TYPE_F32; }
        |       "bool"     { $$ = $1; $$->type = TYPE_BOOL; }
        ;

code_block:    "{" stmts[ss] "}"                              { $$ = $ss; }
        |      "{" "}"                                        { $$ = NULL; }
        |      "{" error "}"                                  { yyerrok; }
        ;

for_1: var_decl
     | assignment
     | %empty { $$ = NULL; }
     ;
for_2: expr
     | %empty { $$ = NULL; }
     ;
for_3: expr
     | %empty { $$ = NULL; }
     ;

else_if_stmt:   "else" "if"[op] "(" expr[e] ")" code_block[cb]     { $$ = NEW_NODE($op->tok, KW_IF); push_childs($$, 2, CAST { $e, $cb}); }
        |       "else" "if" "(" error ")" code_block               { yyerrok; }

else_if_stmts:  else_if_stmts[car] else_if_stmt[eif]               { $$ = $car; push_child($car, $eif); }
        |       %empty                                             { $$ = NULL; }
        ;

if_stmt:         "if"[op] "(" expr[e] ")" code_block[cb] else_if_stmts[car]                           { $$ = NEW_NODE($op->tok, KW_IF); push_childs($$, 4, CAST { $e, $cb, $car, NULL}); }
        |        "if" "(" error ")" code_block else_if_stmts
        |        "if"[op] "(" expr[e] ")" code_block[cb] else_if_stmts[car] "else" code_block[ecb]    { $$ = NEW_NODE($op->tok, KW_IF); push_childs($$, 4, CAST { $e, $cb, $car, $ecb}); }
        |        "if" "(" error ")" code_block else_if_stmts "else" code_block           { yyerrok; }
        ;

for_stmt:       "for"[op] "(" for_1[f1] ";" for_2[f2] ";" for_3[f3] ")" code_block[cb]        { $$ = NEW_NODE($op->tok, KW_FOR); push_childs($$, 4, CAST {$f1, $f2, $f3, $cb} ); }
        |       "for" "(" error ")" code_block            { yyerrok; }
        ;
while_stmt:     "while"[op] "(" expr[e] ")" code_block[cb]           { $$ = NEW_NODE($op->tok, KW_WHILE); push_childs($$, 2, CAST {$e, $cb} ); }
        |       "while" "(" error ")" code_block          { yyerrok; }
        ;
do_while_stmt:  "do"[op] code_block[cb] "while" "(" expr[e] ")" ";"  { $$ = NEW_NODE($op->tok, KW_DO); push_childs($$, 2, CAST {$e, $cb} ); }
        |       "do" code_block "while" "(" error ")" ";" { yyerrok; }
        ;




expr:          "(" error ")"                                  { yyerrok; }
        |      "(" expr[e] ")"                                { $$ = $e; }
        |       "+"[op] expr[rhs]            %prec POS        { $$ = $rhs; }
        |       "-"[op] expr[rhs]            %prec NEG        { $$ = NEW_NODE($op->tok, NEG); push_child($$, $rhs); }
        |       expr[lhs] "+"[op] expr[rhs]  %prec ADD        { $$ = NEW_NODE($op->tok, ADD); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "-"[op] expr[rhs]  %prec SUB        { $$ = NEW_NODE($op->tok, SUB); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "*"[op] expr[rhs]  %prec MUL        { $$ = NEW_NODE($op->tok, MUL); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "/"[op] expr[rhs]  %prec DIV        { $$ = NEW_NODE($op->tok, DIV); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "%"[op] expr[rhs]  %prec MOD        { $$ = NEW_NODE($op->tok, MOD); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       "!"[op] expr[rhs]            %prec LNOT       { $$ = NEW_NODE($op->tok, LNOT); push_child($$, $rhs); }
        |       expr[lhs] "&&"[op] expr[rhs] %prec LAND       { $$ = NEW_NODE($op->tok, LAND); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "||"[op] expr[rhs] %prec LOR        { $$ = NEW_NODE($op->tok, LOR); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       "~"[op] expr[rhs]            %prec BNOT       { $$ = NEW_NODE($op->tok, BNOT); push_child($$, $rhs); }
        |       expr[lhs] "&"[op] expr[rhs]  %prec BAND       { $$ = NEW_NODE($op->tok, BAND); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "|"[op] expr[rhs]  %prec BOR        { $$ = NEW_NODE($op->tok, BOR); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "^"[op] expr[rhs]  %prec BXOR       { $$ = NEW_NODE($op->tok, BXOR); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "<<"[op] expr[rhs] %prec BLSHIFT    { $$ = NEW_NODE($op->tok, BLSHIFT); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] ">>"[op] expr[rhs] %prec BRSHIFT    { $$ = NEW_NODE($op->tok, BRSHIFT); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "=="[op] expr[rhs] %prec EQ         { $$ = NEW_NODE($op->tok, EQ); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "!="[op] expr[rhs] %prec NEQ        { $$ = NEW_NODE($op->tok, NEQ); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "<"[op] expr[rhs]  %prec LT         { $$ = NEW_NODE($op->tok, LT); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "<="[op] expr[rhs] %prec LTEQ       { $$ = NEW_NODE($op->tok, LTEQ); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] ">"[op] expr[rhs]  %prec GT         { $$ = NEW_NODE($op->tok, GT); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] ">="[op] expr[rhs] %prec GTEQ       { $$ = NEW_NODE($op->tok, GTEQ); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       expr[lhs] "**"[op] expr[rhs] %prec POW        { $$ = NEW_NODE($op->tok, POW); push_childs($$, 2, CAST {$lhs, $rhs}); }
        |       ID[lhs] "++"[op]             %prec INC        { $$ = NEW_NODE($op->tok, INC); push_child($$, $lhs); }
        |       ID[lhs] "--"[op]             %prec DEC        { $$ = NEW_NODE($op->tok, DEC); push_child($$, $lhs); }
        |       assignment                   %prec ASSIGN     { $$ = $1; }
        |       ID                                            { NODE_KIND($$, ID); }
        |       I32_LIT                                       { NODE_KIND($$, I32_LIT); INIT_I32($$); }
        |       F32_LIT                                       { NODE_KIND($$, F32_LIT); INIT_F32($$); }
        |       CHAR_LIT                                      { NODE_KIND($$, CHAR_LIT); INIT_CHAR($$); }
        |       BOOL_LIT                                      { NODE_KIND($$, BOOL_LIT); INIT_BOOL($$); }
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
