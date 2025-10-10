#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include "cosc.h"

static void test_timetag_to_time(void **state)
{
    cosc_uint64 timetag;
    cosc_uint32 seconds, nanos = 0xffffffff;
#ifdef COSC_NOINT64

    timetag.hi = 0x12345678;
    timetag.lo = 0;
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 0);
    timetag = cosc_timetag_from_time(seconds, nanos);
    assert_int_equal(timetag.hi, 0x12345678);
    assert_int_equal(timetag.lo, 0);
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 0);

    timetag.hi = 0x12345678;
    timetag.lo = 0x80000000ULL;
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 500000000);
    timetag = cosc_timetag_from_time(seconds, nanos);
    assert_int_equal(timetag.hi, 0x12345678);
    assert_int_equal(timetag.lo, 0x80000000);
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 500000000);

    timetag.hi = 0xffffffff;
    timetag.lo = 0xffffffff;
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0xffffffff);
    assert_int_equal(nanos, 999999999);
    timetag = cosc_timetag_from_time(seconds, nanos);
    assert_int_equal(timetag.hi, 0xffffffff);
    assert_int_equal(timetag.lo, 0xfffffffc);
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0xffffffff);
    assert_int_equal(nanos, 999999999);

    for (cosc_uint32 i = 0; i < 9999; i++)
    {
        cosc_uint32 tmp = i * 100000;
        timetag = cosc_timetag_from_time(0, tmp);
        cosc_timetag_to_time(timetag, &nanos);
        assert_int_equal(nanos, tmp);
    }
#else

    timetag = 0x1234567800000000ULL;
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 0);
    timetag = cosc_timetag_from_time(seconds, nanos);
    assert_int_equal(timetag, 0x1234567800000000ULL);
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 0);

    timetag = 0x1234567880000000ULL;
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 500000000);
    timetag = cosc_timetag_from_time(seconds, nanos);
    assert_int_equal(timetag, 0x1234567880000000ULL);
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 500000000);

    timetag = 0xffffffffffffffffULL;
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0xffffffff);
    assert_int_equal(nanos, 999999999);
    timetag = cosc_timetag_from_time(seconds, nanos);
    assert_int_equal(timetag, 0xfffffffffffffffcULL);
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0xffffffff);
    assert_int_equal(nanos, 999999999);

    for (cosc_uint32 i = 0; i < 9999; i++)
    {
        cosc_uint32 tmp = i * 100000;
        timetag = cosc_timetag_from_time(0, tmp);
        cosc_timetag_to_time(timetag, &nanos);
        assert_int_equal(nanos, tmp);
    }
#endif
}

// FIXME: time to timetag?

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

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_timetag_to_time),
        cmocka_unit_test(test_64bits_to_uint64),
        cmocka_unit_test(test_64bits_from_uint64),
        cmocka_unit_test(test_64bits_to_int64),
        cmocka_unit_test(test_64bits_from_int64),
        cmocka_unit_test(test_64bits_to_float64),
        cmocka_unit_test(test_64bits_from_float64),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
