#include "globals.h"
#include <memory.h>

#include "utils.h"

i32      yykind    = 0;
char    *yyskind   = NULL;
int32_t  yyprevcol = 0;
YYLTYPE  yylloc    = {1, 0};

bool yy_errored_out = false;

mctx_t      G_allctx  = {0};
ast_t       G_ast     = { 0 };
token_seq_t G_tok_seq = { 0 };


void clear_all_global_vars()
{
    yykind = 0;
    yyskind = NULL;
    yyprevcol = 0;
    yylloc.line = 1;
    yylloc.column = 0;

    yy_errored_out = false;

    tokens_seq_clear(&G_tok_seq);
    ast_clear(&G_ast);


    dallclr(&G_allctx);
    memset(&G_allctx, 0, sizeof(G_allctx));
    memset(&G_tok_seq, 0, sizeof(G_tok_seq));
    memset(&G_ast, 0, sizeof(G_ast));
}
