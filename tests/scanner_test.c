#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "dpcc.h"


#define FS(X) open_from_string(X)

/* A test case that does nothing and succeeds. */
static void number_lexing_test(void **state)
{
    assert_int_equal(lex(FS("0b0")), 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(number_lexing_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
