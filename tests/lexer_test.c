#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include "dpcc.h"
#include "parser.h"
#include "parser_utils.h"
#include "tests_common.h"

#include <unity.h>
#include <assert.h>


#define FS(X) open_from_string(X)

static void number_lexing_test(void)
{
    for (i32 i = 0; i < ARRAY_LEN(i32_tests); i++) {
        TEST_ASSERT_EQUAL_INT32(I32_LIT, lex_once(open_from_string(i32_tests[i].string)));
    }

    for (i32 i = 0; i < ARRAY_LEN(f32_tests); i++) {
        TEST_ASSERT_EQUAL_INT32(F32_LIT, lex_once(open_from_string(f32_tests[i].string)));
    }
}

int main(void)
{
    UNITY_BEGIN(); {

        RUN_TEST(number_lexing_test);

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
