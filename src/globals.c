#include "globals.h"

char *yylex_debug_ret_val = NULL;
int32_t yyprevcol = 0;
YYLTYPE yylloc = {1, 0};

bool  yybis_error_occured = false;
char *yybis_debug_ret_val = NULL;

mctx_t GALLOC = {0};
