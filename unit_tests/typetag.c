#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include "cosc.h"

#ifndef COSC_NOEXTRAS

static void test_typetag_validate(void **state)
{
    cosc_int32 invalid = 0;
    assert_true(cosc_typetag_validate(",ifsb", 1024, &invalid));
    assert_int_equal(invalid, -1);
    assert_true(cosc_typetag_validate(",htd", 1024, &invalid));
    assert_int_equal(invalid, -1);
    assert_false(cosc_typetag_validate("ifsb", 1024, &invalid));
    assert_int_equal(invalid, 0);
    assert_false(cosc_typetag_validate(",ifsbx", 1024, &invalid));
    assert_int_equal(invalid, 5);
    assert_false(cosc_typetag_validate(",ifsbx", 0, &invalid));
    assert_int_equal(invalid, -1);
    assert_false(cosc_typetag_validate("", 1024, &invalid));
    assert_int_equal(invalid, -1);
}

static void test_typetag_match_equal(void **state)
{
    assert_true(cosc_pattern_match(",ifsb", 1024, "ifsb", 1024));
    assert_true(cosc_pattern_match(",", 1024, "", 1024));

    assert_true(cosc_pattern_match(",ifsb", 1024, ",ifsb", 1024));
    assert_true(cosc_pattern_match(",", 1024, ",", 1024));

    assert_false(cosc_pattern_match(",ifsb", 1024, "ifsx", 1024));
    assert_false(cosc_pattern_match(",", 1024, "i", 1024));

    assert_false(cosc_pattern_match(",ifsb", 1024, ",ifsx", 1024));
    assert_false(cosc_pattern_match(",", 1024, ",i", 1024));

#ifndef COSC_NOARRAY
    assert_true(cosc_pattern_match(",ifsb[fff]", 1024, "ifsbfff", 1024));
    assert_true(cosc_pattern_match(",[iff]", 1024, "iff", 1024));
    assert_true(cosc_pattern_match(",ifsb[fff]", 1024, ",ifsbfff", 1024));
    assert_true(cosc_pattern_match(",[iff]", 1024, ",iff", 1024));
    assert_false(cosc_pattern_match(",ifsb[fff]", 1024, "ifsbfif", 1024));
    assert_false(cosc_pattern_match(",[iff]", 1024, "iif", 1024));
    assert_false(cosc_pattern_match(",ifsb[fff]", 1024, ",ifsbfif", 1024));
    assert_false(cosc_pattern_match(",[iff]", 1024, ",iif", 1024));
#endif
}

static void test_typetag_match_asterisk(void **state)
{
    assert_true(cosc_pattern_match(",ifsb", 1024, "i*", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "if*", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "ifs*", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "*ifsb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "ifsb*", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "*i*", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "*if*", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "*ifs*", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "i*fsb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "if*sb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "ifs*b", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "*ifsb*", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "*i*f*s*b*", 1024));

    assert_false(cosc_pattern_match(",ifsb", 1024, "i*i", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "if*i", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "ifs*i", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "ifsb*i", 1024));
}

static void test_typetag_match_question(void **state)
{
    assert_true(cosc_pattern_match(",ifsb", 1024, "?fsb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "i?sb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "if?b", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "ifs?", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "??sb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "i??b", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "if??", 1024));

    assert_false(cosc_pattern_match(",ifsb", 1024, "ifsb?", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "?ifsb", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "if?sb", 1024));
}

static void test_typetag_match_charset(void **state)
{
    assert_true(cosc_pattern_match(",ifsb", 1024, "[iht]fsb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "[hit]fsb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "[hti]fsb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "i[fht]sb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "i[hft]sb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "i[htf]sb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "ifs[bht]", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "ifs[hbt]", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "ifs[htb]", 1024));

    assert_false(cosc_pattern_match(",ifsb", 1024, "[fht]fsb", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "[hft]fsb", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "[htf]fsb", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "i[iht]sb", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "i[hit]sb", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "i[hti]sb", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "ifs[sht]", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "ifs[hst]", 1024));
    assert_false(cosc_pattern_match(",ifsb", 1024, "ifs[hts]", 1024));
}

static void test_typetag_match_stringset(void **state)
{
    assert_true(cosc_pattern_match(",ifsb", 1024, "{ifsb}", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "{ifsb,abcd,abcd}", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "{abcd,ifsb,abcd}", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "{abcd,abcd,ifsb}", 1024));
    assert_true(cosc_pattern_match(",", 1024, "{}", 1024));
    assert_true(cosc_pattern_match(",", 1024, "{,abcd,abcd}", 1024));
    assert_true(cosc_pattern_match(",", 1024, "{abcd,,abcd}", 1024));
    assert_true(cosc_pattern_match(",", 1024, "{abcd,abcd,}", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "{i,a,b,c}fsb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "{a,i,b,c}fsb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "{a,b,i,c}fsb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "{a,b,c,i}fsb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "{xx,if,xx}sb", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "i{xx,fs,xx}b", 1024));
    assert_true(cosc_pattern_match(",ifsb", 1024, "if{xx,sb,xx}", 1024));
}

static void test_typetag_match_scalar(void **state)
{
    assert_true(cosc_pattern_match(",irfhtd", 1024, "######", 1024));
    assert_true(cosc_pattern_match(",irfhtds", 1024, "######s", 1024));
    assert_true(cosc_pattern_match(",sirfhtd", 1024, "s######", 1024));
    assert_true(cosc_pattern_match(",sirfhtds", 1024, "s######s", 1024));
    assert_false(cosc_pattern_match(",s", 1024, "#", 1024));
    assert_false(cosc_pattern_match(",S", 1024, "#", 1024));
    assert_false(cosc_pattern_match(",b", 1024, "#", 1024));
    assert_false(cosc_pattern_match(",m", 1024, "#", 1024));
    assert_false(cosc_pattern_match(",T", 1024, "#", 1024));
    assert_false(cosc_pattern_match(",F", 1024, "#", 1024));
    assert_false(cosc_pattern_match(",N", 1024, "#", 1024));
    assert_false(cosc_pattern_match(",I", 1024, "#", 1024));
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_typetag_validate),
        cmocka_unit_test(test_typetag_match_equal),
        cmocka_unit_test(test_typetag_match_asterisk),
        cmocka_unit_test(test_typetag_match_question),
        cmocka_unit_test(test_typetag_match_charset),
        cmocka_unit_test(test_typetag_match_stringset),
        cmocka_unit_test(test_typetag_match_scalar),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

#else
int main(void)
{
    printf("COSC_NOEXTRAS defined, nothing to test.\n");
    return 0;
}
#endif
