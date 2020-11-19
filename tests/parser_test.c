#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>

#include "dpcc.h"
#include "parser.h"
#include "tests_common.h"

#include <unity.h>
#include <assert.h>

#define FS(X) open_from_string(X)

static void basic_tests(void)
{
    TEST_ASSERT_TRUE(parse(NULL, FS("a = b;")));
    TEST_ASSERT_TRUE(parse(NULL, FS("a = b + c;")));
    TEST_ASSERT_TRUE(parse(NULL, FS("a = ((1 + 2) + b);")));
    TEST_ASSERT_TRUE(parse(NULL, FS("a = b * c * d;")));
}

static void str_to_i32_assert(char *string, i32 expected, bool negated_test)
{
    i32 obtained = INT32_MIN;
    bool success = str_to_i32(string, &obtained);
    if (!negated_test) {
        TEST_ASSERT_TRUE(success);
        TEST_ASSERT_EQUAL_INT(obtained, expected);
    } else {
        TEST_ASSERT_FALSE(success);
    }
}

static void str_to_f32_assert(char *string, f32 expected, bool negated_test)
{
    const f32 EPSILON = 0.00001;
    f32 obtained = NAN;
    bool success = str_to_f32(string, &obtained);
    if (!negated_test) {
        TEST_ASSERT_TRUE(success);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, expected, obtained);
    } else {
        TEST_ASSERT_FALSE(success);
    }
}

static void str_to_literal_test(void)
{
    for (size_t i = 0; i < ARRAY_LEN(i32_tests); i++) {
        str_to_i32_assert(i32_tests[i].string, i32_tests[i].expected, false);
    }

    for (size_t i = 0; i < ARRAY_LEN(f32_tests); i++) {
        str_to_f32_assert(f32_tests[i].string, f32_tests[i].expected, false);
    }
}

static void literal_parsing_test(void)
{
    for (size_t i = 0; i < ARRAY_LEN(i32_tests); i++) {
        TEST_ASSERT_FALSE(parse(NULL, open_from_string(i32_tests[i].string)));
    }

    for (size_t i = 0; i < ARRAY_LEN(f32_tests); i++) {
        TEST_ASSERT_FALSE(parse(NULL, open_from_string(f32_tests[i].string)));
    }
}

int main(void)
{
    UNITY_BEGIN();
    {

        RUN_TEST(basic_tests);
        RUN_TEST(str_to_literal_test);
        RUN_TEST(literal_parsing_test);
    }

    return UNITY_END();
}

/// Ran before each test
void setUp(void)
{
}

/// Ran after each test
void tearDown(void)
{
}
