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
%locations
%define api.location.type {loc_t}

        // As from the Bison MANUAL using LAC parser instead of the default LALR parser
        // table implementations can lead to better error messages provided.
        // Also user actions associated with tokens lookahead are not executed in case
        // of syntax errors. Also since it stores stack frame of the parser state
        // prior to do expolarion it can lead to better identification of
        // the token causing the syntax error
%define parse.lac   full
%define parse.error custom       // detailed

        // API.token.raw: Makes the entry of the enum `yytokentype`
        // (exported in the h file and used by flex) to make them identical 1:1
        // to the enum `yysymbol_kind_t` which is the one used internally by bison.
        //     From the docs. These makes parsing faster, but also the exposed enum is now
        //     tight (easier to map the enum using an array), this however has the advantages
        //     that character literal cannot be used in the grammar, and they cannot be returned
        //     by flex. So both the grammar and flex must use symbolic names. This is mostly
        //     not a problem, since bison allows to associate C-string with symbolic names anyway.
%define api.token.raw     true
%define api.symbol.prefix {YY_}
%define api.token.prefix  {TOK_}
%define api.value.type    {ast_node_t*}

        // %{ %} code will be inserted in the bison C generated file
        // %code {} Will be inserted in the bison C generated file but after %{ %} the inlcusion of the bison header file and the definition of yysymbol_kind_t
        // %code requires {} will be insterted at the top of the H bison generated header. Mostly used to include types that are used to define typedefs by bison
        // %code provides {} will be generated at the bottom of the H bison generated header. Use case: export functions that you define at the bottom of this bison file

%code {
        #include "globals.h"
        #include "yacc_utils.h"
        #include "dpcc.h"
        #include "log.h"
        #define YYANARRAY (ast_node_t*[])
}

%code requires {
        #include "types.h"
}


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

%token                  COMMA ","
%token                  DOT "."
%token                  ARROW "->"

%token                  COLON ":"
%token                  SEMICOLON ";"
%token                  OPEN_PAREN "("
%token                  CLOSE_PAREN ")"
%token                  OPEN_BRACKET "["
%token                  CLOSE_BRACKET "]"
%token                  OPEN_BRACE "{"
%token                  CLOSE_BRACE "}"



%token KW_INT     "int"
%token KW_FLOAT   "float"
%token KW_BOOL    "bool"


%token KW_FN       "fn"
%token KW_MAIN     "main"
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

%right                  ASSIGN
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
%left                   AR_SUBSCR


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
%token LAST        // NOT USED: Used mainly to export an entry in the final parser.h::yytokentype
                   // so that we can now the number of elements in it, and thus possibly
                   // creating arrays to map enums to values.

%%



ident: ID { $$ = $1; NODE_KIND($$, Ident); }
pident: ID  {
        $$ = $1;
        NODE_KIND($$, Ident);
        YYSTYPE decl = symtable_lookup($$->tok);
        if (!decl) {
                dpcc_log(DPCC_SEVERITY_ERROR, &$$->tok->loc, "Use of undeclared identifier `%s`", $$->tok->lexeme);
                PARSE_ERROR();
        } else {
                $$->decl = decl;
        }
}

i32lit:    I32_LIT                                            { $$ = $1; NODE_KIND($$, IntLit); INIT_I32($$); }
f32lit:    F32_LIT                                            { $$ = $1; NODE_KIND($$, FloatLit); INIT_F32($$); }
charlit:   CHAR_LIT                                           { $$ = $1; NODE_KIND($$, IntLit); INIT_CHAR($$); }
boollit:   BOOL_LIT                                           { $$ = $1; NODE_KIND($$, BoolLit); INIT_BOOL($$); }
stringlit: STRING_LIT                                         { $$ = $1; NODE_KIND($$, StringLit); }

expr_terminal: pident
    |          i32lit
    |          f32lit
    |          charlit
    |          boollit
    |          stringlit
    ;



      // Bison MANUAL says to prefer left recursion where possible. Better memory footprint (bounded stack space)



root: { symtable_begin_block(); } productions { symtable_end_block(); }
        ;

productions:
         main       { NODE_KIND(&G_root_node, RootNode); push_child(&G_root_node, $1); }
       | stmts      { NODE_KIND(&G_root_node, RootNode); NODE_KIND($1, CodeBlock); push_child(&G_root_node, $1); }
       | %empty
       ;


main:    "fn" "main"[op] "(" ")" stmts[s]                    { $$ = NEW_NODE($op->tok, MainFn); push_child($$, $s); }
        ;

