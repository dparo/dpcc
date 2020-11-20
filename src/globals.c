#include "globals.h"
#include <memory.h>

#include "utils.h"
#include <stb_ds.h>

int32_t yyprevcol = 0;
tokloc_t yylloc = { NULL, 1, 0 };

str_str_key_value_t *G_str_intern = NULL;
mctx_t G_allctx = { 0 };

token_seq_t G_tok_seq = { 0 };
ast_node_t G_root_node = { 0 };

void clear_all_global_vars()
{
    yyprevcol = 0;
    yylloc.line = 1;
    yylloc.column = 0;

    tokens_seq_clear(&G_tok_seq);

    shfree(G_str_intern);
    G_str_intern = NULL;

    memset(&G_root_node, 0, sizeof(G_root_node));
    memset(&G_allctx, 0, sizeof(G_allctx));
    memset(&G_tok_seq, 0, sizeof(G_tok_seq));

    free(yylloc.filepath);
    yylloc.filepath = "";
    dallclr(&G_allctx);
}
