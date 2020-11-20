#include "dpcc.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "globals.h"
#include "lexer.h"
#include "parser.h"
#include "types.h"
#include "utils.h"

FILE *open_from_string(char *string)
{
    FILE *result = fmemopen(string, strlen(string), "r");
    if (result == NULL) {
        fprintf(stderr, "Failed create FILE* stream from string\n");
        abort();
    }

    return result;
}

FILE *open_file_for_reading(char *filepath)
{
    FILE *result = fopen(filepath, "r");
    if (result == NULL) {
        fprintf(stderr, "Failed to open file for reading (%s)\n", filepath);
        abort();
    }

    return result;
}

static void reset(void)
{
    yylex_destroy();
    clear_all_global_vars();
}

static void setup_filepath(char *filepath)
{
    if (!filepath) {
        yylloc.filepath = strdup("<UNNAMED>");
    } else {
        yylloc.filepath = strdup(filepath);
    }
}

bool lex(char *filepath, FILE *input_stream)
{
    reset();
    setup_filepath(filepath);

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::lex() --- NULL input_stream\n");
        abort();
    }

#if 1
    yyrestart(input_stream);
#else
    yyin = input_stream;
#endif

    int result = 0;
    int kind = 0;
    while ((kind = yylex()) != TOK_YYEOF) {
        if (kind == TOK_YYUNDEF || kind == TOK_YYerror) {
            result = kind;
            break;
        }
    }

    return result == 0;
}

bool parse(char *filepath, FILE *input_stream)
{
    reset();
    setup_filepath(filepath);

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::parse() --- NULL input_stream\n");
        abort();
    }

    yyin = input_stream;

    int result = yyparse();
    return result == 0;
}

bool compile(char *filepath, FILE *input_stream, FILE *output_stream)
{
    reset();
    setup_filepath(filepath);

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::parse() --- NULL input_stream\n");
        abort();
    }
    if (output_stream == NULL) {
        output_stream = stdout;
    }

    bool parse_success = parse(filepath, input_stream);
    if (!parse_success) {
        return false;
    }

    /// Log and maybe error out (set success to false)
#define LOG(severity, node, ...)               \
    do {                                       \
        dpcc_log(severity, node, __VA_ARGS__); \
        if (severity == DPCC_SEVERITY_ERROR)   \
            success = false;                   \
    } while (0)

    bool success = true;

    // Broken code
#if 0
    for (usize node_idx = 0; node_idx < (usize)G_ast.nodes_cnt; node_idx++) {
        ast_node_t *node = &G_ast.nodes[node_idx];
        LOG(DPCC_SEVERITY_WARNING, node, "Hello from node (lexeme: `%s`, skind: %s)\n", node->tok->lexeme, node->skind);
    }
#endif

#undef LOG
    return success;
}

static void ast_traversal_push(ast_traversal_t *t, ast_node_t *parent, int32_t current_child)
{
    t->stack_nodes = dallrsz(&G_allctx, t->stack_nodes, (t->stack_cnt + 1) * sizeof(parent));
    t->stack_childs = dallrsz(&G_allctx, t->stack_childs, (t->stack_cnt + 1) * sizeof(current_child));
    t->stack_cnt += 1;
    t->stack_nodes[t->stack_cnt - 1] = parent;
    t->stack_childs[t->stack_cnt - 1] = current_child;
}

static inline bool ast_traversal_pop(ast_traversal_t *t)
{
    if (t->stack_cnt == 0) {
        return false;
    }
    t->stack_cnt -= 1;
    return true;
}

void ast_traversal_begin(ast_traversal_t *t)
{
    ast_traversal_push(t, &G_root_node, 0);
}

ast_node_t *ast_traverse_next(ast_traversal_t *t)
{
    static int32_t cnt = 0;
    if (t->stack_cnt == 0) {
        return NULL;
    }

    while (t->stack_childs[t->stack_cnt - 1] < t->stack_nodes[t->stack_cnt - 1]->num_childs) {
        int32_t ci = t->stack_childs[t->stack_cnt - 1];
        ast_node_t *child = NULL;

        while ((ci < t->stack_nodes[t->stack_cnt - 1]->num_childs) && ((child = t->stack_nodes[t->stack_cnt - 1]->childs[ci]) == NULL)) {
            printf("cnt: %d\n", cnt++);
            ci++;
        }
        if (child) {
            t->stack_childs[t->stack_cnt - 1] = ci + 1;
            ast_traversal_push(t, child, 0);
        } else {
            break;
        }
    }

    ast_node_t *nvcs = t->stack_nodes[t->stack_cnt - 1];
    ast_traversal_pop(t);

    if (nvcs->kind == TOK_YYEOF) {
        return NULL;
    }

    return nvcs;
}