stmts:          stmts[car] stmt[self]                        { $$ = $car; push_child($car, $self); }
        |       stmt[self]                                   { $$ = NEW_NODE($self->tok, NullAstNodeKind); push_child($$, $self); }
        ;


stmt:           expr[c] ";"[op]                   { $$ = NEW_NODE($op->tok, Stmt); push_child($$, $c); }
        |       print_stmt[c] ";"[op]             { $$ = NEW_NODE($op->tok, Stmt); push_child($$, $c); }
        |       decl[c] ";"[op]                   { $$ = NEW_NODE($op->tok, Stmt); push_child($$, $c); }
        |       if_stmt[c]                        { $$ = NEW_NODE($c->tok, Stmt); push_child($$, $c); }
        |       for_stmt[c]                       { $$ = NEW_NODE($c->tok, Stmt); push_child($$, $c); }
        |       while_stmt[c]                     { $$ = NEW_NODE($c->tok, Stmt); push_child($$, $c); }
        |       do_while_stmt[c]                  { $$ = NEW_NODE($c->tok, Stmt); push_child($$, $c); }
        |       code_block[c]                     { $$ = NEW_NODE($c->tok, Stmt); push_child($$, $c); }
        |       ";"                               { $$ = NULL; }
        |       error                             {  }
        ;

