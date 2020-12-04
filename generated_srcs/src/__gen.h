//
// Generated from /home/dparo/develop/dpcc/generated_srcs/dist/main.js
//
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>


#include "types.h"
#include "globals.h"
#include "utils.h"
#include "log.h"
#include "dpcc.h"


char *new_tmp_var(ast_node_t *n);
char *new_tmp_label(void);
char *get_type_label(enum DPCC_TYPE t);

bool is_prefix_op(ast_node_t *n);
bool is_postfix_op(ast_node_t *n);
bool is_expr_node(ast_node_t *n);
enum DPCC_TYPE deref_type(enum DPCC_TYPE in);
enum DPCC_TYPE unref_type(enum DPCC_TYPE in);

void typecheck_expr_and_operators(ast_node_t *n);
