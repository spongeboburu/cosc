#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include "cosc.h"

static char buffer[1024];

static const struct cosc_message WRITE_MESSAGE =
{
    "/hello", 6,
    ",ifrcmsSbhtdTFNI", 16,
    {.write=NULL},
    0
};

static int func_setup(void **state)
{
    memset(buffer, 0, sizeof(buffer));
    return 0;
}

static void test_signature(void **state)
{
    cosc_int32 ret;
    const char *address = NULL, *typetag = NULL;
    cosc_int32 address_len = 0, typetag_len = 0;
    ret = cosc_write_signature(
        buffer, sizeof(buffer),
        WRITE_MESSAGE.address, WRITE_MESSAGE.address_n,
        WRITE_MESSAGE.typetag, WRITE_MESSAGE.typetag_n,
        0
    );
    assert_int_equal(ret, 28);
    ret = cosc_read_signature(
        buffer, sizeof(buffer),
        &address, &address_len,
        &typetag, &typetag_len,
        NULL
    );
    assert_int_equal(ret, 28);
    assert_non_null(address);
    assert_int_equal(address_len, WRITE_MESSAGE.address_n);
    assert_string_equal(address, WRITE_MESSAGE.address);
    assert_non_null(typetag);
    assert_int_equal(typetag_len, WRITE_MESSAGE.typetag_n);
    assert_string_equal(typetag, WRITE_MESSAGE.typetag);
}

static void test_signature_null(void **state)
{
    cosc_int32 ret;
    ret = cosc_write_signature(
        NULL, 0,
        WRITE_MESSAGE.address, WRITE_MESSAGE.address_n,
        WRITE_MESSAGE.typetag, WRITE_MESSAGE.typetag_n,
        0
    );
    assert_int_equal(ret, 28);
    ret = cosc_read_signature(
        buffer, sizeof(buffer),
        NULL, NULL,
        NULL, NULL,
        NULL
    );
}

static void test_signature_overrun(void **state)
{
    cosc_int32 ret;
    const char *address = NULL, *typetag = NULL;
    cosc_int32 address_len = 0, typetag_len = 0;
    ret = cosc_write_signature(
        buffer, sizeof(buffer),
        WRITE_MESSAGE.address, WRITE_MESSAGE.address_n,
        WRITE_MESSAGE.typetag, WRITE_MESSAGE.typetag_n,
        0
    );
    assert_int_equal(ret, 28);
    ret = cosc_write_signature(
        buffer, 27,
        WRITE_MESSAGE.address, WRITE_MESSAGE.address_n,
        WRITE_MESSAGE.typetag, WRITE_MESSAGE.typetag_n,
        0
    );
    assert_int_equal(ret, COSC_EOVERRUN);
    ret = cosc_read_signature(
        buffer, 27,
        &address, &address_len,
        &typetag, &typetag_len,
        NULL
    );
}

static void test_signature_psize(void **state)
{
    cosc_int32 ret, psize = -1;
    const char *address = NULL, *typetag = NULL;
    cosc_int32 address_len = 0, typetag_len = 0;
    ret = cosc_write_signature(
        buffer, sizeof(buffer),
        WRITE_MESSAGE.address, WRITE_MESSAGE.address_n,
        WRITE_MESSAGE.typetag, WRITE_MESSAGE.typetag_n,
        -1
    );
    assert_int_equal(ret, 32);
    ret = cosc_read_signature(
        buffer, sizeof(buffer),
        &address, &address_len,
        &typetag, &typetag_len,
        &psize
    );
    assert_int_equal(ret, 32);
    assert_int_equal(psize, 28);
    assert_non_null(address);
    assert_int_equal(address_len, WRITE_MESSAGE.address_n);
    assert_string_equal(address, WRITE_MESSAGE.address);
    assert_non_null(typetag);
    assert_int_equal(typetag_len, WRITE_MESSAGE.typetag_n);
    assert_string_equal(typetag, WRITE_MESSAGE.typetag);
}

static void test_signature_invalid_psize(void **state)
{
    cosc_int32 ret, psize = -1;
    const char *address = NULL, *typetag = NULL;
    cosc_int32 address_len = 0, typetag_len = 0;
    ret = cosc_write_signature(
        buffer, sizeof(buffer),
        WRITE_MESSAGE.address, WRITE_MESSAGE.address_n,
        WRITE_MESSAGE.typetag, WRITE_MESSAGE.typetag_n,
        -1
    );
    assert_int_equal(ret, 32);
    ret = cosc_write_signature(
        buffer, sizeof(buffer),
        WRITE_MESSAGE.address, WRITE_MESSAGE.address_n,
        WRITE_MESSAGE.typetag, WRITE_MESSAGE.typetag_n,
        27
    );
    assert_int_equal(ret, COSC_EPSIZE);
    cosc_write_int32(buffer, 4, 27);
    ret = cosc_read_signature(
        buffer, sizeof(buffer),
        &address, &address_len,
        &typetag, &typetag_len,
        &psize
    );
    assert_int_equal(ret, COSC_EPSIZE);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_signature, func_setup),
        cmocka_unit_test_setup(test_signature_null, func_setup),
        cmocka_unit_test_setup(test_signature_overrun, func_setup),
        cmocka_unit_test_setup(test_signature_psize, func_setup),
        cmocka_unit_test_setup(test_signature_invalid_psize, func_setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
