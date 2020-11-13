#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "parser_utils.h"


#include "parser.h"
#include "dpcc.h"
#include "tests_common.h"

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
    assert_int_equal(parse(FS("a = b * c * d;")), 0);
}


static void str_to_i32_assert(char *string, i32 expected, bool negated_test)
{
    i32 obtained = INT32_MIN;
    bool success = str_to_i32(string, &obtained);
    if (!negated_test) {
        assert_true(success);
        assert_int_equal(obtained, expected);
    } else {
        assert_false(success);
    }
}


static void str_to_f32_assert(char *string, f32 expected, bool negated_test)
{
    const f32 EPSILON = 0.00001;
    f32 obtained = NAN;
    bool success = str_to_f32(string, &obtained);
    if (!negated_test) {
        assert_true(success);
        assert_float_equal(obtained, expected, EPSILON);
    } else {
        assert_false(success);
    }
}


static void str_to_literal_test(void **state)
{
    for (i32 i = 0; i < ARRAY_LEN(i32_tests); i++) {
        str_to_i32_assert(i32_tests[i].string, i32_tests[i].expected, false);
    }

    for (i32 i = 0; i < ARRAY_LEN(f32_tests); i++) {
        str_to_f32_assert(f32_tests[i].string, f32_tests[i].expected, false);
    }
}


static void literal_parsing_test(void **state)
{
    /* for (i32 i = 0; i < ARRAY_LEN(i32_tests); i++) { */
    /*     assert_int_equal(parse_once(open_from_string(i32_tests[i].string)), I32_LIT); */
    /* } */

    /* for (i32 i = 0; i < ARRAY_LEN(f32_tests); i++) { */
    /*     assert_int_equal( parse_once(open_from_string(f32_tests[i].string)), F32_LIT); */
    /* } */

}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(basic_tests),
        cmocka_unit_test(str_to_literal_test),
        cmocka_unit_test(literal_parsing_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
