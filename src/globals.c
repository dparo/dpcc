#include "globals.h"
#include <memory.h>

#include "utils.h"
#include <stb_ds.h>

bool    yy_errored_out = false;
int32_t yyprevcol = 0;
YYLTYPE yylloc = {1, 0};


str_str_key_value_t* G_str_intern = NULL;
mctx_t               G_allctx     = {0};
ast_t                G_ast        = { 0 };
token_seq_t          G_tok_seq    = { 0 };


void clear_all_global_vars()
{
    yyprevcol = 0;
    yylloc.line = 1;
    yylloc.column = 0;

    yy_errored_out = false;

    tokens_seq_clear(&G_tok_seq);
    ast_clear(&G_ast);


    shfree(G_str_intern);
    G_str_intern = NULL;

    dallclr(&G_allctx);
    memset(&G_allctx, 0, sizeof(G_allctx));
    memset(&G_tok_seq, 0, sizeof(G_tok_seq));
    memset(&G_ast, 0, sizeof(G_ast));
}
