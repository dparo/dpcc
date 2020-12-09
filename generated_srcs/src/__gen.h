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



bool is_prefix_op(ast_node_t *n);
bool is_postfix_op(ast_node_t *n);
bool is_expr_node(ast_node_t *n);

void typecheck_expr_and_operators(ast_node_t *n);
