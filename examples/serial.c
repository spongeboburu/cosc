/**
 * @brief Example of using the serial writer and reader APIs.
 * @file serial.c
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
    struct cosc_level levels[4];
    cosc_int32 ret;
    struct cosc_serial serial;

    // Setup the writer.
    cosc_writer_setup(
        &serial,
        buffer, sizeof(buffer), // Store to this buffer.
        levels, 4, // Provide 4 levels, 1 is minimum.
        0 // Zero flags.
    );

    // Timetag to 3.5 seconds.
    cosc_uint64 timetag = cosc_time_to_timetag(3, 500000000);

    // Start with a bundle (we could start with a message directly too).
    ret = cosc_writer_start_bundle(&serial, timetag);
    if (ret < 0)
    {
        printf("Uh oh, couldn't start writing bundle: %d\n", ret);
        return 1;
    }

    // We could use cosc_writer_message(&serial, &message, NULL) to write
    // an entire message, but we want to nest things in the blob so
    // let's go for an start message.
    ret = cosc_writer_start_message(&serial, "/address", 1024, ",ibf", 1024);
    if (ret < 0)
    {
        printf("Uh oh, couldn't start writing message: %d\n", ret);
        return 1;
    }

    // The first value is 'i'.
    ret = cosc_writer_int32(&serial, 150);
    if (ret < 0)
    {
        printf("Uh oh, couldn't write 'i': %d\n", ret);
        return 1;
    }

    // We could write the entire blob like this:
    // ret = cosc_writer_blob(&serial, blob_data, blob_size);
    // But we want to nest another message into the blob.
    ret = cosc_writer_start_blob(&serial);
    if (ret < 0)
    {
        printf("Uh oh, couldn't start writing blob: %d\n", ret);
        return 1;
    }

    // With the blob start all other writes will be stored to the blob,
    // but we want a message. We could do it like this:
    // cosc_writer_message(&serial, &message, NULL);
    // But let's do it with an start message instead.
    ret = cosc_writer_start_message(&serial, "/nested", 1024, ",i", 1024);
    if (ret < 0)
    {
        printf("Uh oh, couldn't start writing nested message: %d\n", ret);
        return 1;
    }
    // The first value is 'i'.
    ret = cosc_writer_int32(&serial, 12345678);
    if (ret < 0)
    {
        printf("Uh oh, couldn't write 'i': %d\n", ret);
        return 1;
    }

    // Let's end the message nested message.
    ret = cosc_writer_end_message(&serial);
    if (ret < 0)
    {
        printf("Uh oh, couldn't end writing the nested message: %d\n", ret);
        return 1;
    }

    // Let's end the blob.
    ret = cosc_writer_end_blob(&serial);
    if (ret < 0)
    {
        printf("Uh oh, couldn't end writing the blob: %d\n", ret);
        return 1;
    }

    // Now we're back to the first message, let's wrap it up with
    // it's third and final value of type 'f'.
    ret = cosc_writer_float32(&serial, 1.5);
    if (ret < 0)
    {
        printf("Uh oh, couldn't write 'f': %d\n", ret);
        return 1;
    }

    // Done, let's end the message.
    ret = cosc_writer_end_message(&serial);
    if (ret < 0)
    {
        printf("Uh oh, couldn't end writing the message: %d\n", ret);
        return 1;
    }

    // Done, let's end the bundle.
    ret = cosc_writer_end_bundle(&serial);
    if (ret < 0)
    {
        printf("Uh oh, couldn't end writing the bundle: %d\n", ret);
        return 1;
    }
    
    // How big was it?
    printf("Wrote %d bytes of OSC data.\n", cosc_serial_get_size(&serial));

    // We could dump the buffer to see what it looks like.
    for (int i = 0; i < cosc_serial_get_size(&serial); i++)
    {
        printf("%03d: 0x%02x '%c'\n", i, (unsigned char)buffer[i], buffer[i] > 31 ? buffer[i] : ' ');
    }

    // Now to read it all.
    cosc_reader_setup(
        &serial,
        buffer, cosc_serial_get_size(&serial), // Load from this buffer using the written size.
        levels, 4, // Levels.
        0 // No flags.
    );

    // Let's just make sure we have a bundle
    if (cosc_reader_peek_bundle(&serial, NULL, NULL) < 0)
    {
        printf("Uh oh, not a bundle? %d\n", ret);
        return 1;
    }

    // The bundle.
    timetag = 0; // Just to make sure we actuall did read it.
    ret = cosc_reader_start_bundle(&serial, &timetag);
    if (ret < 0)
    {
        printf("Uh oh, couldn't start bundle: %d\n", ret);
        return 1;
    }

    cosc_uint32 nanos;
    cosc_uint32 seconds = cosc_timetag_to_time(timetag, &nanos);
    printf("Timetag is %f.\n", (double)seconds + nanos / 1000000000.0);

    // Start the message.
    const char *address, *typetag;
    cosc_int32 address_len, typetag_len;
    ret = cosc_reader_start_message(
        &serial,
        &address, &address_len,
        &typetag, &typetag_len
    );
    if (ret < 0)
    {
        printf("Uh oh, couldn't start message: %d\n", ret);
        return 1;
    }
    printf("Address '%s' and typetag '%s'\n", address, typetag);

    // First value
    cosc_int32 value_i;
    ret = cosc_reader_int32(&serial, &value_i);
    if (ret < 0)
    {
        printf("Uh oh, couldn't read 'i': %d\n", ret);
        return 1;
    }
    printf("Value 'i': %d\n", value_i);

    // Then the blob.
    cosc_int32 blob_size;
    ret = cosc_reader_start_blob(&serial, &blob_size);
    if (ret < 0)
    {
        printf("Uh oh, couldn't start blob: %d\n", ret);
        return 1;
    }
    printf("Blob is %d bytes.\n", blob_size);

    // We know we put a message in here, let's do the shorthand
    // function this time. We need to store the values which
    // is only 1 'i' value.
    union cosc_value values[1];
    struct cosc_message message = {
        .values = {.read = values},
        .values_n = 1
    };
    cosc_int32 value_count; // We can see the actual number of values here.
    ret = cosc_reader_message(&serial, &message, &value_count, true);
    if (ret < 0)
    {
        printf("Uh oh, couldn't read nested message: %d\n", ret);
        return 1;
    }
    printf("Address='%s', Typetag='%s'\n", message.address, message.typetag);
    printf("Nested message has %d values.\n", value_count);
    printf("Value 'i': %d\n", values[0].i);

    // Close the blob.
    ret = cosc_reader_end_blob(&serial);
    if (ret < 0)
    {
        printf("Uh oh, couldn't close the blob: %d\n", ret);
        return 1;
    }

    // Final value of the top message.
    cosc_float32 value_f;
    ret = cosc_reader_float32(&serial, &value_f);
    if (ret < 0)
    {
        printf("Uh oh, couldn't read 'f': %d\n", ret);
        return 1;
    }
    printf("Final float is %f\n", value_f);

    // We need to end the top message.
    ret = cosc_reader_end_message(&serial, true);
    if (ret < 0)
    {
        printf("Uh oh, couldn't end the message: %d\n", ret);
        return 1;
    }

    // We need to end the bundle.
    ret = cosc_reader_end_bundle(&serial);
    if (ret < 0)
    {
        printf("Uh oh, couldn't end the message: %d\n", ret);
        return 1;
    }
    
    // How big was it?
    printf("Read %d bytes of OSC data.\n", cosc_serial_get_size(&serial));

    return 0;
}
