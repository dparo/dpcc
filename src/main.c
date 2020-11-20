#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dpcc.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv)
{
    char *filepath = "run_tree/in/input.txt";
    char *mode = "compile";

    if (argc >= 2) {
        if (argc >= 3) {
            mode = argv[1];
            filepath = argv[2];
        } else {
            filepath = argv[1];
        }
    }

    printf("Opening file: `%s`\n", filepath);
    FILE *input_stream = open_file_for_reading(filepath);

    if (0 == strcmp(mode, "lex")) {
        bool lexsuccess = lex(filepath, input_stream);
        if (!lexsuccess) {
            return -1;
        }

        for (i32 i = 0; i < G_tok_seq.tokens_cnt; i++) {
            token_t *t = &G_tok_seq.tokens[i];

            printf("Token[%d]: \"%s\"\t\t[kind = %s (%d), yylloc=(%d, %d)]\n",
                i,
                t->lexeme,
                t->skind,
                t->kind,
                t->loc.line,
                t->loc.column);
        }

    } else if (0 == strcmp(mode, "parse")) {
        bool parsesuccess = parse(filepath, input_stream);
        if (!parsesuccess) {
            return -1;
        }

        // Broken ass code

        ast_traversal_t att = { 0 };
        ast_traversal_begin(&att);
        ast_node_t *n = NULL;
        while ((n = ast_traverse_next(&att)) != NULL) {
            print_node(stdout, n);
        }

#if 0
        for (i32 i = 0; i < G_ast.nodes_cnt; i++) {
            ast_node_t *node = &G_ast.nodes[i];
            printf("NODE: {idx: %d, lexeme: \"%s\", kind = %d, skind = %s}\n",
                i,
                node->tok->lexeme,
                node->kind,
                node->skind);
        }
#endif
    } else if ((0 == strcmp(mode, "compile")) || (0 == strcmp(mode, "cc"))) {
        bool compile_success = compile(filepath, input_stream, stdout);
        if (!compile_success) {
            return -1;
        }
    } else {
        fprintf(stderr, "Invalid code path\n");
        fflush(stderr);
        return -1;
    }

    fflush(stderr);
    return 0;
}
