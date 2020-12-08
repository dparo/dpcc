#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "types.h"
#include "utils.h"
#include "parser.h"
#include "dpcc.h"

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

    { "10.0", 10.0 },
    { "10e4", 10e4 },
    { ".10", .10 },
    { "10f", 10.0f },
    { "10.0f", 10.0f },
    { "10e4f", 10e4f },
    { ".10f", .10f },
    { "10f", 10.0f },

    { "-10.0", -10.0 },
    { "-10e4", -10e4 },
    { "-.10", -.10 },
    { "-10f", -10.0f },
    { "-10.0f", -10.0f },
    { "-10e4f", -10e4f },
    { "-.10f", -.10f },
    { "-10f", -10.0f },
};

typedef struct {
    char *buffer;
    i32_array_t expected_tokens;
} dpcc_test_t;

static inline bool lex_string(char *string)
{
    return dpcc_lex("", open_from_string(string));
}

static inline bool parse_string(char *string)
{
    return dpcc_parse("", open_from_string(string));
}
