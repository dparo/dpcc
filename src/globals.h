#pragma once

#include "types.h"


extern YYLTYPE yylloc;
extern i32     yyprevcol;


extern char* yylex_debug_ret_val;

extern bool  yybis_error_occured;
extern char *yybis_debug_ret_val;


extern mctx_t GALLOC;
