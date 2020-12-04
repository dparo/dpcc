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
    if (input_stream == NULL) {
        return 1;
    }

    if (0 == strcmp(mode, "lex")) {
        bool lexsuccess = lex(filepath, input_stream);
        if (!lexsuccess) {
            return -1;
        }

        for (i32 i = 0; i < G_tok_seq.tokens_cnt; i++) {
            print_token(stdout, G_tok_seq.tokens[i]);
        }

    } else if (0 == strcmp(mode, "parse")) {
        bool parsesuccess = parse(filepath, input_stream);
        if (!parsesuccess) {
            return -1;
        }
        ast_traversal_t att = { 0 };
        ast_traversal_begin(&att, &G_root_node);
        ast_node_t *n = NULL;
        int32_t match_idx = 0;
        while ((n = ast_traverse_next(&att, &match_idx)) != NULL) {
            if (match_idx == 0) {
                print_node(stdout, n, att.stack_cnt - 1);
            }
        }
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

    dpcc_reset();
    dallclr(&G_allctx);

    fflush(stderr);
    return 0;
}