assignment:     expr[lhs] "="[op] expr[rhs]                   { $$ = NEW_NODE($op->tok, ExprAssign); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        ;

print_stmt:     "print"[op] "(" expr[e] ")"                   { $$ = NEW_NODE($op->tok, PrintStmt); push_child($$, $e); }
        |       "print"[op] "(" error ")"                     {  }
        ;


decl:      integral_var_decl                                  { $$ = $1; if (!var_decl($1)) { PARSE_ERROR(); } }
        |  array_var_decl                                     { $$ = $1; if (!var_decl($1)) { PARSE_ERROR(); } }
        ;

list_init:  "["[op] list_elems[es] "]" { $$ = NEW_NODE($op->tok, InitializerList); push_childs($$, $es->num_childs, $es->childs); $es->num_childs = 0; }
        |   "["[op]  "]"               { $$ = NEW_NODE($op->tok, InitializerList);  }
        |   error           { }
        ;


list_elems:       list_elems[car] "," list_elem[self]  { $$ = $car; push_child($car, $self); }
        |         list_elem[self]                      { $$ = NEW_NODE($self->tok, NullAstNodeKind); push_child($$, $self); }
        ;

list_elem:        expr  { $$ = $1; }
        ;

integral_var_decl: "let"[op] ident[id]                                                  { $$ = NEW_NODE($op->tok, VarDeclStmt); push_childs($$, 3, YYANARRAY {NULL, $id, NULL}); }
        |          "let"[op] ident[id] "=" expr[e]                                      { $$ = NEW_NODE($op->tok, VarDeclStmt); push_childs($$, 3, YYANARRAY {NULL, $id, $e}); }
        |          "let"[op] ident[id] ":" integral_type[t]                             { $$ = NEW_NODE($op->tok, VarDeclStmt); push_childs($$, 3, YYANARRAY {$t, $id, NULL}); }
        |          "let"[op] ident[id] ":" integral_type[t] "=" expr[e]                 { $$ = NEW_NODE($op->tok, VarDeclStmt); push_childs($$, 3, YYANARRAY {$t, $id, $e}); }
        ;

array_var_decl:  "let"[op] ident[id] ":" array_type[t]                                  { $$ = NEW_NODE($op->tok, VarDeclStmt); push_childs($$, 3, YYANARRAY {$t, $id, NULL}); }
        |        "let"[op] ident[id] ":" array_type[t] "=" list_init[il]                { $$ = NEW_NODE($op->tok, VarDeclStmt); push_childs($$, 3, YYANARRAY {$t, $id, $il}); }
        |        "let"[op] ident[id] "=" list_init[il]                                  { YYSTYPE t = NEW_NODE($op->tok, TypeInfoArray); $$ = NEW_NODE($op->tok, VarDeclStmt); push_childs($$, 3, YYANARRAY {t, $id, $il}); }
        ;

integral_type:  "int"      { $$ = $1; NODE_KIND($$, TypeInfoInt); }
        |       "float"    { $$ = $1; NODE_KIND($$, TypeInfoFloat);}
        |       "bool"     { $$ = $1; NODE_KIND($$, TypeInfoBool); }
        ;

array_type:  sized_array_type    { $$ = $1; }
        |    unsized_array_type  { $$ = $1; }
        ;


sized_array_type:     integral_type[t] "["[op] i32lit[n] "]"      { $$ = NEW_NODE($op->tok, TypeInfoArray); push_childs($$, 2, YYANARRAY {$t, $n}); }
        ;

unsized_array_type:   integral_type[t] "["[op] "]"      { $$ = NEW_NODE($op->tok, TypeInfoArray); push_childs($$, 2, YYANARRAY { $t, NULL}); }
        ;

code_block:    "{"[op] {symtable_begin_block(); } stmts[ss] "}"                          {
                        $$ = NEW_NODE($op->tok, CodeBlock);
                        // Keep bison LEFT recursive (faster) and reverse the order of the childs,
                        // only when needed
                        if (0) {
                                for (int32_t i = 0; i < $ss->num_childs / 2; i++) {
                                    YYSTYPE temp = $ss->childs[0];
                                    $ss->childs[0] = $ss->childs[$ss->num_childs - 1 - i];
                                    $ss->childs[$ss->num_childs - 1 - i] = temp;
                                }
                        }
                        push_childs($$, $ss->num_childs, $ss->childs);
                        symtable_end_block();
                }
        |      "{"[op] "}"                                        { $$ = NEW_NODE($op->tok, CodeBlock); }
        ;

for_1: decl
     | assignment
     | %empty { $$ = NULL; }
     ;
for_2: expr
     | %empty { $$ = NULL; }
     ;
for_3: expr
     | %empty { $$ = NULL; }
     ;

if_stmt:         "if"[op] "(" expr[e] ")" code_block[cb]                                              { $$ = NEW_NODE($op->tok, IfStmt); push_childs($$, 3, YYANARRAY { $e, $cb, NULL}); }
        |        "if"[op] "(" expr[e] ")" code_block[cbi] "else" code_block[cbe]                      { $$ = NEW_NODE($op->tok, IfStmt); push_childs($$, 3, YYANARRAY { $e, $cbi, $cbe}); }
        |        "if"[op] "(" expr[e] ")" code_block[cb] "else" if_stmt[car]                          { $$ = NEW_NODE($op->tok, IfStmt); push_childs($$, 3, YYANARRAY { $e, $cb, $car}); }
        |        "if"[op] "(" error ")" code_block[cb]                                                {  }
        |        "if"[op] "(" error ")" code_block[cbi] "else" code_block[cbe]                        {  }
        |        "if"[op] "("error ")" code_block[cb] "else" if_stmt[car]                             {  }
        ;

for_stmt:       "for"[op] "(" { symtable_begin_block(); } for_1[f1] ";" for_2[f2] ";" for_3[f3] ")" code_block[cb]        { symtable_end_block(); $$ = NEW_NODE($op->tok, ForStmt); push_childs($$, 4, YYANARRAY {$f1, $f2, $cb, $f3} ); }
        |       "for" "(" error ")" code_block            {  }
        ;
while_stmt:     "while"[op] "(" expr[e] ")" code_block[cb]           { $$ = NEW_NODE($op->tok, WhileStmt); push_childs($$, 2, YYANARRAY {$e, $cb} ); }
        |       "while" "(" error ")" code_block          {  }
        ;
do_while_stmt:  "do"[op] code_block[cb] "while" "(" expr[e] ")" ";"  { $$ = NEW_NODE($op->tok, DoWhileStmt); push_childs($$, 2, YYANARRAY {$cb, $e} ); }
        |       "do" code_block "while" "(" error ")" ";" {  }
        ;


expr:          "(" error ")"                                       {  }
        |      "(" expr[e] ")"                                     { $$ = $e; }
        |       "+"[op] expr[rhs]                 %prec POS        { $$ = $rhs; }
        |       "-"[op] expr[rhs]                 %prec NEG        { $$ = NEW_NODE($op->tok, ExprNeg); push_child($$, $rhs); }
        |       expr[lhs] "+"[op] expr[rhs]       %prec ADD        { $$ = NEW_NODE($op->tok, ExprAdd); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "-"[op] expr[rhs]       %prec SUB        { $$ = NEW_NODE($op->tok, ExprSub); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "*"[op] expr[rhs]       %prec MUL        { $$ = NEW_NODE($op->tok, ExprMul); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "/"[op] expr[rhs]       %prec DIV        { $$ = NEW_NODE($op->tok, ExprDiv); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "%"[op] expr[rhs]       %prec MOD        { $$ = NEW_NODE($op->tok, ExprMod); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       "!"[op] expr[rhs]                 %prec LNOT       { $$ = NEW_NODE($op->tok, ExprLNot); push_child($$, $rhs); }
        |       expr[lhs] "&&"[op] expr[rhs]      %prec LAND       { $$ = NEW_NODE($op->tok, ExprLAnd); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "||"[op] expr[rhs]      %prec LOR        { $$ = NEW_NODE($op->tok, ExprLOr); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       "~"[op] expr[rhs]                 %prec BNOT       { $$ = NEW_NODE($op->tok, ExprBNot); push_child($$, $rhs); }
        |       expr[lhs] "&"[op] expr[rhs]       %prec BAND       { $$ = NEW_NODE($op->tok, ExprBAnd); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "|"[op] expr[rhs]       %prec BOR        { $$ = NEW_NODE($op->tok, ExprBOr); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "^"[op] expr[rhs]       %prec BXOR       { $$ = NEW_NODE($op->tok, ExprBXor); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "<<"[op] expr[rhs]      %prec BLSHIFT    { $$ = NEW_NODE($op->tok, ExprBLShift); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] ">>"[op] expr[rhs]      %prec BRSHIFT    { $$ = NEW_NODE($op->tok, ExprBRShift); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "=="[op] expr[rhs]      %prec EQ         { $$ = NEW_NODE($op->tok, ExprEq); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "!="[op] expr[rhs]      %prec NEQ        { $$ = NEW_NODE($op->tok, ExprNeq); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "<"[op] expr[rhs]       %prec LT         { $$ = NEW_NODE($op->tok, ExprLt); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "<="[op] expr[rhs]      %prec LTEQ       { $$ = NEW_NODE($op->tok, ExprLtEq); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] ">"[op] expr[rhs]       %prec GT         { $$ = NEW_NODE($op->tok, ExprGt); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] ">="[op] expr[rhs]      %prec GTEQ       { $$ = NEW_NODE($op->tok, ExprGtEq); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       expr[lhs] "**"[op] expr[rhs]      %prec POW        { $$ = NEW_NODE($op->tok, ExprPow); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       pident[lhs] "++"[op]              %prec INC        { $$ = NEW_NODE($op->tok, ExprInc); push_child($$, $lhs); }
        |       pident[lhs] "--"[op]              %prec DEC        { $$ = NEW_NODE($op->tok, ExprDec); push_child($$, $lhs); }
        |       pident[lhs] "["[op] expr[rhs] "]" %prec AR_SUBSCR  { $$ = NEW_NODE($op->tok, ExprArraySubscript); push_childs($$, 2, YYANARRAY {$lhs, $rhs}); }
        |       integral_type[cast] "(" expr[e] ")"                { $$ = NEW_NODE($cast->tok, ExprCast); push_childs($$, 2, YYANARRAY { $cast, $e }); }
        |       assignment                        %prec ASSIGN     { $$ = $1; }
        |       expr_terminal
        
        ;

%%


void yyerror(char const *s)
{
    dpcc_log(DPCC_SEVERITY_ERROR, &yylloc, "%s", s);
}


int yyreport_syntax_error (const yypcontext_t *ctx)
{
    yysymbol_kind_t unexpected = yypcontext_token(ctx);
    YYLTYPE *loc = yypcontext_location(ctx);

    yysymbol_kind_t *expected = NULL;
    int num_expected = yypcontext_expected_tokens(ctx, NULL, 0);

    expected = malloc(sizeof(*expected) * num_expected);

    if (!expected) {
        return YYENOMEM;
    }

    int result = yypcontext_expected_tokens(ctx, expected, num_expected);

    if (result != num_expected) {
        return YYENOMEM;
    }

    dpcc_log(DPCC_SEVERITY_ERROR, loc, "Syntax error. Unexpected token `%s` found", yysymbol_name(unexpected));


    if (expected[0] != YY_YYEMPTY) {
        str_t expected_str = {0};

        for (int32_t i = 0; i < num_expected; i++) {
            sfcat(&G_allctx, &expected_str, "`%s`%s", yysymbol_name(expected[i]), i == (num_expected - 1) ? "" : "  ") ;
        }
        dpcc_log(DPCC_SEVERITY_INFO, loc, "Expected one of   %s", expected_str.cstr);
        dalldel(&G_allctx, expected_str.cstr);
    }

    free(expected);
    return 0;
}
