#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
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
#ifdef COSC_NO64
    {.h = {0x12345678, 0x12345678}},
    {.t = {0x87654321, 0x87654321}},
    {.d = {0x12345678, 0x12345678}},
#else
    {.h = 0x1234567812345678},
    {.t = 0x8765432187654321},
#ifdef COSC_NOFLOAT64
    {.d = 12345678},
#else
    {.d = 1234.5678},
#endif
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
    cosc_int32 value_offsets[sizeof(WRITE_VALUES) / sizeof(*WRITE_VALUES)];
    union cosc_value read_values[11] = {0};
    ret = cosc_write_values(
        buffer, sizeof(buffer),
        WRITE_TYPETAG, WRITE_TYPETAG_LEN,
        WRITE_VALUES, sizeof(WRITE_VALUES) / sizeof(*WRITE_VALUES),
        &value_count, value_offsets
    );
    assert_int_equal(ret, 92);
    assert_int_equal(value_count, 11);
    assert_int_equal(value_offsets[0], 0);
    assert_int_equal(value_offsets[1], 4);
    assert_int_equal(value_offsets[2], 8);
    assert_int_equal(value_offsets[3], 12);
    assert_int_equal(value_offsets[4], 16);
    assert_int_equal(value_offsets[5], 20);
    assert_int_equal(value_offsets[6], 28);
    assert_int_equal(value_offsets[7], 36);
    assert_int_equal(value_offsets[8], 44);
    assert_int_equal(value_offsets[9], 60);
    assert_int_equal(value_offsets[10], 76);
    value_count = 0;
    memset(value_offsets, 0, sizeof(value_offsets));
    ret = cosc_read_values(
        buffer, sizeof(buffer),
        WRITE_TYPETAG, WRITE_TYPETAG_LEN,
        read_values, sizeof(WRITE_VALUES) / sizeof(*WRITE_VALUES),
        &value_count, value_offsets
    );
    assert_int_equal(ret, 92);
    assert_int_equal(value_count, 11);
    assert_int_equal(value_offsets[0], 0);
    assert_int_equal(value_offsets[1], 4);
    assert_int_equal(value_offsets[2], 8);
    assert_int_equal(value_offsets[3], 12);
    assert_int_equal(value_offsets[4], 16);
    assert_int_equal(value_offsets[5], 20);
    assert_int_equal(value_offsets[6], 28);
    assert_int_equal(value_offsets[7], 36);
    assert_int_equal(value_offsets[8], 44);
    assert_int_equal(value_offsets[9], 60);
    assert_int_equal(value_offsets[10], 76);
}

#ifndef COSC_NOARRAY
static void test_with_array(void **state)
{
    cosc_int32 ret;
    cosc_int32 value_count = 0;
    cosc_int32 value_offsets[31];
    union cosc_value read_values[31] = {0};
    union cosc_value write_values[31];
    write_values[0].i = 10;
    for (int i = 1; i < 31; i++)
        write_values[i].f = i;
    ret = cosc_write_values(
        buffer, sizeof(buffer),
        ",i[fff]", 1024,
        write_values, 31,
        &value_count, value_offsets
    );
    assert_int_equal(ret, 4 + 12 * 10);
    assert_int_equal(value_count, 31);
    cosc_int32 offset = 0;
    for (cosc_int32 i = 0; i < value_count; i++)
    {
        assert_int_equal(value_offsets[i], offset);
        offset += 4;
    }
    value_count = 0;
    memset(value_offsets, 0, sizeof(value_offsets));
    ret = cosc_read_values(
        buffer, sizeof(buffer),
        ",i[fff]", 1024,
        read_values, 31,
        &value_count, value_offsets
    );
    assert_int_equal(ret, 4 + 12 * 10);
    assert_int_equal(value_count, 31);
    offset = 0;
    for (cosc_int32 i = 0; i < value_count; i++)
    {
        assert_int_equal(value_offsets[i], offset);
        offset += 4;
    }
    assert_int_equal(read_values[0].i, 10);
}
#endif

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_without_array, func_setup),
#ifndef COSC_NOARRAY
        cmocka_unit_test_setup(test_with_array, func_setup),
#endif
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
    return 0;
}
