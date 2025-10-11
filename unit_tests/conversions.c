#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include "cosc.h"

#ifndef COSC_NOCONVERSION

static void test_64bits_to_uint64(void **state)
{
    struct cosc_64bits bits = {0x12345678, 0x87654321};
    cosc_uint64 out = cosc_64bits_to_uint64(bits);
#ifndef COSC_NOINT64
    assert_int_equal(out, 0x1234567887654321ULL);
#else
    assert_int_equal(out.hi, 0x12345678);
    assert_int_equal(out.lo, 0x87654321);
#endif
}

static void test_64bits_from_uint64(void **state)
{
#ifndef COSC_NOINT64
    cosc_uint64 u64 = 0x1234567887654321ULL;
#else
    cosc_uint64 u64 = {0x12345678, 0x87654321};
#endif
    struct cosc_64bits out = cosc_64bits_from_uint64(u64);
    assert_int_equal(out.hi, 0x12345678);
    assert_int_equal(out.lo, 0x87654321);
}

static void test_64bits_to_int64(void **state)
{
    struct cosc_64bits bits = {0x12345678, 0x87654321};
    cosc_int64 out = cosc_64bits_to_int64(bits);
#ifndef COSC_NOINT64
    assert_int_equal(out, 0x1234567887654321ULL);
#else
    assert_int_equal(out.hi, 0x12345678);
    assert_int_equal(out.lo, 0x87654321);
#endif
}

static void test_64bits_from_int64(void **state)
{
#ifndef COSC_NOINT64
    cosc_int64 u64 = 0x1234567887654321ULL;
#else
    cosc_int64 u64 = {0x12345678, 0x87654321};
#endif
    struct cosc_64bits out = cosc_64bits_from_int64(u64);
    assert_int_equal(out.hi, 0x12345678);
    assert_int_equal(out.lo, 0x87654321);
}

static void test_64bits_to_float64(void **state)
{
    struct cosc_64bits bits = {0x4028ae14, 0x7ae147ae};
    cosc_float64 out = cosc_64bits_to_float64(bits);
#ifndef COSC_NOFLOAT64
    assert_float_equal(out, 12.34, 0.001);
#else
    assert_int_equal(out.hi, 0x4028ae14);
    assert_int_equal(out.lo, 0x7ae147ae);
#endif
}

static void test_64bits_from_float64(void **state)
{
#ifndef COSC_NOFLOAT64
    cosc_float64 u64 = 12.34;
#else
    cosc_float64 u64 = {0x4028ae14, 0x7ae147ae};
#endif
    struct cosc_64bits out = cosc_64bits_from_float64(u64);
    assert_int_equal(out.hi, 0x4028ae14);
    assert_int_equal(out.lo, 0x7ae147ae);
}

static void test_uint32_x_int32(void **state)
{
    cosc_int32 i = -1234;
    cosc_uint32 u = cosc_uint32_from_int32(i);
    assert_int_equal(u, 0xfffffb2e);
    assert_int_equal(cosc_uint32_to_int32(u), -1234);
}

static void test_uint32_x_float32(void **state)
{
#ifndef COSC_NOFLOAT32
    cosc_float32 f = 12.34;
    cosc_uint32 u = cosc_uint32_from_float32(f);
    assert_int_equal(u, 0x414570a4);
    assert_float_equal(cosc_uint32_to_float32(u), 12.34, 0.001);
#else
    cosc_float32 f = 0x414570a4;
    cosc_uint32 u = cosc_uint32_from_float32(f);
    assert_int_equal(u, 0x414570a4);
    assert_int_equal(cosc_uint32_to_float32(u), 0x414570a4);
#endif
}

static void test_uint64_x_int64(void **state)
{
#ifndef COSC_NOINT64
    cosc_int64 i = -1234;
    cosc_uint64 u = cosc_uint64_from_int64(i);
    assert_int_equal(cosc_uint64_to_int64(u), -1234);
#else
    cosc_int64 i = {1234, 5678};
    cosc_uint64 u = cosc_uint64_from_int64(i);
    assert_int_equal(u.hi, 1234);
    assert_int_equal(u.lo, 5678);
    i = cosc_uint64_to_int64(u);
    assert_int_equal(i.hi, 1234);
    assert_int_equal(i.lo, 5678);
#endif
}

static void test_uint64_x_float64(void **state)
{
#ifndef COSC_NOFLOAT64
    cosc_float64 f = 123.456;
    cosc_uint64 u = cosc_uint64_from_float64(f);
    assert_float_equal(cosc_uint64_to_float64(u), 123.456, 0.001);
#else
    cosc_float64 f = {0x405edd2f, 0x1a9fbe77};
    cosc_uint64 u = cosc_uint64_from_float64(f);
    f = cosc_uint64_to_float64(u);
    assert_int_equal(f.hi, 0x405edd2f);
    assert_int_equal(f.lo, 0x1a9fbe77);
#endif
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_64bits_to_uint64),
        cmocka_unit_test(test_64bits_from_uint64),
        cmocka_unit_test(test_64bits_to_int64),
        cmocka_unit_test(test_64bits_from_int64),
        cmocka_unit_test(test_64bits_to_float64),
        cmocka_unit_test(test_64bits_from_float64),
        cmocka_unit_test(test_uint32_x_int32),
        cmocka_unit_test(test_uint32_x_float32),
        cmocka_unit_test(test_uint64_x_int64),
        cmocka_unit_test(test_uint64_x_float64),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

#else /* !COSC_CONVERSION */
#include <stdio.h>
int main(void)
{
    printf("Built without conversion support, skipping test.\n");
    return 0;
}
#endif
