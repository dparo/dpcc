#include "dpcc.h"

#include <assert.h>
#include <stdbool.h>
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
#include "codegen.h"

#include "__3ac_preamble.h"
#include "__3ac_postamble.h"

FILE *dpcc_xfopen_w(char *filepath)
{
    if (filepath == NULL) {
        return stdout;
    }
    FILE *stream = fopen(filepath, "w");
    if (!stream) {
        fprintf(stderr, "Failed to open output file (%s)\n", filepath);
        abort();
    }
    return stream;
}

void symtable_clear(void)
{
    for (int32_t list_idx = 0; list_idx < G_symtable.num_lists; list_idx++) {
        dalldel(&G_allctx, G_symtable.lists[list_idx].records);
    }

    dalldel(&G_allctx, G_symtable.lists);
}

/// NOTE: the token lexeme is considered to be corretly interned.
///       This will allows us to do string matching by a simple pointer comparison
///       instead of using a full blown strcmp (which requires to iterate all characters)
ast_node_t *symtable_lookup(token_t *tok)
{
    // Walk the stack backward. Lasts created symbols have higher precedence
    for (int32_t list_idx = G_symtable.num_lists - 1; list_idx >= 0; list_idx--) {
        for (int32_t record_idx = 0; record_idx < G_symtable.lists[list_idx].num_records; record_idx++) {

            // NOTE: Child number 1 is the actual ID provided in the declaration
            // Since both lexeme are string interned. 2 Strings are equal if they have identical pointers
            if (G_symtable.lists[list_idx].records[record_idx].node->childs[1]->tok->lexeme == tok->lexeme) {
                G_symtable.lists[list_idx].records[record_idx].num_usages += 1;
                return G_symtable.lists[list_idx].records[record_idx].node;
            }
        }
    }
    return NULL;
}

void symtable_begin_block(void)
{
    size_t new_size = (G_symtable.num_lists + 1) * sizeof(*G_symtable.lists);

    G_symtable.lists = dallrsz(&G_allctx, G_symtable.lists, new_size);
    G_symtable.lists[G_symtable.num_lists].num_records = 0;
    G_symtable.lists[G_symtable.num_lists].records = NULL;
    G_symtable.num_lists += 1;
}

void symtable_end_block(void)
{
    symlist_t *list = &G_symtable.lists[G_symtable.num_lists - 1];
    for (int32_t record_idx = 0; record_idx < list->num_records; record_idx++) {
        if (list->records[record_idx].num_usages == 0) {
            dpcc_log(DPCC_SEVERITY_WARNING, &list->records[record_idx].node->tok->loc, "Variable `%s` is never used", list->records[record_idx].node->childs[1]->tok->lexeme);
        }
    }

    dalldel(&G_allctx, G_symtable.lists[G_symtable.num_lists - 1].records);
    G_symtable.num_lists -= 1;
}

ast_node_t *symtable_push_sym(ast_node_t *symvar_decl)
{
    assert(symvar_decl->tok->kind == TOK_KW_LET);
    assert(symvar_decl->num_childs == 3);
    symlist_t *list = &G_symtable.lists[G_symtable.num_lists - 1];

    for (int32_t i = 0; i < list->num_records; i++) {
        if (list->records[i].node->childs[1]->tok->lexeme == symvar_decl->childs[1]->tok->lexeme) {
            return list->records[i].node;
        }
    }

    size_t new_size = (list->num_records + 1) * sizeof(*list->records);
    list->records = dallrsz(&G_allctx, list->records, new_size);

    memset(&list->records[list->num_records], 0, sizeof(list->records[list->num_records]));
    list->records[list->num_records].node = symvar_decl;

    list->num_records += 1;
    return NULL;
}

FILE *
open_from_string(char *string)
{
    FILE *result = fmemopen(string, strlen(string), "r");
    if (result == NULL) {
        fprintf(stderr, "Failed create FILE* stream from string\n");
        abort();
    }

    return result;
}

void dpcc_reset(void)
{
    yylex_destroy();
    clear_all_global_vars();
    symtable_clear();
}

static void setup_filepath(char *filepath)
{
    if (!filepath) {
        G_filepath = strdup("<UNNAMED>");
    } else {
        G_filepath = strdup(filepath);
    }
}

bool dpcc_lex(char *filepath, FILE *input_stream)
{
    dpcc_reset();
    setup_filepath(filepath);

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::lex() --- NULL input_stream\n");
        abort();
    }

    yyrestart(input_stream);

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

bool dpcc_parse(char *filepath, FILE *input_stream)
{
    dpcc_reset();
    setup_filepath(filepath);

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::parse() --- NULL input_stream\n");
        abort();
    }

    yyin = input_stream;

    int result = yyparse();
    extern int yynerrs;
    return result == 0 && yynerrs == 0;
}

char *dpcc_3ac(char *filepath, FILE *input_stream)
{
    dpcc_reset();
    setup_filepath(filepath);

    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::3ac() --- NULL input_stream\n");
        abort();
    }

    bool parse_success = dpcc_parse(filepath, input_stream);
    if (!parse_success) {
        return false;
    }

    extern int yynerrs;
    char *generated_code = NULL;
    generated_code = codegen();

    if (yynerrs != 0) {
        return NULL;
    }

    return generated_code;
}

