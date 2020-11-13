#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "dpcc.h"

#include "tests_common.h"


#define FS(X) open_from_string(X)

static void number_lexing_test(void **state)
{
    for (i32 i = 0; i < ARRAY_LEN(i32_tests); i++) {
        assert_int_equal(lex(open_from_string(i32_tests[i].string)), 0);
    }

    for (i32 i = 0; i < ARRAY_LEN(f32_tests); i++) {
        assert_int_equal(lex(open_from_string(f32_tests[i].string)), 0);
    }
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(number_lexing_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
