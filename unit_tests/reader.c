#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include "cosc.h"

// FIXME: implement unfinished blob test
// FIXME: maybe check that the read values are actually correct.

#if !defined(COSC_NOREADER) && !defined(COSC_NOSWAP)

static struct cosc_reader reader;
static struct cosc_level levels[4];
static const cosc_int32 level_max = 4;


static const unsigned char empty_bundle_noprefix[16] = {
    '#',  'b',  'u',  'n',  'd',  'l',  'e', 0x00,
    0x00, 0x00, 0x00, 0x00, 0x12,  '4',  'V',  'x',
};

static const unsigned char empty_bundle_prefix[40] = {
    0x00, 0x00, 0x00, 0x10,  '#',  'b',  'u',  'n',
    'd',  'l',  'e', 0x00, 0x00, 0x00, 0x00, 0x00,
    0x12,  '4',  'V',  'x', 0x00, 0x00, 0x00, 0x10,
    '#',  'b',  'u',  'n',  'd',  'l',  'e', 0x00,
    0x00, 0x00, 0x00, 0x00, 0x12,  '4',  'V',  'x',
};

static const unsigned char empty_message_noprefix[8] = {
    'a',  'b',  'c', 0x00,  ',', 0x00, 0x00, 0x00,
};

static const unsigned char empty_message_prefix[24] = {
    0x00, 0x00, 0x00, 0x08,  'a',  'b',  'c', 0x00,
     ',', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
     'a',  'b',  'c', 0x00,  ',', 0x00, 0x00, 0x00,
};

static const unsigned char bundle_empty_messages_noprefix[64] = {
     '#',  'b',  'u',  'n',  'd',  'l',  'e', 0x00,
    0x00, 0x00, 0x00, 0x00, 0x12,  '4',  'V',  'x',
    0x00, 0x00, 0x00, 0x08,  'a',  'b',  'c', 0x00,
     ',', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
     'a',  'b',  'c', 0x00,  ',', 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08,  'a',  'b',  'c', 0x00,
     ',', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
     'a',  'b',  'c', 0x00,  ',', 0x00, 0x00, 0x00,
};

static const unsigned char message_noarray[88] = {
    0x00, 0x00, 0x00,  'T',  'a',  'b',  'c', 0x00,
     ',',  'i',  'f',  's',  'b',  'h',  't',  'd',
     'S',  'c',  'r',  'm',  'T',  'F',  'N',  'I',
    0x00, 0x00, 0x00, 0x00, 0x87,  'e',  'C',  '!',
     'A',  'E',  'p', 0xa4,  'a',  'b',  'c', 0x00,
    0x00, 0x00, 0x00, 0x04,  'a',  'b',  'c', 0x00,
    0x12,  '4',  'V',  'x', 0x12,  '4',  'V',  'x',
    0x87,  'e',  'C',  '!', 0x87,  'e',  'C',  '!',
    '@', 0x93,  'J',  'E',  'm',  '\\', 0xfa, 0xad,
     'a',  'b',  'c', 0x00,  'c', 0x00, 0x00, 0x00,
    0x87,  'e',  'C',  '!', 0x01, 0x02, 0x03, 0x04,
};

#ifndef COSC_NOARRAY
static const unsigned char message_array[152] = {
    0x00, 0x00, 0x00, 0x94, 'a', 'b', 'c', 0x00,
     ',', 'i', '[', 'f', 'f', 'f', ']', 0x00,
    0x00, 0x00, 0x00, 0x0a, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, '?', 0x80, 0x00, 0x00,
     '?', 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
#endif

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
    cosc_uint64 timetag = 0;
#else
    cosc_uint64 timetag = {0, 0};
#endif
    cosc_reader_setup(&reader, empty_bundle_noprefix, sizeof(empty_bundle_noprefix), levels, level_max, 0);
    assert_int_equal(cosc_reader_open_bundle(&reader, &timetag, NULL), 16);
#ifndef COSC_NOINT64
    assert_int_equal(timetag, 0x12345678);
#else
    assert_int_equal(timetag.lo, 0x12345678);
#endif
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 16);
    assert_int_equal(cosc_reader_open_bundle(&reader, NULL, NULL), COSC_EPSIZEFLAG);
}

static void test_empty_bundle_prefix(void **state)
{
#ifndef COSC_NOINT64
    cosc_uint64 timetag = 0;
#else
    cosc_uint64 timetag = {0, 0};
#endif
    cosc_reader_setup(&reader, empty_bundle_prefix, sizeof(empty_bundle_prefix), levels, level_max, COSC_SERIAL_PSIZE);
    cosc_int32 psize = 0;
    assert_int_equal(cosc_reader_open_bundle(&reader, &timetag, &psize), 20);
    assert_int_equal(psize, 16);
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 20);
    psize = 0;
    assert_int_equal(cosc_reader_open_bundle(&reader, &timetag, &psize), 20);
    assert_int_equal(psize, 16);
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 40);
}

