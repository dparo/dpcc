#pragma once

#include "types.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>

void ast_clear(ast_t *ast);
ast_node_t *ast_push(ast_t *ast, char *lexeme, i32 kind, char *skind);


bool str_to_i32(char* string, i32* out);
bool str_to_f32(char* string, f32* out);
