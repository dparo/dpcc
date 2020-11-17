%define parse.error detailed

%code requires {
    /* This code block will be exported to the generated header file by bison */
    #include "globals.h"
 }
%{

#include "utils.h"
#include "lexer.h"
#include "parser.h"
#define YYERROR_VERBOSE 1

void yyerror(char const *s);
int  yylex(void);


#define PUSH(X) \
    ast_push(yylval, X, #X, (isize) 0, NULL)


static inline void gen_error(char *msg) {
    fprintf(stderr, "%s\n", msg);
}

static bool yacc_from_str_to_i32(ast_node_t *node) {
    int32_t i = 0;
    bool success = str_to_i32(node->tok->lexeme, &i);
    if (success == false) {
        gen_error("Invalid integer");
    } else {
        node->val.i = i;
    }
    return success;
}

static bool yacc_from_str_to_f32(ast_node_t *node) {
    f32 f = 0;
    bool success = str_to_f32(node->tok->lexeme, &f);
    if (success == false) {
        gen_error("Invalid float");
    } else {
        node->val.f = f;
    }
    return success;
}

static bool yacc_from_str_to_char(ast_node_t *node) {
    char c = 0;
    bool success = str_to_char(node->tok->lexeme, &c);
    if (success == false) {
        gen_error("Invalid char");
    } else {
        node->val.c = c;
    }
    return success;
}

static bool yacc_from_str_to_bool(ast_node_t *node) {
    bool b = 0;
    bool success = str_to_bool(node->tok->lexeme, &b);
    if (success == false) {
        gen_error("Invalid boolean");
    } else {
        node->val.b = b;
    }
    return success;
}




#define YACC_FROM_STR_TO_I32(node) \
    do { if(!yacc_from_str_to_i32(node)) { YYERROR; } } while(0)

#define YACC_FROM_STR_TO_F32(node) \
    do { if(!yacc_from_str_to_f32(node)) { YYERROR; } } while(0)

#define YACC_FROM_STR_TO_CHAR(node) \
    do { if(!yacc_from_str_to_char(node)) { YYERROR; } } while(0)

#define YACC_FROM_STR_TO_BOOL(node) \
    do { if(!yacc_from_str_to_bool(node)) { YYERROR; } } while(0)

%}


%token                  ID
%token                  I32_LIT
%token                  F32_LIT
%token                  CHAR_LIT
%token                  BOOL_LIT
%token                  STRING_LIT

%token                  ASSIGN
%token                  PLUS
%token                  MINUS
%token                  MUL

%token                  SEMICOLON
%token                  OPEN_PAREN
%token                  CLOSE_PAREN
%token                  STATEMENT


// Precedence of the operators
// From top to bottom:
// - TOP: Lowest precedence
// - Bottom: Higher precedence
// Spaces can be used to separate operators on the same line,
//  and assign the same precedence
%left                   ASSIGN
%left                   PLUS MINUS
%left                   MUL
%right                  NEG

%%

lines:           line lines
        |        YYEOF                   {      }
        ;

line:            statement
        ;

statement:      ID { PUSH(ID); } ASSIGN { PUSH(ASSIGN); } expr SEMICOLON { PUSH(STATEMENT); }
        |       SEMICOLON                { PUSH(STATEMENT); }
        ;

expr:           expr PLUS { PUSH(PLUS); } expr 
        |       expr MINUS expr { PUSH(MINUS); }
        |       expr MUL expr { PUSH(MUL); }
        |       MINUS expr  %prec NEG { PUSH(NEG); }
        |       OPEN_PAREN expr CLOSE_PAREN { PUSH(OPEN_PAREN); }
        |       ID { PUSH(ID); }
        
        |       I32_LIT    { YACC_FROM_STR_TO_I32(PUSH(I32_LIT)); }
        |       F32_LIT    { YACC_FROM_STR_TO_F32(PUSH(F32_LIT)); }
        |       CHAR_LIT   { YACC_FROM_STR_TO_F32(PUSH(CHAR_LIT)); }
        |       BOOL_LIT   { YACC_FROM_STR_TO_BOOL(PUSH(BOOL_LIT)); }
        ;

%%

void yyerror (char const *s)
{
    extern int yylineno;
    fprintf(stderr, "Error at yylineno: %d, yylloc=(%d, %d)\n\t%s\n", yylineno, yylloc.line, yylloc.column, s);
    yy_errored_out = true;
}