static void test_empty_message_noprefix(void **state)
{
    const char *address = "", *typetag = "";
    cosc_int32 address_n = 0, typetag_n = 0;
    cosc_reader_setup(&reader, empty_message_noprefix, sizeof(empty_message_noprefix), levels, level_max, 0);
    assert_int_equal(cosc_reader_open_message(&reader, &address, &address_n, &typetag, &typetag_n, NULL), 8);
    assert_string_equal(address, "abc");
    assert_int_equal(address_n, 3);
    assert_string_equal(typetag, ",");
    assert_int_equal(typetag_n, 1);
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 8);
    assert_int_equal(cosc_reader_open_message(&reader, &address, &address_n, &typetag, &typetag_n, 0), COSC_EPSIZEFLAG);
}

static void test_empty_message_prefix(void **state)
{
    const char *address = "", *typetag = "";
    cosc_int32 address_n = 0, typetag_n = 0;
    cosc_reader_setup(&reader, empty_message_prefix, sizeof(empty_message_prefix), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_reader_open_message(&reader, &address, &address_n, &typetag, &typetag_n, NULL), 12);
    assert_string_equal(address, "abc");
    assert_int_equal(address_n, 3);
    assert_string_equal(typetag, ",");
    assert_int_equal(typetag_n, 1);
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 12);
    address = ""; typetag = ""; address_n = 0; typetag_n = 0;
    assert_int_equal(cosc_reader_open_message(&reader, &address, &address_n, &typetag, &typetag_n, NULL), 12);
    assert_string_equal(address, "abc");
    assert_int_equal(address_n, 3);
    assert_string_equal(typetag, ",");
    assert_int_equal(typetag_n, 1);
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 24);
}

static void test_bundle_empty_messages_noprefix(void **state)
{
#ifndef COSC_NOINT64
    cosc_uint64 timetag = 0;
#else
    cosc_uint64 timetag = {0, 0};
#endif
    const char *address = "", *typetag = "";
    cosc_int32 address_n = 0, typetag_n = 0;
    cosc_int32 psize = 0;
    cosc_reader_setup(&reader, bundle_empty_messages_noprefix, sizeof(bundle_empty_messages_noprefix), levels, level_max, 0);
    assert_int_equal(cosc_reader_open_bundle(&reader, &timetag, 0), 16);
    assert_int_equal(cosc_reader_get_size(&reader), 16);
    assert_int_equal(cosc_reader_open_message(&reader, &address, &address_n, &typetag, &typetag_n, &psize), 12);
    assert_int_equal(psize, 8);
    assert_string_equal(address, "abc");
    assert_int_equal(address_n, 3);
    assert_string_equal(typetag, ",");
    assert_int_equal(typetag_n, 1);
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 16 + 12);
    assert_int_equal(cosc_reader_open_message(&reader, 0, 0, 0, 0, 0), 12);
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 16 + 24);
    assert_int_equal(cosc_reader_open_message(&reader, 0, 0, 0, 0, 0), 12);
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 16 + 36);
    assert_int_equal(cosc_reader_open_message(&reader, 0, 0, 0, 0, 0), 12);
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 16 + 48);
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 16 + 48);
    assert_int_equal(cosc_reader_open_bundle(&reader, 0, 0), COSC_EPSIZEFLAG);
}

