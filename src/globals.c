#include "globals.h"
#include <memory.h>

#include "utils.h"

char *yylex_debug_ret_val = NULL;
int32_t yyprevcol = 0;
YYLTYPE yylloc = {1, 0};

bool  yybis_error_occured = false;
char *yybis_debug_ret_val = NULL;

mctx_t G_allctx = {0};
ast_t  G_ast = {0};


void clear_all_global_vars()
{
    yyprevcol = 0;
    yylloc.line = 1;
    yylloc.column = 0;

    yybis_error_occured = false;
    yybis_debug_ret_val = NULL;


    dallclr(&G_allctx);
    memset(&G_allctx, 0, sizeof(G_allctx));
    memset(&G_ast, 0, sizeof(G_ast));
}
