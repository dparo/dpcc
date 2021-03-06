#include "globals.h"
#include <memory.h>

#include "utils.h"
#include <stb_ds.h>

char *G_filepath = NULL;

str_str_key_value_t *G_str_intern = NULL;
mctx_t G_allctx = { 0 };

token_seq_t G_tok_seq = { 0 };
ast_node_t G_root_node = { 0 };

symtable_t G_symtable = { 0 };

int32_t G_codegen_i32_cnt;
int32_t G_codegen_f32_cnt;
int32_t G_codegen_bool_cnt;

int32_t G_codegen_jmp_cnt = 0;

void clear_all_global_vars()
{
    extern loc_t yylloc;
    extern int yynerrs;
    yynerrs = 0;

    yylloc.first_line = 1;
    yylloc.first_column = 1;
    yylloc.last_column = 0;

    G_codegen_i32_cnt = 0;
    G_codegen_f32_cnt = 0;
    G_codegen_bool_cnt = 0;

    G_codegen_jmp_cnt = 0;

    tokens_seq_clear(&G_tok_seq);

    shfree(G_str_intern);
    G_str_intern = NULL;

    memset(&G_root_node, 0, sizeof(G_root_node));
    memset(&G_allctx, 0, sizeof(G_allctx));
    memset(&G_tok_seq, 0, sizeof(G_tok_seq));

    free(G_filepath);
    G_filepath = NULL;

    dallclr(&G_allctx);
}
