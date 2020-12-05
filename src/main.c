#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dpcc.h"
#include "lexer.h"
#include "parser.h"

typedef struct {
    char *mode;
    char *input_filepath;
    char *output_filepath;
} cmdline_args_t;

static cmdline_args_t S_cmdargs;

void print_usage(char *argv0)
{
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "    %s lex <input> [-o <out>]     # Lex and print each token in stdout or in the given file\n", argv0);
    fprintf(stderr, "    %s parse <input> [-o <out>]   # Parse and print the entire AST to stdout or in the given file\n", argv0);
    fprintf(stderr, "    %s 3ac <input> [-o <out>]     # Emit 3AC onto stdout or in the given file\n", argv0);
    fprintf(stderr, "    %s cc <input> [-o <out>]      # Emit valid C code containing 3AC instructions onto stdout or in the given file\n", argv0);
    fprintf(stderr, "    %s run <input> [-o <out>]     # Compile the produced C code and run it in one pass (requires GCC to be on the path)\n", argv0);
    fprintf(stderr, "\n");
    abort();
}

void parse_cmdline(int argc, char **argv)
{
    if (argc < 2) {
        print_usage(argv[0]);
    }
    S_cmdargs.mode = argv[1];
    S_cmdargs.input_filepath = argv[2];

    for (int32_t i = 0; i < argc; i++) {
        if (0 == strcmp("-o", argv[i])) {
            if ((i + 1) < argc) {
                S_cmdargs.output_filepath = argv[i + 1];
            } else {
                fprintf(stderr, "Expected valie output_filepath after -o flag\n");
            }
            break;
        }
    }
}

int main(int argc, char **argv)
{
    parse_cmdline(argc, argv);

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

    FILE *input_stream = open_file_for_reading(filepath);
    if (input_stream == NULL) {
        return 1;
    }

    FILE *output_stream = stdout;

    if (0 == strcmp(mode, "lex")) {
        bool lexsuccess = dpcc_lex(filepath, input_stream);
        if (!lexsuccess) {
            return -1;
        }

        for (i32 i = 0; i < G_tok_seq.tokens_cnt; i++) {
            print_token(stdout, G_tok_seq.tokens[i]);
        }

    } else if (0 == strcmp(mode, "parse")) {
        bool parsesuccess = dpcc_parse(filepath, input_stream);
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
    } else if ((0 == strcmp(mode, "3ac"))) {
        char *generated_code = dpcc_3ac(filepath, input_stream);
        if (!generated_code) {
            return -1;
        }
        fprintf(output_stream, "%s", generated_code);
    } else if ((0 == strcmp(mode, "cc"))) {
        bool compile_success = dpcc_cc(filepath, input_stream, output_stream);
        if (!compile_success) {
            return -1;
        }
    } else if ((0 == strcmp(mode, "run"))) {
        bool run_success = dpcc_run(filepath, input_stream);
        if (!run_success) {
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
