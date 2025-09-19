#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include "cosc.h"

static void test_address_valid(void **state)
{
    cosc_int32 invalid = 0;
    assert_true(cosc_address_validate("/hello/world", 1024, &invalid));
    assert_int_equal(invalid, -1);
    assert_true(cosc_address_validate("/blahblah", 1024, &invalid));
    assert_int_equal(invalid, -1);
}

static void test_address_invalid(void **state)
{
    cosc_int32 invalid = 0;
    assert_false(cosc_address_validate("/he#llo/world", 1024, &invalid));
    assert_int_equal(invalid, 3);
    assert_false(cosc_address_validate("/blahbl*ah", 1024, &invalid));
    assert_int_equal(invalid, 7);
}

static void test_address_match_equal(void **state)
{
    assert_true(cosc_pattern_match("/hello/world", 1024, "/hello/world", 1024));
}

static void test_address_match_asterisk(void **state)
{
    assert_true(cosc_pattern_match("/hello/world", 1024, "/hello/*", 1024));
}

static void test_address_match_question(void **state)
{
    assert_true(cosc_pattern_match("/hello/world", 1024, "/hell?/wo?ld", 1024));
}

static void test_address_match_charset(void **state)
{
    assert_true(cosc_pattern_match("/hello/world", 1024, "/hell[xoy]/world", 1024));
}

static void test_address_match_stringset(void **state)
{
    assert_true(cosc_pattern_match("/hello/world", 1024, "/hello/{abc,world,xyz}", 1024));
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_address_valid),
        cmocka_unit_test(test_address_invalid),
        cmocka_unit_test(test_address_match_equal),
        cmocka_unit_test(test_address_match_asterisk),
        cmocka_unit_test(test_address_match_question),
        cmocka_unit_test(test_address_match_charset),
        cmocka_unit_test(test_address_match_stringset),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
