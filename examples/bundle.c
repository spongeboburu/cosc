/**
 * @brief Example of writing a bundle with 10 message in it.
 * @file bundle.c
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

    // The message will only have 2 values.
    union cosc_value values[2] = {0};

    // Put the message together.
    struct cosc_message message;
    message.address = "/hello";
    message.address_n = 1024;
    message.typetag = ",if";
    message.typetag_n = 1024;
    message.values.write = values;
    message.values_n = 2;

    // Set timestamp at 60.25 seconds.
    cosc_uint64 timetag = cosc_time_to_timetag(60, 250000000);
    cosc_int32 size = cosc_write_bundle(buffer, sizeof(buffer), timetag, 0);
    if (size < 0)
    {
        printf("Uh oh, error %d.\n", size);
        return 1;
    }

    for (cosc_int32 i = 0; i < 10; i++)
    {
        values[0].i = i;
        values[1].f = i / 10.0;
        cosc_int32 ret = cosc_write_message(
            buffer + size, (cosc_int32)sizeof(buffer) - size, &message,
            true, NULL
        );
        if (ret < 0)
        {
            printf("Uh oh, error %d.\n", ret);
            return 2;
        }
        size += ret;
    }

    // Store the bundle size as the first 4 bytes, minus those 4 bytes.
    // Normally the bundle size would be indicated by some outside source,
    // like an UDP packet, but here we are providing it in the buffer.
    cosc_write_int32(buffer, 4, size - 4);
    
    printf("%d bytes written for the bundle.\n", size);

    // Remember how we stored the bundle size, read bundle will retrieve it.
    cosc_int32 bundle_size = 0;
    size = cosc_read_bundle(buffer, sizeof(buffer), &timetag, &bundle_size);
    if (size < 0)
    {
        printf("Uh oh, error %d.\n", size);
        return 3;
    }

    // Get the timestamp.
    cosc_uint32 nanos;
    double seconds = cosc_timetag_to_time(timetag, &nanos);
    printf("Bundle timestamp: %f\n", seconds + nanos / 1000000000.0);

    // bundle_size does not specify the first four bytes of the size integer.
    while (size - 4 < bundle_size)
    {

        // Read message.
        cosc_int32 packet_size;
        cosc_int32 ret = cosc_read_message(
            buffer + size, sizeof(buffer) - size, &message,
            &packet_size, 0
        );
        if (ret < 0)
        {
            printf("Uh oh, error %d.\n", ret);
            return 4;
        }

        printf("Message was %d bytes in size, packetsize is %d.\n", ret, packet_size);

        // Dump the message to a string.
        char tmps[1024];
        cosc_message_dump(tmps, sizeof(tmps), &message);
        printf("%s\n", tmps);
        size += ret;
    }
    printf("%d bytes read for the bundle.\n", size);

    return 0;
}
