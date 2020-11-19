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

bool lex(FILE *input_stream)
{
    reset();

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
    while ((kind = yylex()) != YYEOF) {
        if (kind == YYUNDEF || kind == YYerror) {
            result = kind;
            break;
        }
    }

    return result == 0;
}

bool parse(FILE *input_stream)
{
    reset();

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::parse() --- NULL input_stream\n");
        abort();
    }

    yyin = input_stream;

    int result = yyparse();
    return result == 0;
}

bool compile(FILE *input_stream, FILE *output_stream)
{

    // My fucking comment

    reset();

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::parse() --- NULL input_stream\n");
        abort();
    }
    if (output_stream == NULL) {
        output_stream = stdout;
    }

    bool parse_success = parse(input_stream);
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
    for (usize node_idx = 0; node_idx < (usize)G_ast.nodes_cnt; node_idx++) {
        ast_node_t *node = &G_ast.nodes[node_idx];
        LOG(DPCC_SEVERITY_WARNING, node, "Hello from node (lexeme: `%s`, skind: %s)\n", node->tok->lexeme, node->skind);
    }

#undef LOG
    return success;
}
