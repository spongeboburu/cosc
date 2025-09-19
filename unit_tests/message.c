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
    {.s = {"Hello World!", 1024}},
    {.s = {"Hello World!", 1024}},
    {.b = {"Hello World!", 12}},
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
};

static const struct cosc_message WRITE_MESSAGE =
{
    "/hello", 6,
    ",ifrcmsSbhtdTFNI", 16,
    {.write = WRITE_VALUES}, 11
};

static int func_setup(void **state)
{
    memset(buffer, 0, sizeof(buffer));
    return 0;
}

static void test_message_nopsize(void **state)
{
    cosc_int32 ret;
    cosc_int32 value_count = 0;
    cosc_int32 value_offsets[11] = {0};
    ret = cosc_write_message(
        buffer, sizeof(buffer),
        &WRITE_MESSAGE, 0,
        &value_count, value_offsets
    );
    assert_int_equal(ret, 120);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_message_nopsize, func_setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