static bool fdpcc_c(char *filepath, FILE *input_stream, FILE *output_stream)
{
    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::c() --- NULL input_stream\n");
        abort();
    }

    char *code = dpcc_3ac(filepath, input_stream);
    if (code) {
        fwrite(THREEAC_PREAMBLE, 1, THREEAC_PREAMBLE_len, output_stream);
        fprintf(output_stream, "\n\n\n\n\n\n\n");
        fprintf(output_stream, "int main()\n");
        fprintf(output_stream, "{\n");
        fprintf(output_stream, "%s", code);
        fprintf(output_stream, "}\n");
        fprintf(output_stream, "\n\n\n\n\n\n\n");
        fwrite(THREEAC_POSTAMBLE, 1, THREEAC_POSTAMBLE_len, output_stream);
        return true;
    }

    return false;
}

bool dpcc_c(char *filepath, FILE *input_stream, char *output_filepath)
{
    FILE *output_stream = dpcc_xfopen_w(output_filepath);
    return fdpcc_c(filepath, input_stream, output_stream);
}

bool dpcc_gcc(char *filepath, FILE *input_stream, char *output_binary_path)
{
    if (input_stream == NULL) {
        fprintf(stderr, "dpcc::gcc() --- NULL input_stream\n");
        abort();
    }

    if (output_binary_path == NULL) {
        fprintf(stderr, "dpcc:gcc() --- output filepath must be provided\n");
        abort();
    }

    bool result = true;
    str_t gcc_command = { 0 };

    sfcat(&G_allctx, &gcc_command, "gcc -x c -o \"%s\" -", output_binary_path);

    FILE *gcc_pipe = popen(gcc_command.cstr, "w");
    bool c_code_generation_success = fdpcc_c(filepath, input_stream, gcc_pipe);

    if (c_code_generation_success == false) {
        result = false;
        fprintf(stderr, "dpcc_gcc() :: C code generation failed\n");
    }

    int exit_status = pclose(gcc_pipe);
    if (exit_status != 0) {
        result = false;
        fprintf(stderr, "dpcc_gcc() :: GCC returned with a nonzero error status ($status = %d)\n", exit_status);
    }

    return result;
}

bool dpcc_run(char *filepath, FILE *input_stream)
{
    bool result = true;
    char *output_binary_path = tmpnam(NULL);
    bool gcc_success = dpcc_gcc(filepath, input_stream, output_binary_path);
    if (gcc_success == false) {
        result = false;
        fprintf(stderr, "dpcc_run() :: Gcc failed to compile the emitted  C code\n");
    } else {
        str_t run_command = { 0 };
        sfcat(&G_allctx, &run_command, "exec \"%s\"", output_binary_path);

        int exit_status = system(run_command.cstr);
        if (exit_status != 0) {
            result = false;
            fprintf(stderr, "dpcc_run() :: The produced binary returned with a non zero exit status ($status = %d)\n", exit_status);
        }
        remove(output_binary_path);
    }

    return result;
}

void ast_traversal_push(ast_traversal_t *t, ast_node_t *parent, int32_t current_child)
{
    t->stack_nodes = dallrsz(&G_allctx, t->stack_nodes, (t->stack_cnt + 1) * sizeof(parent));
    t->stack_childs = dallrsz(&G_allctx, t->stack_childs, (t->stack_cnt + 1) * sizeof(current_child));
    t->stack_cnt += 1;
    t->stack_nodes[t->stack_cnt - 1] = parent;
    t->stack_childs[t->stack_cnt - 1] = current_child;
}

inline bool ast_traversal_pop(ast_traversal_t *t)
{
    if (t->stack_cnt == 0) {
        return false;
    }
    t->stack_cnt -= 1;
    return true;
}

void ast_traversal_begin(ast_traversal_t *t, ast_node_t *root)
{
    t->stack_cnt = 0;
    if (root == NULL) {
        root = &G_root_node;
    }
    ast_traversal_push(t, root, 0);
}

ast_node_t *ast_traverse_next(ast_traversal_t *t, int32_t *match_idx)
{
    if (t->stack_cnt == 0) {
        return NULL;
    }

    while (true) {
        while (t->stack_childs[t->stack_cnt - 1] < t->stack_nodes[t->stack_cnt - 1]->num_childs) {
            int32_t ci = t->stack_childs[t->stack_cnt - 1];
            ast_node_t *child = NULL;

            while ((ci < t->stack_nodes[t->stack_cnt - 1]->num_childs) && ((child = t->stack_nodes[t->stack_cnt - 1]->childs[ci]) == NULL)) {
                ci++;
            }
            if (child) {
                // Assert that the parent backpointer of the child is indeed
                // correct.
                assert(child->parent == t->stack_nodes[t->stack_cnt - 1]);

                t->stack_childs[t->stack_cnt - 1] = ci + 1;
                ast_traversal_push(t, child, 0);

                if (match_idx) {
                    // First return
                    assert(t->stack_cnt >= 1);
                    assert(t->stack_childs[t->stack_cnt - 1] == 0);
                    *match_idx = t->stack_childs[t->stack_cnt - 1];
                    return t->stack_nodes[t->stack_cnt - 1];
                }
            } else {
                break;
            }
        }

        if (t->stack_cnt == 0) {
            return NULL;
        }
        ast_node_t *nvcs = t->stack_nodes[t->stack_cnt - 1];
        ast_node_t *parent = t->stack_cnt >= 2 ? t->stack_nodes[t->stack_cnt - 2] : 0;

        bool pop_success = ast_traversal_pop(t);

        assert(pop_success);

        // If bottom up (match_idx == NULL return)
        if (match_idx == NULL) {
            return nvcs;
        } else {
            if (parent) {
                *match_idx = t->stack_childs[t->stack_cnt - 1];
            }
            return parent;
        }
    }
}
