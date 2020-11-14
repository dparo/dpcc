#pragma once

#include "types.h"


extern bool    yy_errored_out;
extern int32_t yyprevcol;
extern YYLTYPE yylloc;


extern mctx_t      G_allctx;
extern token_seq_t G_tok_seq;
extern ast_t       G_ast;


void clear_all_global_vars(void);
