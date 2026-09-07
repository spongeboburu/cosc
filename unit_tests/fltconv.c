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

static cosc_float64 float64_from_bits(cosc_uint32 hi, cosc_uint32 lo)
{
#ifdef COSC_NOFLOAT64
    struct cosc_64bits bits = COSC_64BITS_INIT(hi, lo);
    return bits;
#else
    struct cosc_64bits bits = COSC_64BITS_INIT(hi, lo);
    cosc_float64 value;
    if (!cosc_big_endian())
    {
        cosc_uint32 tmp = COSC_64BITS_HI(&bits);
        COSC_64BITS_HI(&bits) = COSC_64BITS_LO(&bits);
        COSC_64BITS_LO(&bits) = tmp;
    }
    memcpy(&value, &bits, sizeof(value));
    return value;
#endif
}

static cosc_uint32 float32_bits(cosc_float32 value)
{
    cosc_uint32 bits;
    memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static struct cosc_64bits float64_bits(cosc_float64 value)
{
#ifdef COSC_NOFLOAT64
    return value;
#else
    struct cosc_64bits bits;
    memcpy(&bits, &value, sizeof(bits));
    if (!cosc_big_endian())
    {
        cosc_uint32 tmp = COSC_64BITS_HI(&bits);
        COSC_64BITS_HI(&bits) = COSC_64BITS_LO(&bits);
        COSC_64BITS_LO(&bits) = tmp;
    }
    return bits;
#endif
}

static void test_float64_to_float32_subnormal(void **state)
{
    static const struct {
        cosc_uint32 hi;
        cosc_uint32 lo;
        cosc_uint32 expected;
    } values[] = {
        /*  2^-127, 2^-149, and the halfway case 2^-150. */
        {0x38000000, 0x00000000, 0x00400000},
        {0x36a00000, 0x00000000, 0x00000001},
        {0x36900000, 0x00000000, 0x00000000},
        /*  3 * 2^-150 is halfway between float32 1 and 2. */
        {0x36a80000, 0x00000000, 0x00000002},
        /* Halfway between the largest subnormal and smallest normal. */
        {0x380fffff, 0xf0000000, 0x00800000},
        /* The same cases with the sign bit set. */
        {0xb8000000, 0x00000000, 0x80400000},
        {0xb6a00000, 0x00000000, 0x80000001},
    };

    (void)state;
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        cosc_float64 input = float64_from_bits(values[i].hi, values[i].lo);
        cosc_float32 output = cosc_float64_to_float32(input);
        assert_int_equal(float32_bits(output), values[i].expected);
    }
}

static void test_float32_to_float64_bits(void **state)
{
    static const struct {
        cosc_uint32 input;
        cosc_uint32 expected_hi;
        cosc_uint32 expected_lo;
    } values[] = {
        {0x00000001, 0x36a00000, 0x00000000},
        {0x00400000, 0x38000000, 0x00000000},
        {0x007fffff, 0x380fffff, 0xc0000000},
        {0x80000001, 0xb6a00000, 0x00000000},
        {0x7fc00001, 0x7ff80000, 0x20000000},
    };

    (void)state;
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        cosc_float32 input;
        memcpy(&input, &values[i].input, sizeof(input));
        struct cosc_64bits output = float64_bits(cosc_float32_to_float64(input));
        assert_int_equal(COSC_64BITS_HI(&output), values[i].expected_hi);
        assert_int_equal(COSC_64BITS_LO(&output), values[i].expected_lo);
    }
}

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
        cmocka_unit_test(test_float64_to_float32_subnormal),
        cmocka_unit_test(test_float32_to_float64_bits),
        cmocka_unit_test(test_float32_to_float64_static),
        cmocka_unit_test(test_float32_to_float64_range_big),
        cmocka_unit_test(test_float32_to_float64_range_small),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