static void test_message_noarray(void **state)
{
    const char *address = "", *typetag = "";
    cosc_int32 address_n = 0, typetag_n = 0;
    cosc_int32 psize = 0;
    union cosc_value value;
    cosc_reader_setup(&reader, message_noarray, sizeof(message_noarray), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_reader_open_message(&reader, &address, &address_n, &typetag, &typetag_n, &psize), 28);
    assert_int_equal(84, psize);
    assert_int_equal(cosc_reader_get_size(&reader), 28);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'i');
    assert_int_equal(cosc_reader_int32(&reader, &value.i), 4);
    assert_int_equal(cosc_reader_get_size(&reader), 32);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'f');
    assert_int_equal(cosc_reader_float32(&reader, &value.f), 4);
    assert_int_equal(cosc_reader_get_size(&reader), 36);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 's');
    assert_int_equal(cosc_reader_string(&reader, 0, 0, &value.s.s, &value.s.length), 4);
    assert_int_equal(cosc_reader_get_size(&reader), 40);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'b');
    assert_int_equal(cosc_reader_blob(&reader, 0, 0, &value.b.b, &value.b.size), 8);
    assert_int_equal(cosc_reader_get_size(&reader), 48);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'h');
    assert_int_equal(cosc_reader_int64(&reader, &value.h), 8);
    assert_int_equal(cosc_reader_get_size(&reader), 56);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 't');
    assert_int_equal(cosc_reader_uint64(&reader, &value.t), 8);
    assert_int_equal(cosc_reader_get_size(&reader), 64);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'd');
    assert_int_equal(cosc_reader_float64(&reader, &value.d), 8);
    assert_int_equal(cosc_reader_get_size(&reader), 72);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'S');
    assert_int_equal(cosc_reader_string(&reader, 0, 0, &value.s.s, &value.s.length), 4);
    assert_int_equal(cosc_reader_get_size(&reader), 76);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'c');
    assert_int_equal(cosc_reader_char(&reader, &value.c), 4);
    assert_int_equal(cosc_reader_get_size(&reader), 80);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'r');
    assert_int_equal(cosc_reader_uint32(&reader, &value.r), 4);
    assert_int_equal(cosc_reader_get_size(&reader), 84);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'm');
    assert_int_equal(cosc_reader_midi(&reader, value.m), 4);
    assert_int_equal(cosc_reader_get_size(&reader), 88);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'T');
    assert_int_equal(cosc_reader_skip(&reader), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 88);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'F');
    assert_int_equal(cosc_reader_skip(&reader), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 88);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'N');
    assert_int_equal(cosc_reader_skip(&reader), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 88);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'I');
    assert_int_equal(cosc_reader_skip(&reader), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 88);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 0);
    assert_int_equal(cosc_reader_close(&reader, 1), 0);
    assert_int_equal(cosc_reader_get_size(&reader), 88);
}

#ifndef COSC_NOARRAY
static void test_message_array(void **state)
{
    const char *address = "", *typetag = "";
    cosc_int32 address_n = 0, typetag_n = 0;
    cosc_int32 psize = 0;
    union cosc_value value;
    cosc_reader_setup(&reader, message_array, sizeof(message_array), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_reader_open_message(&reader, &address, &address_n, &typetag, &typetag_n, &psize), 16);
    assert_int_equal(148, psize);
    assert_int_equal(cosc_reader_get_size(&reader), 16);
    assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'i');
    assert_int_equal(cosc_reader_int32(&reader, &value.i), 4);
    assert_int_equal(cosc_reader_get_size(&reader), 20);
    cosc_int32 sz = cosc_reader_get_size(&reader);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            assert_int_equal(cosc_reader_get_level_msgtype(&reader), 'f');
            assert_int_equal(cosc_reader_float32(&reader, &value.f), 4);
            sz += 4;
            assert_int_equal(cosc_reader_get_size(&reader), sz);
        }
    }
    assert_int_equal(cosc_reader_close(&reader, 0), 0);
}
#endif

static void test_message_unfinished_noarray(void **state)
{
    cosc_reader_setup(&reader, message_noarray, sizeof(message_noarray), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_reader_open_message(&reader, 0, 0, 0, 0, 0), 28);
    assert_int_equal(cosc_reader_close(&reader, 1), 60);
    assert_int_equal(cosc_reader_get_size(&reader), 88);
}

#ifndef COSC_NOARRAY
static void test_message_unfinished_array(void **state)
{
    cosc_reader_setup(&reader, message_array, sizeof(message_array), levels, level_max, COSC_SERIAL_PSIZE);
    assert_int_equal(cosc_reader_open_message(&reader, 0, 0, 0, 0, 0), 16);
    assert_int_equal(cosc_reader_close(&reader, 1), 16);
    assert_int_equal(cosc_reader_get_size(&reader), 32);
}
#endif

// static void test_blob_unfinished(void **state)
// {
    
// }

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_empty_bundle_noprefix, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_empty_bundle_prefix, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_empty_message_noprefix, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_empty_message_prefix, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_bundle_empty_messages_noprefix, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_message_noarray, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_message_unfinished_noarray, func_setup, func_teardown),
        // cmocka_unit_test_setup_teardown(test_blob_unfinished, func_setup, func_teardown),
#ifndef COSC_NOARRAY
        cmocka_unit_test_setup_teardown(test_message_array, func_setup, func_teardown),
        cmocka_unit_test_setup_teardown(test_message_unfinished_array, func_setup, func_teardown),
#endif
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
    return 0;
}

#else /* !COSC_NOREADER && ! COSC_NOSWAP */
#include <stdio.h>
int main(void)
{
    printf("Built without reader (or swap) support, skipping test.\n");
    return 0;
}
#endif
