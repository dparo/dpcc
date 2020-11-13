#include "parser_utils.h"
#include "stdlib.h"
#include "types.h"

#include <errno.h>
#include <inttypes.h>
#include <string.h>

bool str_to_i32(char* string, i32* out)
{
    size_t len = strlen(string);
    int base = 10;

    bool is_negated = false;
    if (len >= 1 && (string[0] == '-' || string[0] == '+')) {
        is_negated = string[0] == '-';

        len -= 1;
        string += 1;
    }

    if (len >= 2 && string[0] == '0' && string[1] == 'x') {
        base = 16;
        string += 2;
        len -= 2;
    } else if (len >= 2 && string[0] == '0' && string[1] == 'b') {
        base = 2;
        string += 2;
        len -= 2;
    }

    char* endptr = NULL;
    intmax_t conv_ret_val = strtoimax(string, &endptr, base);

    bool out_of_range = (errno == ERANGE);
    bool failed = len == 0 || out_of_range || endptr == string || endptr == NULL;
    bool result = !failed && *endptr == '\0';

    if (result) {
        *out = (is_negated ? (u32)-1 : (u32)1) * (u32)conv_ret_val;
    } else {
        *out = INT32_MIN;
    }

    return result;
}

bool str_to_f32(char* string, f32* out)
{
    size_t len = strlen(string);

    char* endptr = NULL;
    f32 conv_ret_val = strtof(string, &endptr);

    bool out_of_range = (errno == ERANGE);
    bool failed = len == 0 || out_of_range || endptr == string || endptr == NULL;
    bool result = !failed && *endptr == '\0';

    if (result) {
        *out = conv_ret_val;
    } else {
        *out = INT32_MIN;
    }

    return result;
}

ast_node_t *ast_push(mctx_t *mctx, ast_t *ast, char *lexeme, i32 kind, char *skind)
{
    ast_node_t node = {
        .lexeme = lexeme,
        .kind = kind,
        .skind = skind,
    };

    void* ptr = dallrsz(mctx, ast->nodes, (ast->nodes_cnt + 1) * sizeof(*ast->nodes));

    if (ptr == NULL) {
        fprintf(stderr, "ast_push :: Failed memory allocation\n");
        fflush(stderr);
        abort();
    }

    ast->nodes = ptr;
    ast->nodes_cnt += 1;

    memcpy(&ast->nodes[ast->nodes_cnt - 1], &node, sizeof(node));
    return &ast->nodes[ast->nodes_cnt - 1];
}
