#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <math.h>
#include "cosc.h"

#define NUM_STATIC_TESTS 12

static const double d[NUM_STATIC_TESTS] = {
    0.0, -0.0, 1.0, -1.0,
    12.34, -12.34, 0.01234, -0.01234,
    INFINITY, -INFINITY, FP_NAN, -FP_NAN
};

static const float f[NUM_STATIC_TESTS] = {
    d[0], d[1], d[2], d[3],
    d[4], d[5], d[6], d[7],
    d[8], d[9], d[10], d[11],
};

static void test_float64_to_float32_static(void **state)
{
    float value;
    for (int i = 0; i < NUM_STATIC_TESTS; i++)
    {
        cosc_float64 input;
        memcpy(&input, d + i, 8);
#ifdef COSC_NOFLOAT64
        if (!cosc_big_endian())
        {
            cosc_uint32 tmp = COSC_64BITS_HI(&input);
            COSC_64BITS_HI(&input) = COSC_64BITS_LO(&input);
            COSC_64BITS_LO(&input) = tmp;
        }
#endif
        cosc_float32 output = cosc_float64_to_float32(input);
        memcpy(&value, &output, 4);
        assert_float_equal(value, f[i], 0.001);
    }
}

static void test_float64_to_float32_range_big(void **state)
{
    for (int i = 0; i <= 128; i++)
    {
        double value = i - 64;
        value *= 100;
        cosc_float64 input;
        memcpy(&input, &value, 8);
#ifdef COSC_NOFLOAT64
        if (!cosc_big_endian())
        {
            cosc_uint32 tmp = COSC_64BITS_HI(&input);
            COSC_64BITS_HI(&input) = COSC_64BITS_LO(&input);
            COSC_64BITS_LO(&input) = tmp;
        }
#endif
        cosc_float32 output = cosc_float64_to_float32(input);
        float testvalue;
        memcpy(&testvalue, &output, 4);
        assert_float_equal(testvalue, (float)value, 0.001);
    }
}

static void test_float64_to_float32_range_small(void **state)
{
    for (int i = 0; i <= 128; i++)
    {
        double value = i - 64;
        value /= 100;
        cosc_float64 input;
        memcpy(&input, &value, 8);
#ifdef COSC_NOFLOAT64
        if (!cosc_big_endian())
        {
            cosc_uint32 tmp = COSC_64BITS_HI(&input);
            COSC_64BITS_HI(&input) = COSC_64BITS_LO(&input);
            COSC_64BITS_LO(&input) = tmp;
        }
#endif
        cosc_float32 output = cosc_float64_to_float32(input);
        float testvalue;
        memcpy(&testvalue, &output, 4);
        assert_float_equal(testvalue, (float)value, 0.001);
    }
}

static void test_float32_to_float64_static(void **state)
{
    double value;
    for (int i = 0; i < NUM_STATIC_TESTS; i++)
    {
        cosc_float32 input;
        memcpy(&input, f + i, 4);
        cosc_float64 output = cosc_float32_to_float64(input);
#ifdef COSC_NOFLOAT64
        if (!cosc_big_endian())
        {
            cosc_uint32 tmp = COSC_64BITS_HI(&output);
            COSC_64BITS_HI(&output) = COSC_64BITS_LO(&output);
            COSC_64BITS_LO(&output) = tmp;
        }
#endif
        memcpy(&value, &output, 8);
        assert_float_equal((double)value, d[i], 0.00001);
    }
}

static void test_float32_to_float64_range_big(void **state)
{
    for (int i = 0; i <= 128; i++)
    {
        float value = i - 64;
        value *= 100;
        cosc_float32 input;
        memcpy(&input, &value, 4);
        cosc_float64 output = cosc_float32_to_float64(input);
#ifdef COSC_NOFLOAT64
        if (!cosc_big_endian())
        {
            cosc_uint32 tmp = COSC_64BITS_HI(&output);
            COSC_64BITS_HI(&output) = COSC_64BITS_LO(&output);
            COSC_64BITS_LO(&output) = tmp;
        }
#endif
        double testvalue;
        memcpy(&testvalue, &output, 8);
        assert_float_equal(testvalue, (double)value, 0.00001);
    }
}

static void test_float32_to_float64_range_small(void **state)
{
    for (int i = 0; i <= 128; i++)
    {
        float value = i - 64;
        value /= 100;
        cosc_float32 input;
        memcpy(&input, &value, 4);
        cosc_float64 output = cosc_float32_to_float64(input);
#ifdef COSC_NOFLOAT64
        if (!cosc_big_endian())
        {
            cosc_uint32 tmp = COSC_64BITS_HI(&output);
            COSC_64BITS_HI(&output) = COSC_64BITS_LO(&output);
            COSC_64BITS_LO(&output) = tmp;
        }
#endif
        double testvalue;
        memcpy(&testvalue, &output, 8);
        assert_float_equal(testvalue, (double)value, 0.00001);
    }
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_float64_to_float32_static),
        cmocka_unit_test(test_float64_to_float32_range_big),
        cmocka_unit_test(test_float64_to_float32_range_small),
        cmocka_unit_test(test_float32_to_float64_static),
        cmocka_unit_test(test_float32_to_float64_range_big),
        cmocka_unit_test(test_float32_to_float64_range_small),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
