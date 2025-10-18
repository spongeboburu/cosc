/**
 * @brief Example of writing and reading a message.
 * @file message.c
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

    // An array of values that actually have a payload.
    union cosc_value values[11] = {
        {.i = 0x12345678},
        {.f = 12.34f},
        {.s = {.s="Hello World!", .length = 1024}},
        {.b = {.b="Hello World!", .size = 12}},
        {.h = 0x1234567812345678ULL},
        {.t = 0x8765432187654321ULL},
        {.d = 1234.5678},
        {.s = {.s="Hello World!", .length = 1024}},
        {.c = 'A'},
        {.r = 0x87654321},
        {.m = {0x01, 0x02, 0x03, 0x04}}
    };

    // Put the message together.
    struct cosc_message message;
    message.address = "/hello_world";
    message.address_n = 1024;
    message.typetag = ",ifsbhtdScrmTFNI";
    message.typetag_n = 1024;
    message.values.write = values;
    message.values_n = 11;

    // Write it.
    cosc_int32 value_count = 0;
    cosc_int32 ret = cosc_write_message(
        buffer, sizeof(buffer), &message,
        -1, &value_count
    );
    if (ret < 0)
    {
        printf("Uh oh, error writing message: %d!\n", ret);
        return 1;
    }
    printf("%d bytes written for %d values.\n", ret, value_count);

    // Read it.
    cosc_int32 packet_size;
    ret = cosc_read_message(
        buffer, sizeof(buffer), &message,
        &packet_size, &value_count, true
    );
    if (ret < 0)
    {
        printf("Uh oh, error reading message: %d!\n", ret);
        return 2;
    }
    printf("%d bytes read for %d values, packet size is %d.\n", ret, value_count, packet_size);

    // Dump the message to a string.
    char tmps[1024];
    cosc_message_dump(tmps, sizeof(tmps), &message);
    printf("%s\n", tmps);
    return 0;
}
