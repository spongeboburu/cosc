#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdbool.h>
#include "cosc.h"

static char buffer[1024];

static const union cosc_value WRITE_VALUES[11] =
{
    {.i = 0x12345678},
#ifdef COSC_NOFLOAT32
    {.f = 1234},
#else
    {.f = 12.34},
#endif
    {.r = 0x87654321},
    {.c = 'A'},
    {.m = {1, 2, 3, 4}},
#ifdef COSC_NOINT64
    {.h = COSC_64BITS_INIT(0x12345678, 0x12345678)},
    {.t = COSC_64BITS_INIT(0x87654321, 0x87654321)},
#else
    {.h = 0x1234567812345678},
    {.t = 0x8765432187654321},
#endif
#ifdef COSC_NOFLOAT64
    {.d = COSC_64BITS_INIT(0x1234, 0x5678)},
#else
    {.d = 1234.5678},
#endif
    {.s = {"Hello World!", 1024}},
    {.s = {"Hello World!", 1024}},
    {.b = {"Hello World!", 12}},
};

static const int WRITE_TYPETAG_LEN = 16;
static const char *WRITE_TYPETAG = ",ifrcmhtdsSbTFNI";

static int func_setup(void **state)
{
    memset(buffer, 0, sizeof(buffer));
    return 0;
}

static void test_without_array(void **state)
{
    cosc_int32 ret;
    cosc_int32 value_count = 0;
    union cosc_value read_values[11] = {0};
    ret = cosc_write_values(
        buffer, sizeof(buffer),
        WRITE_TYPETAG, WRITE_TYPETAG_LEN,
        WRITE_VALUES, sizeof(WRITE_VALUES) / sizeof(*WRITE_VALUES),
        &value_count
    );
    assert_int_equal(ret, 92);
    assert_int_equal(value_count, 11);
    value_count = 0;
    ret = cosc_read_values(
        buffer, sizeof(buffer),
        WRITE_TYPETAG, WRITE_TYPETAG_LEN,
        read_values, sizeof(WRITE_VALUES) / sizeof(*WRITE_VALUES),
        &value_count,
        true
    );
    assert_int_equal(ret, 92);
    assert_int_equal(value_count, 11);
}

#ifndef COSC_NOARRAY
static void test_with_array(void **state)
{
    cosc_int32 ret;
    cosc_int32 value_count = 0;
    union cosc_value read_values[31] = {0};
    union cosc_value write_values[31];
    write_values[0].i = 10;
    for (int i = 1; i < 31; i++)
        write_values[i].f = i;
    ret = cosc_write_values(
        buffer, sizeof(buffer),
        ",i[fff]", 1024,
        write_values, 31,
        &value_count
    );
    assert_int_equal(ret, 4 + 12 * 10);
    assert_int_equal(value_count, 31);
    value_count = 0;
    ret = cosc_read_values(
        buffer, sizeof(buffer),
        ",i[fff]", 1024,
        read_values, 31,
        &value_count,
        true
    );
    assert_int_equal(ret, 4 + 12 * 10);
    assert_int_equal(value_count, 31);
    assert_int_equal(read_values[0].i, 10);
}

static void test_with_array_unfinished(void **state)
{
    cosc_int32 ret;
    cosc_int32 value_count = 0;
    union cosc_value read_values[31] = {0};
    union cosc_value write_values[31];
    write_values[0].i = 10;
    for (int i = 1; i < 31; i++)
        write_values[i].f = i;
    ret = cosc_write_values(
        buffer, sizeof(buffer),
        ",i[fff]", 1024,
        write_values, 30,
        &value_count
    );
    assert_int_equal(ret, 4 + 12 * 10);
    assert_int_equal(value_count, 31);
    value_count = 0;
    ret = cosc_read_values(
        buffer, sizeof(buffer),
        ",i[fff]", 1024,
        read_values, 30,
        &value_count,
        true
    );
    assert_int_equal(ret, 4 + 12 * 10);
    assert_int_equal(value_count, 31);
    assert_int_equal(read_values[0].i, 10);
}

static void test_with_array_early_exit(void **state)
{
    cosc_int32 ret;
    cosc_int32 value_count = 0;
    union cosc_value read_values[31] = {0};
    union cosc_value write_values[31];
    write_values[0].i = 10;
    for (int i = 1; i < 31; i++)
        write_values[i].f = i;
    ret = cosc_write_values(
        buffer, sizeof(buffer),
        ",i[fff]", 1024,
        write_values, 28,
        &value_count
    );
    assert_int_equal(ret, 4 + 12 * 9);
    assert_int_equal(value_count, 28);
    value_count = 0;
    ret = cosc_read_values(
        buffer, sizeof(buffer),
        ",i[fff]", 1024,
        read_values, 28,
        &value_count,
        true
    );
    assert_int_equal(ret, 4 + 12 * 9);
    assert_int_equal(value_count, 28);
    assert_int_equal(read_values[0].i, 10);
}

#endif

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_without_array, func_setup),
#ifndef COSC_NOARRAY
        cmocka_unit_test_setup(test_with_array, func_setup),
        cmocka_unit_test_setup(test_with_array_unfinished, func_setup),
        cmocka_unit_test_setup(test_with_array_early_exit, func_setup),
#endif
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
    return 0;
}
