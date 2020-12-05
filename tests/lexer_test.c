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
    for (size_t i = 0; i < ARRAY_LEN(i32_tests); i++) {
        TEST_ASSERT_TRUE(dpcc_lex(NULL, open_from_string(i32_tests[i].string)));
        TEST_ASSERT_EQUAL_INT32(1, G_tok_seq.tokens_cnt);
        TEST_ASSERT_EQUAL_INT32(TOK_I32_LIT, G_tok_seq.tokens[0]->kind);
    }

    for (size_t i = 0; i < ARRAY_LEN(f32_tests); i++) {
        TEST_ASSERT_TRUE(dpcc_lex(NULL, open_from_string(f32_tests[i].string)));
        TEST_ASSERT_EQUAL_INT32(1, G_tok_seq.tokens_cnt);
        TEST_ASSERT_EQUAL_INT32(TOK_F32_LIT, G_tok_seq.tokens[0]->kind);
    }
}

static void lex_test(i32 testidx, dpcc_test_t *test)
{
    bool lexsucc = dpcc_lex(NULL, open_from_string(test->buffer));
    bool expected_lex_success = test->expected_tokens.cnt != 0;

    TEST_ASSERT_NOT_NULL(test->expected_tokens.buf);

    char *lexmsg;
    asprintf(&lexmsg,
        "--- Fixture test [%d]:\n"
        "--- buffer:"
        "%s\n"
        "--- expected lex success: %d\n"
        "--- expected tokens_cnt: %zi\n"
        "--- found lex success: %d\n"
        "--- found tokens_cnt: %d\n",
        testidx,
        test->buffer,
        expected_lex_success,
        test->expected_tokens.cnt,
        lexsucc,
        G_tok_seq.tokens_cnt);

    TEST_ASSERT(false == true);
    TEST_ASSERT_EQUAL_MESSAGE(expected_lex_success, lexsucc, lexmsg);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(test->expected_tokens.cnt, G_tok_seq.tokens_cnt, lexmsg);

    for (i32 i = 0; i < G_tok_seq.tokens_cnt; i++) {
        char *tokmsg;
        i32 expected_kind = test->expected_tokens.buf[i];
        i32 found_kind = G_tok_seq.tokens[i]->kind;
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
            G_tok_seq.tokens[i]->skind,
            G_tok_seq.tokens[i]->lexeme);

        TEST_ASSERT_EQUAL_INT32_MESSAGE(expected_kind, found_kind, tokmsg);
        free(tokmsg);
    }
    free(lexmsg);
}

static void testture_lexing_test(void)
{
    for (size_t i = 0; i < ARRAY_LEN(all_dpcc_tests); i++) {
        lex_test(i, &all_dpcc_tests[i]);
    }
}

int main(void)
{
    UNITY_BEGIN();
    {

        RUN_TEST(number_lexing_test);
        RUN_TEST(testture_lexing_test);
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
