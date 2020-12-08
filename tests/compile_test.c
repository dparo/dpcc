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

static void non_compilable_test(void)
{
    for (size_t i = 0; i < ARRAY_LEN(non_valid_programs); i++) {
        char *s = non_valid_programs[i];
        if (!s)
            continue;

        dpcc_reset();
        bool run_success = dpcc_run("*test*", open_from_string(s));
        printf("program: %s\n", s);
        TEST_ASSERT_FALSE(run_success);
    }
}

int main(void)
{
    UNITY_BEGIN();
    {

        RUN_TEST(non_compilable_test);
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
