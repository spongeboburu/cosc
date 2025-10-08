#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include "cosc.h"

#ifndef COSC_NOWRITER

static struct cosc_writer writer;
static unsigned char buffer[1024];
static struct cosc_level levels[4];
static const cosc_int32 level_max = 4;

void dump_buffer(
    const struct cosc_writer *writer
)
{
    cosc_int32 sz = cosc_writer_get_size(writer);
    printf("%d\n", sz);
    cosc_int32 offset = 0;
    printf("static const unsigned char bytes[%d] = {\n", sz);
    while (offset < sz)
    {
        if (writer->buffer[offset] == '\\')
            printf("     '\\\\'");
        else if (writer->buffer[offset] >= 32 && writer->buffer[offset] < 128)
            printf("     '%c'", (char)writer->buffer[offset]);
        else
            printf("    0x%02x", writer->buffer[offset]);
        offset++;
        for (cosc_int32 i = 1; offset < sz && i < 8; i++)
        {
            if (writer->buffer[offset] == '\\')
                printf(", '\\\\'");
            else if (writer->buffer[offset] >= 32 && writer->buffer[offset] < 128)
                printf(", '%c'", (char)writer->buffer[offset]);
            else
                printf(", 0x%02x", writer->buffer[offset]);
            offset++;
        }
        printf(",\n");
    }
    printf("};\n");
}

static int func_setup(void **state)
{
    return 0;
}

static int func_teardown(void **state)
{
    return 0;
}

static void test_empty_bundle_noprefix(void **state)
{
#ifndef COSC_NOINT64
    cosc_uint64 timetag = 0x12345678;
#else
    cosc_uint64 timetag = {0, 0x12345678};
#endif
    cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, 0);
    assert_int_equal(cosc_writer_open_bundle(&writer, timetag), 16);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 16);
    assert_int_equal(cosc_writer_open_bundle(&writer, timetag), COSC_EPREFIXFLAG);
}

static void test_empty_bundle_prefix(void **state)
{
#ifndef COSC_NOINT64
    cosc_uint64 timetag = 0x12345678;
#else
    cosc_uint64 timetag = {0, 0x12345678};
#endif
    cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_writer_open_bundle(&writer, timetag), 20);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 20);
    assert_int_equal(cosc_writer_open_bundle(&writer, timetag), 20);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 40);
}

static void test_empty_message_noprefix(void **state)
{
    cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, 0);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), 8);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 8);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), COSC_EPREFIXFLAG);
}

static void test_empty_message_prefix(void **state)
{
    cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), 12);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 12);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), 12);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 24);
}

static void test_bundle_empty_messages_noprefix(void **state)
{
#ifndef COSC_NOINT64
    cosc_uint64 timetag = 0x12345678;
#else
    cosc_uint64 timetag = {0, 0x12345678};
#endif
    cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, 0);
    assert_int_equal(cosc_writer_open_bundle(&writer, timetag), 16);
    assert_int_equal(cosc_writer_get_size(&writer), 16);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), 12);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 16 + 12);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), 12);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 16 + 24);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), 12);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 16 + 36);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), 12);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 16 + 48);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 16 + 48);
    assert_int_equal(cosc_writer_open_bundle(&writer, timetag), COSC_EPREFIXFLAG);
}

static void test_bundle_empty_messages_prefix(void **state)
{
#ifndef COSC_NOINT64
    cosc_uint64 timetag = 0x12345678;
#else
    cosc_uint64 timetag = {0, 0x12345678};
#endif
    cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_writer_open_bundle(&writer, timetag), 20);
    assert_int_equal(cosc_writer_get_size(&writer), 20);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), 12);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 20 + 12);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), 12);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 20 + 24);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), 12);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 20 + 36);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",", 4), 12);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 20 + 48);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 20 + 48);
    assert_int_equal(cosc_writer_open_bundle(&writer, timetag), 20);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 40 + 48);
}

