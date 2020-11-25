#pragma once

#include "types.h"

extern symtable_t G_symtable;
extern char *G_filepath;

extern str_str_key_value_t *G_str_intern;
extern mctx_t G_allctx;
extern token_seq_t G_tok_seq;
extern ast_node_t G_root_node;

extern int32_t G_codegen_addr_inc;
extern int32_t G_codegen_jmp_inc;

void clear_all_global_vars(void);
