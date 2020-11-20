#pragma once

#include "utils.h"
#include "log.h"
#include "types.h"

#define YYERROR_VERBOSE 1

int yylex(void);
void yyerror(char const *s);

static inline void gen_error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

static ast_node_t *yacc_from_str_to_i32(ast_node_t *node)
{
    int32_t i = 0;
    bool success = str_to_i32(node->tok->lexeme, &i);
    if (success == false) {
        dpcc_log(DPCC_SEVERITY_ERROR, node, "Invalid int32 literal: got `%s`", node->tok->lexeme);
    } else {
        node->val.i = i;
    }
    return node;
}

static ast_node_t *yacc_from_str_to_f32(ast_node_t *node)
{
    f32 f = 0;
    bool success = str_to_f32(node->tok->lexeme, &f);
    if (success == false) {
        dpcc_log(DPCC_SEVERITY_ERROR, node, "Invalid float literal: got `%s`", node->tok->lexeme);
    } else {
        node->val.f = f;
    }
    return node;
}

static ast_node_t *yacc_from_str_to_char(ast_node_t *node)
{
    char c = 0;
    bool success = str_to_char(node->tok->lexeme, &c);
    if (success == false) {
        dpcc_log(DPCC_SEVERITY_ERROR, node, "Invalid char literal: got `%s`", node->tok->lexeme);
    } else {
        node->val.c = c;
    }
    return node;
}

static ast_node_t *yacc_from_str_to_bool(ast_node_t *node)
{
    bool b = 0;
    bool success = str_to_bool(node->tok->lexeme, &b);
    if (success == false) {
        dpcc_log(DPCC_SEVERITY_ERROR, node, "Invalid bool literal: got `%s`", node->tok->lexeme);
    } else {
        node->val.b = b;
    }
    return node;
}

#define LEX_STRIP()                 \
    do {                            \
        yylloc.column += yyprevcol; \
        yyprevcol = yyleng;         \
    } while (0)

#define LEX_ERROR()                                                \
    do {                                                           \
        LEX_STRIP();                                               \
        fprintf(stderr, "LEXER UNRECOGNIZED CHAR '%s'\n", yytext); \
        return TOK_YYUNDEF;                                        \
    } while (0)

#define LEX_FWD(X)                                              \
    do {                                                        \
        LEX_STRIP();                                            \
        token_t *t = token_push(yylloc, yytext, TOK_##X, (#X)); \
        yylval = new_node(t, TOK_##X, (#X));                    \
        return (TOK_##X);                                       \
    } while (0)

#define A ast_node_t **

#define NEW_NODE(TOKEN_PTR, KIND) \
    new_node(TOKEN_PTR, KIND, #KIND)

#define PUSH_CHILDS(CNT, ...) \
    push_childs(yyval, CNT, (ast_node_t *[])__VA_ARGS__)

#define INIT_I32(node)                               \
    do {                                             \
        if (!(yyval = yacc_from_str_to_i32(node))) { \
            YYERROR;                                 \
        } else {                                     \
            node->type = TYPE_I32;                   \
        }                                            \
    } while (0)

#define INIT_F32(node)                               \
    do {                                             \
        if (!(yyval = yacc_from_str_to_f32(node))) { \
            YYERROR;                                 \
        } else {                                     \
            node->type = TYPE_F32;                   \
        }                                            \
    } while (0)

#define INIT_CHAR(node)                               \
    do {                                              \
        if (!(yyval = yacc_from_str_to_char(node))) { \
            YYERROR;                                  \
        } else {                                      \
            node->type = TYPE_I32;                    \
        }                                             \
    } while (0)

#define INIT_BOOL(node)                               \
    do {                                              \
        if (!(yyval = yacc_from_str_to_bool(node))) { \
            YYERROR;                                  \
        } else {                                      \
            node->type = TYPE_BOOL;                   \
        }                                             \
    } while (0)