static void test_message_noarray(void **state)
{
#ifndef COSC_NOINT64
    cosc_int64 i64 = 0x12345678;
    cosc_uint64 u64 = 0x12345678;
#else
    cosc_int64 i64 = {0, 0x12345678};
    cosc_uint64 u64 = {0, 0x12345678};
#endif
#if defined(COSC_NOINT64)  && defined(COSC_NOFLOAT64)
    cosc_float64 f64 = {0, 1234};
#else
    cosc_float64 f64 = 0;
#endif
    cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",ifsbhtdScrmTFNI", 1024), 28);
    assert_int_equal(cosc_writer_get_size(&writer), 28);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'i');
    assert_int_equal(cosc_writer_int32(&writer, 0x87654321), 4);
    assert_int_equal(cosc_writer_get_size(&writer), 32);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'f');
    assert_int_equal(cosc_writer_float32(&writer, 1234), 4);
    assert_int_equal(cosc_writer_get_size(&writer), 36);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 's');
    assert_int_equal(cosc_writer_string(&writer, "abc", 4, 0), 4);
    assert_int_equal(cosc_writer_get_size(&writer), 40);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'b');
    assert_int_equal(cosc_writer_blob(&writer, "abc", 4), 8);
    assert_int_equal(cosc_writer_get_size(&writer), 48);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'h');
    assert_int_equal(cosc_writer_int64(&writer, i64), 8);
    assert_int_equal(cosc_writer_get_size(&writer), 56);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 't');
    assert_int_equal(cosc_writer_uint64(&writer, u64), 8);
    assert_int_equal(cosc_writer_get_size(&writer), 64);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'd');
    assert_int_equal(cosc_writer_float64(&writer, f64), 8);
    assert_int_equal(cosc_writer_get_size(&writer), 72);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'S');
    assert_int_equal(cosc_writer_string(&writer, "abc", 4, 0), 4);
    assert_int_equal(cosc_writer_get_size(&writer), 76);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'c');
    assert_int_equal(cosc_writer_char(&writer, 'c'), 4);
    assert_int_equal(cosc_writer_get_size(&writer), 80);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'r');
    assert_int_equal(cosc_writer_uint32(&writer, 0x87654321), 4);
    assert_int_equal(cosc_writer_get_size(&writer), 84);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'm');
    unsigned char midi[4] = {1, 2, 3, 4};
    assert_int_equal(cosc_writer_midi(&writer, midi), 4);
    assert_int_equal(cosc_writer_get_size(&writer), 88);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'T');
    assert_int_equal(cosc_writer_skip(&writer), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 88);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'F');
    assert_int_equal(cosc_writer_skip(&writer), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 88);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'N');
    assert_int_equal(cosc_writer_skip(&writer), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 88);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'I');
    assert_int_equal(cosc_writer_skip(&writer), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 88);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 0);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 88);
    assert_int_equal(cosc_writer_close(&writer, 0), 0);
}

#ifndef COSC_NOARRAY
static void test_message_array(void **state)
{
    cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",i[fff]", 1024), 16);
    assert_int_equal(cosc_writer_get_size(&writer), 16);
    assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'i');
    assert_int_equal(cosc_writer_int32(&writer, 10), 4);
    assert_int_equal(cosc_writer_get_size(&writer), 20);
    cosc_int32 sz = cosc_writer_get_size(&writer);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            assert_int_equal(cosc_writer_get_level_msgtype(&writer), 'f');
            assert_int_equal(cosc_writer_float32(&writer, 1.0), 4);
            sz += 4;
            assert_int_equal(cosc_writer_get_size(&writer), sz);
        }
    }
    assert_int_equal(cosc_writer_close(&writer, 0), 0);
}
#endif

static void test_message_unfinished_noarray(void **state)
{
    cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",ifsbhtdScrmTFNI", 1024), 28);
    assert_int_equal(cosc_writer_close(&writer, 1), 56);
    assert_int_equal(cosc_writer_get_size(&writer), 84);
}

#ifndef COSC_NOARRAY
static void test_message_unfinished_array(void **state)
{
    cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",i[fff]", 1024), 16);
    assert_int_equal(cosc_writer_close(&writer, 1), 16);
    assert_int_equal(cosc_writer_get_size(&writer), 32);
}
#endif

static void test_blob_unfinished(void **state)
{
    cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",b", 1024), 12);
    assert_int_equal(cosc_writer_open_blob(&writer), 4);
    assert_int_equal(cosc_writer_bytes(&writer, "abcd", 4), 4);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 20);
    cosc_writer_reset(&writer);
    assert_int_equal(cosc_writer_open_message(&writer, "abc", 4, ",b", 1024), 12);
    assert_int_equal(cosc_writer_open_blob(&writer), 4);
    assert_int_equal(cosc_writer_bytes(&writer, "ab", 2), 2);
    assert_int_equal(cosc_writer_close(&writer, 1), 2);
    assert_int_equal(cosc_writer_close(&writer, 1), 0);
    assert_int_equal(cosc_writer_get_size(&writer), 20);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_empty_bundle_noprefix, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_empty_bundle_prefix, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_empty_message_noprefix, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_empty_message_prefix, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_bundle_empty_messages_noprefix, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_bundle_empty_messages_prefix, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_message_noarray, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_message_unfinished_noarray, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_blob_unfinished, func_setup, func_teardown),
#ifndef COSC_NOARRAY
        cmocka_unit_test_setup_teardown(test_message_array, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_message_unfinished_array, func_setup, func_teardown),
#endif
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
    return 0;

    // cosc_int32 ret;
    // cosc_writer_setup(&writer, buffer, sizeof(buffer), levels, level_max, 0);
    // ret = cosc_writer_open_message(&writer, "abc", 16, ",[if]", 16);
    // printf("* %d\n", ret);
    // ret = cosc_writer_skip(&writer);
    // printf("* %d\n", ret);
    // ret = cosc_writer_skip(&writer);
    // printf("* %d\n", ret);
    // ret = cosc_writer_close(&writer, 1);
    // printf("* %d\n", ret);
}

#else /* !COSC_NOWRITER */
#include <stdio.h>
int main(void)
{
    printf("Built without writer support, skipping test.\n");
    return 0;
}
#endif
