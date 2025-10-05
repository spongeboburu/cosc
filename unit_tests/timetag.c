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

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_timetag_to_time),
    };

    // for (int i = 0; i < 1; i++)
    // {
    //     cosc_uint32 nanos = i, tmp;
    //     cosc_uint64 timetag = cosc_timetag_from_time(0, nanos);
    //     cosc_uint32 seconds = cosc_timetag_to_time(timetag, &tmp);
    //     if (nanos != tmp || seconds > 0)
    //         printf("%u != %u (%u)\n", tmp, nanos, seconds);
    // }
    // return 0;
    return cmocka_run_group_tests(tests, NULL, NULL);
}
