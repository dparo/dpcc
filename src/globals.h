#pragma once

#include "types.h"


extern YYLTYPE yylloc;
extern i32     yyprevcol;


extern char* yylex_debug_ret_val;

extern bool  yybis_error_occured;
extern char *yybis_debug_ret_val;


extern mctx_t G_allctx;


extern token_seq_t G_tok_seq;
extern ast_t  G_ast;


void clear_all_global_vars(void);
