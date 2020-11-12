#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "dpcc.h"


#define FS(X) open_from_string(X)


/* A test case that does nothing and succeeds. */
static void basic_tests(void **state)
{

    // Empty File test
    assert_int_equal(parse(FS("")), 0);

    // Empty Statement test
    assert_int_equal(parse(FS(";")), 0);

    // Simple assignment test
    assert_int_equal(parse(FS("a = b;")), 0);


    // A Non trivial expression assignment
    assert_int_equal(parse(FS("a = ((1 + 2) + b);")), 0);

    // Multiply test
    assert_int_equal(parse(FS("a = b * c;")), 0);
}


static void number_parsing_test(void **state)
{
    assert_int_equal(parse_i32("0"), 0);
    assert_int_equal(parse_i32("1"), 1);

    assert_int_equal(parse_i32("1"), 1);

    const float eps = 0.0001;
    assert_float_equal(parse_f32("0.0"), 0.0, eps);
    assert_float_equal(parse_f32("0.0"), 0.0, eps);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(basic_tests),
        cmocka_unit_test(number_parsing_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
