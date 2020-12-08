#pragma once

#include <assert.h>

#include "parser.h"
#include "utils.h"
#include "log.h"
#include "types.h"
#include "dpcc.h"
int yylex(void);
void yyerror(char const *s);

static inline void gen_error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

static bool yacc_from_str_to_i32(ast_node_t *node)
{
    int32_t i = 0;
    bool success = str_to_i32(node->tok->lexeme, &i);
    if (success == false) {
        dpcc_log(DPCC_SEVERITY_ERROR, &node->tok->loc, "Invalid int32 literal: got `%s`", node->tok->lexeme);
    } else {
        node->val.as_i32 = i;
    }
    return success;
}

static bool yacc_from_str_to_f32(ast_node_t *node)
{
    f32 f = 0;
    bool success = str_to_f32(node->tok->lexeme, &f);
    if (success == false) {
        dpcc_log(DPCC_SEVERITY_ERROR, &node->tok->loc, "Invalid float literal: got `%s`", node->tok->lexeme);
    } else {
        node->val.as_f32 = f;
    }
    return success;
}

static bool yacc_from_str_to_char(ast_node_t *node)
{
    char c = 0;
    bool success = str_to_char(node->tok->lexeme, &c);
    if (success == false) {
        dpcc_log(DPCC_SEVERITY_ERROR, &node->tok->loc, "Invalid char literal: got `%s`", node->tok->lexeme);
    } else {
        node->val.as_i32 = c;
    }
    return success;
}

static bool yacc_from_str_to_bool(ast_node_t *node)
{
    bool b = 0;
    bool success = str_to_bool(node->tok->lexeme, &b);
    if (success == false) {
        dpcc_log(DPCC_SEVERITY_ERROR, &node->tok->loc, "Invalid bool literal: got `%s`", node->tok->lexeme);
    } else {
        node->val.as_bool = b;
    }
    return success;
}

#define LEX_STRIP()                                        \
    do {                                                   \
        yylloc.first_column = yylloc.last_column;          \
        yylloc.last_column = yylloc.first_column + yyleng; \
    } while (0)

#define LEX_ERROR()                                                \
    do {                                                           \
        LEX_STRIP();                                               \
        fprintf(stderr, "LEXER UNRECOGNIZED CHAR '%s'\n", yytext); \
        return TOK_YYUNDEF;                                        \
    } while (0)

#define LEX_FWD(X)                                             \
    do {                                                       \
        LEX_STRIP();                                           \
        yylloc.last_line = yylloc.first_line;                  \
        yylloc.last_column = yylloc.first_column + yyleng;     \
        token_t *t = token_new(yylloc, yytext, TOK_##X, (#X)); \
        yylval = new_node(t, TOK_##X, (#X));                   \
        return (TOK_##X);                                      \
    } while (0)

#define NEW_NODE(TOKEN_PTR, KIND) \
    new_node(TOKEN_PTR, KIND, #KIND)

#define NODE_KIND(node, KIND) \
    node_set_kind(node, KIND, (#KIND))

#define PARSE_ERROR() \
    do {              \
        yynerrs += 1; \
        YYERROR;      \
    } while (0)

#define INIT_I32(node)                     \
    do {                                   \
        if (!yacc_from_str_to_i32(node)) { \
            PARSE_ERROR();                 \
        }                                  \
        NODE_KIND(node, I32Lit);           \
    } while (0)

#define INIT_F32(node)                     \
    do {                                   \
        if (!yacc_from_str_to_f32(node)) { \
            PARSE_ERROR();                 \
        }                                  \
        NODE_KIND(node, F32Lit);           \
    } while (0)

#define INIT_CHAR(node)                     \
    do {                                    \
        if (!yacc_from_str_to_char(node)) { \
            PARSE_ERROR();                  \
        }                                   \
        NODE_KIND(node, I32Lit);            \
    } while (0)

#define INIT_BOOL(node)                     \
    do {                                    \
        if (!yacc_from_str_to_bool(node)) { \
            PARSE_ERROR();                  \
        }                                   \
        NODE_KIND(node, BoolLit);           \
    } while (0)

static inline bool var_decl(ast_node_t *var_decl_node)
{
    assert(var_decl_node->tok->kind == TOK_KW_LET);
    assert(var_decl_node->num_childs == 3);
    char *lexeme = var_decl_node->childs[1]->tok->lexeme;
    if (lexeme[0] == '_') {
        dpcc_log(DPCC_SEVERITY_ERROR, &var_decl_node->tok->loc, "Identifiers beginning with `_` are reserved for compiler use.");
        return false;
    }
    ast_node_t *already_declared = symtable_push_sym(var_decl_node);
    if (!already_declared) {
        return true;
    } else {
        dpcc_log(DPCC_SEVERITY_ERROR, &var_decl_node->tok->loc, "Redefinition of previously declared identifier");
        dpcc_log(DPCC_SEVERITY_INFO, &already_declared->tok->loc, "Previous declaration was here");
        return false;
    }
}
