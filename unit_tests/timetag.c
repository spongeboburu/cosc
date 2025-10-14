#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include "cosc.h"

#ifndef COSC_NOTIMETAG

static void test_timetag_to_time(void **state)
{
    cosc_uint64 timetag;
    cosc_uint32 seconds, nanos = 0xffffffff;
#ifdef COSC_NOINT64

    COSC_64BITS_SET(&timetag, 0x12345678, 0);
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 0);
    timetag = cosc_timetag_from_time(seconds, nanos);
    assert_int_equal(COSC_64BITS_HI(&timetag), 0x12345678);
    assert_int_equal(COSC_64BITS_LO(&timetag), 0);
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 0);

    COSC_64BITS_SET(&timetag, 0x12345678, 0x80000000);
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 500000000);
    timetag = cosc_timetag_from_time(seconds, nanos);
    assert_int_equal(COSC_64BITS_HI(&timetag), 0x12345678);
    assert_int_equal(COSC_64BITS_LO(&timetag), 0x80000000);
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0x12345678);
    assert_int_equal(nanos, 500000000);

    COSC_64BITS_SET(&timetag, 0xffffffff, 0xffffffff);
    seconds = cosc_timetag_to_time(timetag, &nanos);
    assert_int_equal(seconds, 0xffffffff);
    assert_int_equal(nanos, 999999999);
    timetag = cosc_timetag_from_time(seconds, nanos);
    assert_int_equal(COSC_64BITS_HI(&timetag), 0xffffffff);
    assert_int_equal(COSC_64BITS_LO(&timetag), 0xfffffffc);
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

static void test_timetag_from_time(void **state)
{
    cosc_uint64 timetag;
#ifndef COSC_NOINT64
    timetag = cosc_timetag_from_time(0, 0);
    assert_int_equal(timetag, 0);
    timetag = cosc_timetag_from_time(10, 500000000);
    assert_int_equal(timetag, 0x0000000a80000000ULL);
    timetag = cosc_timetag_from_time(15, 999999999);
    assert_int_equal(timetag, 0x0000000ffffffffcULL);
#else
    timetag = cosc_timetag_from_time(0, 0);
    assert_int_equal(COSC_64BITS_HI(&timetag), 0);
    assert_int_equal(COSC_64BITS_LO(&timetag), 0);
    timetag = cosc_timetag_from_time(10, 500000000);
    assert_int_equal(COSC_64BITS_HI(&timetag), 0xa);
    assert_int_equal(COSC_64BITS_LO(&timetag), 0x80000000);
    timetag = cosc_timetag_from_time(15, 999999999);
    assert_int_equal(COSC_64BITS_HI(&timetag), 0xf);
    assert_int_equal(COSC_64BITS_LO(&timetag), 0xfffffffcULL);
#endif
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_timetag_to_time),
        cmocka_unit_test(test_timetag_from_time),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

#else /* !COSC_NOREADER && ! COSC_NOSWAP */
#include <stdio.h>
int main(void)
{
    printf("Built without timetag support, skipping test.\n");
    return 0;
}
#endif
