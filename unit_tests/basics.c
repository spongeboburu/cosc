#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include "cosc.h"

static char buffer[1024];

static int func_setup(void **state)
{
    memset(buffer, 0, sizeof(buffer));
    return 0;
}

//
// 32 bit types.
//

static void test_uint32(void **state)
{
    cosc_int32 ret;
    cosc_uint32 value = 0;
    ret = cosc_write_uint32(buffer, sizeof(buffer), 0x87654321);
    assert_int_equal(ret, 4);
    ret = cosc_read_uint32(buffer, sizeof(buffer), &value);
    assert_int_equal(ret, 4);
    assert_int_equal(value, 0x87654321);
}

static void test_uint32_null(void **state)
{
    cosc_int32 ret;
    ret = cosc_write_uint32(NULL, 0, 0x87654321);
    assert_int_equal(ret, 4);
    ret = cosc_read_uint32(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 4);
}

static void test_uint32_overrun(void **state)
{
    cosc_int32 ret;
    cosc_uint32 value = 0;
    ret = cosc_write_uint32(buffer, 3, 0x87654321);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_uint32(buffer, 3, &value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_uint32(buffer, 3, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

static void test_int32(void **state)
{
    cosc_int32 ret;
    cosc_int32 value = 0;
    ret = cosc_write_int32(buffer, sizeof(buffer), 0x12345678);
    assert_int_equal(ret, 4);
    ret = cosc_read_int32(buffer, sizeof(buffer), &value);
    assert_int_equal(ret, 4);
    assert_int_equal(value, 0x12345678);
}

static void test_int32_null(void **state)
{
    cosc_int32 ret;
    ret = cosc_write_int32(NULL, 0, 0x12345678);
    assert_int_equal(ret, 4);
    ret = cosc_read_int32(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 4);
}

static void test_int32_overrun(void **state)
{
    cosc_int32 ret;
    cosc_int32 value = 0;
    ret = cosc_write_int32(buffer, 3, 0x12345678);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_int32(buffer, 3, &value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_int32(buffer, 3, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

#ifndef COSC_NOFLOAT32

static void test_float32(void **state)
{
    cosc_int32 ret;
    cosc_float32 value = 0;
    ret = cosc_write_float32(buffer, sizeof(buffer), 12.34);
    assert_int_equal(ret, 4);
    ret = cosc_read_float32(buffer, sizeof(buffer), &value);
    assert_int_equal(ret, 4);
    assert_float_equal(value, 12.34, 0.001);
}

static void test_float32_null(void **state)
{
    cosc_int32 ret;
    ret = cosc_write_float32(NULL, 0, 12.34);
    assert_int_equal(ret, 4);
    ret = cosc_read_float32(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 4);
}

static void test_float32_overrun(void **state)
{
    cosc_int32 ret;
    cosc_float32 value = 0;
    ret = cosc_write_float32(buffer, 3, 12.34);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_float32(buffer, 3, &value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_float32(buffer, 3, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

#else /* COSC_NOFLOAT32 */

static void test_float32(void **state)
{
    cosc_int32 ret;
    cosc_float32 value = 0;
    ret = cosc_write_float32(buffer, sizeof(buffer), 1234);
    assert_int_equal(ret, 4);
    ret = cosc_read_float32(buffer, sizeof(buffer), &value);
    assert_int_equal(ret, 4);
    assert_int_equal(value, 1234);
}

static void test_float32_null(void **state)
{
    cosc_int32 ret;
    ret = cosc_write_float32(NULL, 0, 1234);
    assert_int_equal(ret, 4);
    ret = cosc_read_float32(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 4);
}

static void test_float32_overrun(void **state)
{
    cosc_int32 ret;
    cosc_float32 value = 0;
    ret = cosc_write_float32(buffer, 3, 1234);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_float32(buffer, 3, &value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_float32(buffer, 3, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

#endif /* COSC_NOFLOAT32 */

static void test_char(void **state)
{
    cosc_int32 ret;
    cosc_int32 value = 0;
    ret = cosc_write_char(buffer, sizeof(buffer), 'A');
    assert_int_equal(ret, 4);
    ret = cosc_read_char(buffer, sizeof(buffer), &value);
    assert_int_equal(ret, 4);
    assert_int_equal(value, 'A');
}

static void test_char_null(void **state)
{
    cosc_int32 ret;
    ret = cosc_write_char(NULL, 0, 'A');
    assert_int_equal(ret, 4);
    ret = cosc_read_char(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 4);
}

static void test_char_overrun(void **state)
{
    cosc_int32 ret;
    cosc_int32 value = 0;
    ret = cosc_write_char(buffer, 3, 'A');
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_char(buffer, 3, &value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_char(buffer, 3, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

static void test_midi(void **state)
{
    cosc_int32 ret;
    const unsigned char input[4] = {1, 2, 3, 4};
    unsigned char value[4] = {0};
    ret = cosc_write_midi(buffer, sizeof(buffer), input);
    assert_int_equal(ret, 4);
    ret = cosc_read_midi(buffer, sizeof(buffer), value);
    assert_memory_equal(value, input, 4);
}

static void test_midi_null(void **state)
{
    cosc_int32 ret;
    const unsigned char input[4] = {1, 2, 3, 4};
    ret = cosc_write_midi(NULL, 0, input);
    assert_int_equal(ret, 4);
    ret = cosc_read_midi(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 4);
}

static void test_midi_overrun(void **state)
{
    cosc_int32 ret;
    const unsigned char input[4] = {1, 2, 3, 4};
    unsigned char value[4] = {0};
    ret = cosc_write_midi(buffer, 3, input);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_midi(buffer, 3, value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_midi(buffer, 3, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

//
// 64 bit types.
//

#ifndef COSC_NOINT64

static void test_uint64(void **state)
{
    cosc_int32 ret;
    cosc_uint64 value = 0;
    ret = cosc_write_uint64(buffer, sizeof(buffer), 0x8765432187654321ULL);
    assert_int_equal(ret, 8);
    ret = cosc_read_uint64(buffer, sizeof(buffer), &value);
    assert_int_equal(ret, 8);
    assert_int_equal(value, 0x8765432187654321ULL);
}

static void test_uint64_null(void **state)
{
    cosc_int32 ret;
    ret = cosc_write_uint64(NULL, 0, 0x8765432187654321ULL);
    assert_int_equal(ret, 8);
    ret = cosc_read_uint64(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 8);
}

static void test_uint64_overrun(void **state)
{
    cosc_int32 ret;
    cosc_uint64 value = 0;
    ret = cosc_write_uint64(buffer, 7, 0x1234567812345678LL);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_uint64(buffer, 7, &value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_uint64(buffer, 7, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

static void test_int64(void **state)
{
    cosc_int32 ret;
    cosc_int64 value = 0;
    ret = cosc_write_int64(buffer, sizeof(buffer), 0x1234567812345678LL);
    assert_int_equal(ret, 8);
    ret = cosc_read_int64(buffer, sizeof(buffer), &value);
    assert_int_equal(ret, 8);
    assert_int_equal(value, 0x1234567812345678LL);
}

static void test_int64_null(void **state)
{
    cosc_int32 ret;
    ret = cosc_write_int64(NULL, 0, 0x1234567812345678LL);
    assert_int_equal(ret, 8);
    ret = cosc_read_int64(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 8);
}

static void test_int64_overrun(void **state)
{
    cosc_int32 ret;
    cosc_int64 value = 0;
    ret = cosc_write_int64(buffer, 7, 0x1234567812345678LL);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_int64(buffer, 7, &value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_int64(buffer, 7, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

#else /* !COSC_NOINT64 */

static void test_uint64(void **state)
{
    cosc_int32 ret;
    cosc_uint64 input = COSC_64BITS_INIT(0x87654321, 0x87654321);
    cosc_uint64 value = COSC_64BITS_INIT(0, 0);
    ret = cosc_write_uint64(buffer, sizeof(buffer), input);
    assert_int_equal(ret, 8);
    ret = cosc_read_uint64(buffer, sizeof(buffer), &value);
    assert_int_equal(ret, 8);
    assert_int_equal(COSC_64BITS_HI(&value), 0x87654321);
    assert_int_equal(COSC_64BITS_LO(&value), 0x87654321);
}

static void test_uint64_null(void **state)
{
    cosc_int32 ret;
    cosc_uint64 input = COSC_64BITS_INIT(0x87654321, 0x87654321);
    ret = cosc_write_uint64(NULL, 0, input);
    assert_int_equal(ret, 8);
    ret = cosc_read_uint64(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 8);
}

static void test_uint64_overrun(void **state)
{
    cosc_int32 ret;
    cosc_uint64 input = COSC_64BITS_INIT(0x87654321, 0x87654321);
    cosc_uint64 value = COSC_64BITS_INIT(0, 0);
    ret = cosc_write_uint64(buffer, 7, input);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_uint64(buffer, 7, &value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_uint64(buffer, 7, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

static void test_int64(void **state)
{
    cosc_int32 ret;
    cosc_uint64 input = COSC_64BITS_INIT(0x12345678, 0x12345678);
    cosc_int64 value = COSC_64BITS_INIT(0, 0);
    ret = cosc_write_int64(buffer, sizeof(buffer), input);
    assert_int_equal(ret, 8);
    ret = cosc_read_int64(buffer, sizeof(buffer), &value);
    assert_int_equal(ret, 8);
    assert_int_equal(COSC_64BITS_HI(&value), 0x12345678);
    assert_int_equal(COSC_64BITS_LO(&value), 0x12345678);
}

static void test_int64_null(void **state)
{
    cosc_int32 ret;
    cosc_uint64 input = COSC_64BITS_INIT(0x12345678, 0x12345678);
    ret = cosc_write_int64(NULL, 0, input);
    assert_int_equal(ret, 8);
    ret = cosc_read_int64(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 8);
}

static void test_int64_overrun(void **state)
{
    cosc_int32 ret;
    cosc_uint64 input = COSC_64BITS_INIT(0x87654321, 0x87654321);
    cosc_int64 value = COSC_64BITS_INIT(0, 0);
    ret = cosc_write_int64(buffer, 7, input);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_int64(buffer, 7, &value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_int64(buffer, 7, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

#endif /* !COSC_NOINT64 */

#ifndef COSC_NOFLOAT64

static void test_float64(void **state)
{
    cosc_int32 ret;
    cosc_float64 value = 0;
    ret = cosc_write_float64(buffer, sizeof(buffer), 1234.5678);
    assert_int_equal(ret, 8);
    ret = cosc_read_float64(buffer, sizeof(buffer), &value);
    assert_int_equal(ret, 8);
    assert_float_equal(value, 1234.5678, 0.00001);
}

static void test_float64_null(void **state)
{
    cosc_int32 ret;
    ret = cosc_write_float64(NULL, 0, 1234.5678);
    assert_int_equal(ret, 8);
    ret = cosc_read_float64(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 8);
}

static void test_float64_overrun(void **state)
{
    cosc_int32 ret;
    cosc_float64 value = 0;
    ret = cosc_write_float64(buffer, 7, 1234.5678);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_float64(buffer, 7, &value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_float64(buffer, 7, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

#else /* !COSC_NOFLOAT64 */

static void test_float64(void **state)
{
    cosc_int32 ret;
    cosc_float64 value = COSC_64BITS_INIT(0, 0);
    cosc_float64 input = COSC_64BITS_INIT(0x1234, 0x5678);
    ret = cosc_write_float64(buffer, sizeof(buffer), input);
    assert_int_equal(ret, 8);
    ret = cosc_read_float64(buffer, sizeof(buffer), &value);
    assert_int_equal(ret, 8);
    assert_int_equal(COSC_64BITS_HI(&value), 0x1234);
    assert_int_equal(COSC_64BITS_LO(&value), 0x5678);
}

static void test_float64_null(void **state)
{
    cosc_int32 ret;
    cosc_float64 input = COSC_64BITS_INIT(0x1234, 0x5678);
    ret = cosc_write_float64(NULL, 0, input);
    assert_int_equal(ret, 8);
    ret = cosc_read_float64(buffer, sizeof(buffer), NULL);
    assert_int_equal(ret, 8);
}

static void test_float64_overrun(void **state)
{
    cosc_int32 ret;
    cosc_float64 value = COSC_64BITS_INIT(0, 0);
    cosc_float64 input = COSC_64BITS_INIT(0x1234, 0x5678);
    ret = cosc_write_float64(buffer, 7, input);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_float64(buffer, 7, &value);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_float64(buffer, 7, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

#endif /* COSC_NOFLOAT64 */

static void test_string(void **state)
{
    cosc_int32 ret;
    cosc_int32 len = -1;
    ret = cosc_write_string(buffer, sizeof(buffer), "", 1024, 0);
    assert_int_equal(ret, 4);
    ret = cosc_read_string(buffer, sizeof(buffer), 0, 0, &len);
    assert_int_equal(ret, 4);
    assert_int_equal(len, 0);
    ret = cosc_write_string(buffer, sizeof(buffer), "a", 1024, 0);
    assert_int_equal(ret, 4);
    ret = cosc_read_string(buffer, sizeof(buffer), 0, 0, &len);
    assert_int_equal(ret, 4);
    assert_int_equal(len, 1);
    ret = cosc_write_string(buffer, sizeof(buffer), "ab", 1024, 0);
    assert_int_equal(ret, 4);
    ret = cosc_read_string(buffer, sizeof(buffer), 0, 0, &len);
    assert_int_equal(ret, 4);
    assert_int_equal(len, 2);
    ret = cosc_write_string(buffer, sizeof(buffer), "abc", 1024, 0);
    assert_int_equal(ret, 4);
    ret = cosc_read_string(buffer, sizeof(buffer), 0, 0, &len);
    assert_int_equal(ret, 4);
    assert_int_equal(len, 3);
    ret = cosc_write_string(buffer, sizeof(buffer), "abcd", 1024, 0);
    assert_int_equal(ret, 8);
    ret = cosc_read_string(buffer, sizeof(buffer), 0, 0, &len);
    assert_int_equal(ret, 8);
    assert_int_equal(len, 4);
    ret = cosc_write_string(buffer, sizeof(buffer), "abcd", 2, 0);
    assert_int_equal(ret, 4);
    ret = cosc_read_string(buffer, sizeof(buffer), 0, 0, &len);
    assert_int_equal(ret, 4);
    assert_int_equal(len, 2);
}

static void test_string_null(void **state)
{
    cosc_int32 ret;
    cosc_int32 len = -1;
    ret = cosc_write_string(buffer, sizeof(buffer), "abcd", 1024, 0);
    assert_int_equal(ret, 8);
    ret = cosc_read_string(buffer, sizeof(buffer), 0, 0, NULL);
    assert_int_equal(ret, 8);
    ret = cosc_write_string(buffer, sizeof(buffer), NULL, 1024, 0);
    assert_int_equal(ret, 4);
    ret = cosc_read_string(buffer, sizeof(buffer), 0, 0, &len);
    assert_int_equal(ret, 4);
    assert_int_equal(len, 0);
}

static void test_string_overrun(void **state)
{
    cosc_int32 ret;
    cosc_int32 len = -1;
    ret = cosc_write_string(buffer, 7, "abcd", 1024, 0);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_string(buffer, 7, 0, 0, &len);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_string(buffer, 7, 0, 0, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

static void test_blob(void **state)
{
    cosc_int32 ret;
    cosc_int32 psize = -1;
    const unsigned char input[4] = {1, 2, 3, 4};
    const void *output = "BLEH";
    ret = cosc_write_blob(buffer, sizeof(buffer), input, 0);
    assert_int_equal(ret, 4);
    ret = cosc_read_blob(buffer, sizeof(buffer), 0, 0, &output, &psize);
    assert_int_equal(ret, 4);
    assert_int_equal(psize, 0);
    assert_null(output);
    ret = cosc_write_blob(buffer, sizeof(buffer), input, 1);
    assert_int_equal(ret, 8);
    ret = cosc_read_blob(buffer, sizeof(buffer), 0, 0, &output, &psize);
    assert_int_equal(ret, 8);
    assert_int_equal(psize, 1);
    assert_non_null(output);
    assert_memory_equal(output, input, 1);
    ret = cosc_write_blob(buffer, sizeof(buffer), input, 2);
    assert_int_equal(ret, 8);
    ret = cosc_read_blob(buffer, sizeof(buffer), 0, 0, &output, &psize);
    assert_int_equal(ret, 8);
    assert_int_equal(psize, 2);
    assert_non_null(output);
    assert_memory_equal(output, input, 2);
    ret = cosc_write_blob(buffer, sizeof(buffer), input, 3);
    assert_int_equal(ret, 8);
    ret = cosc_read_blob(buffer, sizeof(buffer), 0, 0, &output, &psize);
    assert_int_equal(ret, 8);
    assert_int_equal(psize, 3);
    assert_non_null(output);
    assert_memory_equal(output, input, 3);
    ret = cosc_write_blob(buffer, sizeof(buffer), input, 4);
    assert_int_equal(ret, 8);
    ret = cosc_read_blob(buffer, sizeof(buffer), 0, 0, &output, &psize);
    assert_int_equal(ret, 8);
    assert_int_equal(psize, 4);
    assert_non_null(output);
    assert_memory_equal(output, input, 4);
}

static void test_blob_null(void **state)
{
    cosc_int32 ret;
    cosc_int32 psize = -1;
    const void *output = NULL;
    ret = cosc_write_blob(buffer, sizeof(buffer), NULL, 1);
    assert_int_equal(ret, 8);
    ret = cosc_read_blob(buffer, sizeof(buffer), 0, 0, &output, &psize);
    assert_int_equal(ret, 8);
    assert_int_equal(psize, 1);
    assert_non_null(output);
    assert_int_equal(((const char *)buffer)[4], 0);
    ret = cosc_read_blob(buffer, sizeof(buffer), 0, 0, NULL, NULL);
    assert_int_equal(ret, 8);
}

static void test_blob_overrun(void **state)
{
    cosc_int32 ret;
    cosc_int32 psize = -1;
    const unsigned char input[4] = {1, 2, 3, 4};
    const void *output = "BLEH";
    cosc_write_int32(buffer, 4, 4);
    ret = cosc_write_blob(buffer, 7, input, 4);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_blob(buffer, 7, 0, 0, &output, &psize);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_write_blob(buffer, 7, NULL, 4);
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_blob(buffer, 7, 0, 0, NULL, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
    cosc_write_int32(buffer, 4, 2048);
    ret = cosc_read_blob(buffer, 1024, 0, 0, NULL, NULL);
    assert_int_equal(ret, COSC_EOVERRUN);
}

static void test_blob_psize(void **state)
{
    cosc_int32 ret;
    cosc_int32 psize = -1;
    const void *output = "BLEH";
    cosc_write_int32(buffer, 4, -1);
    ret = cosc_read_blob(buffer, sizeof(buffer), 0, 0, &output, &psize);
    assert_int_equal(ret, COSC_EPSIZE);
    cosc_write_int32(buffer, 4, COSC_SIZE_MAX);
    ret = cosc_read_blob(buffer, sizeof(buffer), 0, 0, &output, &psize);
    assert_int_equal(ret, COSC_ESIZEMAX);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_uint32, func_setup),
        cmocka_unit_test_setup(test_uint32_null, func_setup),
        cmocka_unit_test_setup(test_uint32_overrun, func_setup),
        cmocka_unit_test_setup(test_int32, func_setup),
        cmocka_unit_test_setup(test_int32_null, func_setup),
        cmocka_unit_test_setup(test_int32_overrun, func_setup),
        cmocka_unit_test_setup(test_float32, func_setup),
        cmocka_unit_test_setup(test_float32_null, func_setup),
        cmocka_unit_test_setup(test_float32_overrun, func_setup),
        cmocka_unit_test_setup(test_char, func_setup),
        cmocka_unit_test_setup(test_char_null, func_setup),
        cmocka_unit_test_setup(test_char_overrun, func_setup),
        cmocka_unit_test_setup(test_midi, func_setup),
        cmocka_unit_test_setup(test_midi_null, func_setup),
        cmocka_unit_test_setup(test_midi_overrun, func_setup),
        
        cmocka_unit_test_setup(test_uint64, func_setup),
        cmocka_unit_test_setup(test_uint64_null, func_setup),
        cmocka_unit_test_setup(test_uint64_overrun, func_setup),
        cmocka_unit_test_setup(test_int64, func_setup),
        cmocka_unit_test_setup(test_int64_null, func_setup),
        cmocka_unit_test_setup(test_int64_overrun, func_setup),
        cmocka_unit_test_setup(test_float64, func_setup),
        cmocka_unit_test_setup(test_float64_null, func_setup),
        cmocka_unit_test_setup(test_float64_overrun, func_setup),

        cmocka_unit_test_setup(test_string, func_setup),
        cmocka_unit_test_setup(test_string_null, func_setup),
        cmocka_unit_test_setup(test_string_overrun, func_setup),
        cmocka_unit_test_setup(test_blob, func_setup),
        cmocka_unit_test_setup(test_blob_null, func_setup),
        cmocka_unit_test_setup(test_blob_overrun, func_setup),
        cmocka_unit_test_setup(test_blob_psize, func_setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
