#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include "dpcc.h"
#include "globals.h"
#include "parser.h"
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

static int lex_test(i32 fixture_idx, fixture_t *fix)
{
    int lexresult = lex();
    bool lexsucc = false;

    bool expected_lex_success = !fix->lex_should_fail;
    i32 expected_tokens_cnt = 0;
    while(fix->lex_expected[expected_tokens_cnt] != YYEOF) {
        expected_tokens_cnt++;
    }

    char *lexmsg;
    asprintf(&lexmsg,
             "--- Fixture test [%d]:\n"
             "--- buffer:"
             "%s\n"
             "--- expected lex success: %d\n"
             "--- expected tokens_cnt: %d\n"
             "--- found lex success: %d\n"
             "--- found tokens_cnt: %d\n",
             fixture_idx,
             fix->buffer,
             expected_lex_success,
             expected_tokens_cnt,
             lexsucc,
             G_tok_seq.tokens_cnt
        );


    TEST_ASSERT_EQUAL_MESSAGE(expected_lex_success, lexsucc, lexmsg);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(expected_tokens_cnt, G_tok_seq.tokens_cnt, lexmsg);

    for (i32 i = 0; i < G_tok_seq.tokens_cnt; i++) {
        char *tokmsg;
        i32 expected_kind = fix->lex_expected[i];
        i32 found_kind = G_tok_seq.tokens[i].kind;
        asprintf(&tokmsg,
                 "%s"
                 "- token_idx: %d\n"
                 "- expected kind: %d\n"
                 "- found kind: %d (%s)\n"
                 "- found lexeme: \"%s\"\n",
                 lexmsg,
                 i,
                 expected_kind,
                 found_kind,
                 G_tok_seq.tokens[i].skind,
                 G_tok_seq.tokens[i].lexeme
            );

        TEST_ASSERT_EQUAL_INT32_MESSAGE(expected_kind, found_kind, tokmsg);
        free(tokmsg);
    }
    free(lexmsg);
}

static void fixture_lexing_test(void)
{
    for (int32_t i = 0; i < ARRAY_LEN(fixture_tests); i++) {
        lex_test(i, &fixture_tests[i]);
    }
}

int main(void)
{
    UNITY_BEGIN(); {

        RUN_TEST(number_lexing_test);
        RUN_TEST(fixture_lexing_test);

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
