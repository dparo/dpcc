#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "parser.h"

#include "types.h"
#include "utils.h"

typedef struct str_i32_tuple {
    char *string;
    i32 expected;
} str_i32_tuple_t;

typedef struct str_f32_tuple {
    char *string;
    f32 expected;
} str_f32_tuple_t;

static str_i32_tuple_t i32_tests[] = {
    { "0", 0 },
    { "1", 1 },
    { "0xff", 0xff },
    { "+0xcc", +0xcc },
    { "0xAB", 0xAB },
    { "-0xff", -0xff },
};

static str_f32_tuple_t f32_tests[] = {
    { "0.0", 0 },
    { "1.0", 1.0 },
    { "2.32", 2.32 },
    { "0.04", 0.04 },
    { ".001", .001 },
    { "1.", 1.0 },
    { "1e-3", 1e-3 },
    { "1E-3", 1E-3 },

    { "+0.0", 0 },
    { "+1.0", 1.0 },
    { "+2.32", 2.32 },
    { "+0.04", 0.04 },
    { "+.001", .001 },
    { "+1.", 1.0 },
    { "+1e-3", 1e-3 },
    { "+1E-3", 1E-3 },

    { "-0.0", -0 },
    { "-1.0", -1.0 },
    { "-2.32", -2.32 },
    { "-0.04", -0.04 },
    { "-.001", -.001 },
    { "-1.", -1.0 },
    { "-1e-3", -1e-3 },
    { "-1E-3", -1E-3 },

};

typedef struct {
    char *buffer;
    i32_array_t expected_tokens;
} dpcc_test_t;

static dpcc_test_t all_dpcc_tests[] = {
    { "a = a; b = b; long_name = a + long_name; // A comment",
        ARRAY_LIT(i32, { TOK_ID, TOK_ASSIGN, TOK_ID, TOK_SEMICOLON, TOK_ID, TOK_ASSIGN, TOK_ID, TOK_SEMICOLON, TOK_ID, TOK_ASSIGN, TOK_ID, TOK_ADD, TOK_ID, TOK_SEMICOLON }) },
    { "a = b; // A comment",
        ARRAY_LIT(i32, { TOK_ID, TOK_ASSIGN, TOK_ID, TOK_SEMICOLON }) },
    { "a = b; // A comment\n",
        ARRAY_LIT(i32, { TOK_ID, TOK_ASSIGN, TOK_ID, TOK_SEMICOLON }) },
};
