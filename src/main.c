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
    fprintf(stderr, "    %s cc <input> [-o <out>]      # Emit a valid self contained C code using 3AC instructions onto stdout or in the given file\n", argv0);
    fprintf(stderr, "    %s run <input> [-o <out>]     # Compile the produced C code and run it in one pass (requires GCC to be on the path)\n", argv0);
    fprintf(stderr, "\n");
    abort();
}

void parse_cmdline(int argc, char **argv)
{
    if (argc < 2) {
        print_usage(argv[0]);
    }

    for (int32_t i = 0; i < argc; i++) {
        if ((0 == strcmp(argv[i], "--help")) || (0 == strcmp(argv[i], "-h"))) {
            print_usage(argv[0]);
        }
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

    FILE *input_stream = fopen(S_cmdargs.input_filepath, "r");
    if (input_stream == NULL) {
        fprintf(stderr, "%s: No such file or directory\n", S_cmdargs.input_filepath);
        abort();
    }

    FILE *output_stream = stdout;
    if (S_cmdargs.output_filepath) {
        output_stream = fopen(S_cmdargs.output_filepath, "w");
        if (output_stream) {
            fprintf(stderr, "Failed to open output file (%s)\n", S_cmdargs.output_filepath);
            abort();
        }
    }

    if (0 == strcmp(S_cmdargs.mode, "lex")) {
        bool lexsuccess = dpcc_lex(S_cmdargs.input_filepath, input_stream);
        if (!lexsuccess) {
            return -1;
        }

        for (i32 i = 0; i < G_tok_seq.tokens_cnt; i++) {
            print_token(output_stream, G_tok_seq.tokens[i]);
        }

    } else if (0 == strcmp(S_cmdargs.mode, "parse")) {
        bool parsesuccess = dpcc_parse(S_cmdargs.input_filepath, input_stream);
        if (!parsesuccess) {
            return -1;
        }
        ast_traversal_t att = { 0 };
        ast_traversal_begin(&att, &G_root_node);
        ast_node_t *n = NULL;
        int32_t match_idx = 0;
        while ((n = ast_traverse_next(&att, &match_idx)) != NULL) {
            if (match_idx == 0) {
                print_node(output_stream, n, att.stack_cnt - 1);
            }
        }
    } else if ((0 == strcmp(S_cmdargs.mode, "3ac"))) {
        char *generated_code = dpcc_3ac(S_cmdargs.input_filepath, input_stream);
        if (!generated_code) {
            return -1;
        }
        fprintf(output_stream, "%s", generated_code);
    } else if ((0 == strcmp(S_cmdargs.mode, "cc"))) {
        bool compile_success = dpcc_cc(S_cmdargs.input_filepath, input_stream, output_stream);
        if (!compile_success) {
            return -1;
        }
    } else if ((0 == strcmp(S_cmdargs.mode, "run"))) {
        bool run_success = dpcc_run(S_cmdargs.input_filepath, input_stream);
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
