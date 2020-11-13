#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "parser_utils.h"

#include "dpcc.h"

#include "tests.h"

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
    // assert_int_equal(parse(FS("a = b * c;")), 0);
}



static void correctly_parsed_i32(char *string, i32 expected, bool negated_test)
{
    i32 obtained = INT32_MIN;
    bool success = parse_i32(string, &obtained);
    if (!negated_test) {
        assert_true(success);
        assert_int_equal(obtained, expected);
    } else {
        assert_false(success);
    }
}


static void correctly_parsed_f32(char *string, f32 expected, bool negated_test)
{
    const f32 EPSILON = 0.00001;
    f32 obtained = NAN;
    bool success = parse_f32(string, &obtained);
    if (!negated_test) {
        assert_true(success);
        assert_float_equal(obtained, expected, EPSILON);
    } else {
        assert_false(success);
    }
}


static void number_parsing_test(void **state)
{
    for (i32 i = 0; i < ARRAY_LEN(i32_tests); i++) {
        correctly_parsed_i32(i32_tests[i].string, i32_tests[i].expected, false);
    }

    for (i32 i = 0; i < ARRAY_LEN(f32_tests); i++) {
        correctly_parsed_f32(f32_tests[i].string, f32_tests[i].expected, false);
    }
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(basic_tests),
        cmocka_unit_test(number_parsing_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
