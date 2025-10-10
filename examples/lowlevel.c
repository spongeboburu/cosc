/**
 * @brief Example of using the low level functions.
 * @file lowlevel.c
 *
 * ```
 * Copyright 2025 Peter Gebauer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ```
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "cosc.h"

int main(int argc, char *argv[])
{
    char buffer[1024] = {0};
    cosc_int32 ret, offset = 0;

    // Note, we are doing it sloppy here because we know everything
    // will fit the buffer, but it might be wise to check `ret`
    // for negative error codes.

    // Write the address.
    ret = cosc_write_string(buffer, sizeof(buffer), "/address", 1024, NULL);
    offset += ret;
    // Write the address.
    ret = cosc_write_string(buffer + offset, sizeof(buffer) - offset, ",if", 1024, NULL);
    offset += ret;

    // We could have used the helper cosc_write_signature:
    // ret = cosc_write_signature(buffer, sizeof(buffer), "/address", 1024, ",if", 1024, -1);
    
    ret = cosc_write_int32(buffer + offset, sizeof(buffer) - offset, 1234);
    offset += ret;
    ret = cosc_write_float32(buffer + offset, sizeof(buffer) - offset, 12.34);
    offset += ret;

    printf("Wrote %d bytes.\n", offset);

    // Read it all back.
    offset = 0;
    const char *address = buffer;
    ret = cosc_read_string(buffer + offset, sizeof(buffer) - offset, NULL, 0, NULL);
    offset += ret;
    const char *typetag = buffer + offset;
    ret = cosc_read_string(buffer + offset, sizeof(buffer) - offset, NULL, 0, NULL);
    offset += ret;
    cosc_int32 value_i;
    ret = cosc_read_int32(buffer + offset, sizeof(buffer) - offset, &value_i);
    offset += ret;
    cosc_float32 value_f;
    ret = cosc_read_float32(buffer + offset, sizeof(buffer) - offset, &value_f);
    offset += ret;
    printf("Address=%s, typetag=%s, i=%d f=%f\n", address, typetag, value_i, value_f);
}
