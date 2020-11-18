#pragma once

#include "types.h"

extern token_t *yyltoken;
extern int32_t yyprevcol;
extern YYLTYPE yylloc;

extern str_str_key_value_t *G_str_intern;
extern mctx_t G_allctx;
extern token_seq_t G_tok_seq;
extern ast_t G_ast;

void clear_all_global_vars(void);
