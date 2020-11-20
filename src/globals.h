#pragma once

#include "types.h"

extern char *G_filepath;
extern int32_t yyprevcol;
extern tokloc_t yylloc;

extern str_str_key_value_t *G_str_intern;
extern mctx_t G_allctx;
extern token_seq_t G_tok_seq;
extern ast_node_t G_root_node;

void clear_all_global_vars(void);
