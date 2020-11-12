#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "dpcc.h"

/* A test case that does nothing and succeeds. */
static void basic_tests(void **state) {

    // Empty File test
    assert_int_equal(parse(open_from_string("")), 0);

    // Empty Statement test
    assert_int_equal(parse(open_from_string(";")), 0);

    // Simple assignment test
    assert_int_equal(parse(open_from_string("a = b;")), 0);


    // A Non trivial expression assignment
    assert_int_equal(parse(open_from_string("a = ((1 + 2) + b);")), 0);

    // Multiply test
    assert_int_equal(parse(open_from_string("a = b * c;")), 0);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(basic_tests),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
