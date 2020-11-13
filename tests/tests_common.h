#pragma once
#include "utils.h"
#include "types.h"


typedef struct str_i32_tuple {
    char *string;
    i32  expected;
} str_i32_tuple_t;


typedef struct str_f32_tuple {
    char *string;
    f32  expected;
} str_f32_tuple_t;


static str_i32_tuple_t i32_tests[] = {
    {"0", 0},
    {"1", 1},
    {"0xff", 0xff},
    {"+0xcc", +0xcc},
    {"0xAB", 0xAB},
    {"-0xff", -0xff},
};

static str_f32_tuple_t f32_tests[] = {
    {"0.0", 0},
    {"1.0", 1.0},
    {"2.32", 2.32},
    {"0.04", 0.04},
    {".001", .001},
    {"1.", 1.0},
    {"1e-3", 1e-3},
    {"1E-3", 1E-3},

    {"+0.0", 0},
    {"+1.0", 1.0},
    {"+2.32", 2.32},
    {"+0.04", 0.04},
    {"+.001", .001},
    {"+1.", 1.0},
    {"+1e-3", 1e-3},
    {"+1E-3", 1E-3},

    {"-0.0", -0},
    {"-1.0", -1.0},
    {"-2.32", -2.32},
    {"-0.04", -0.04},
    {"-.001", -.001},
    {"-1.", -1.0},
    {"-1e-3", -1e-3},
    {"-1E-3", -1E-3},

};