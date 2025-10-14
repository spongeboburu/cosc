# cosc, OSC library for C99 or C++11.

What is this about? OSC stands for Open Sound Control and is a formalized way
of reading/writing data, see <https://opensoundcontrol.stanford.edu/spec-1_0.html>.

For example as RPC over UDP in projects like
<https://supercollider.github.io/>, but it can be used in any event
driven system due to it's simple design. Since I've used OSC a lot in
recent work I thought this would be fun project to tinker with on my
spare time and hopefully someone finds it useful.

This library is designed for maximum cross-platform compatibility and with
the build (and first time inclusion) define options it should compile and run
almost anywhere, including unhosted environments without standard libraries.

If you are looking for a more high level implementation with network support,
perhaps <https://liblo.sourceforge.net/> is a better choice.

## Features and limitations

*Features:*

- Write/read OSC data.
- Address and typetag validation.
- Address and typetag pattern matching.
- Timetag conversions.
- Higher level writer/reader APIs with nesting.
- Handle 64-bit values on systems without 64-bit types.
- Handle floating point values on systems without floating point types.
- No dynamic allocations.
- Use of standard library is optional.
- Use of `stdint.h` is optional.
- Ability to typedef custom types for integer and floats.
- C99 and C++11 compliant.
- No optimizations relying on undefined behavior.
- MIT license.

*Limitations:*

- No transport layer, though this is as intended for this library.
- Only one array per message.
- Arrays must be at the end of the message.

Because the OSC specifications does not describe how arrays should
be specified and does not give any examples this implementation is
mostly guess work. The fact that the length of the array is unspecified,
unlike BLOBs for example, makes it very difficult to implement in a
flexible way which is why many implementations do not support arrays.

The limitation is then that arrays may appear only once and only at
the end of the message.

Example uses of arrays in a typetag:

- ",[fff]"      -- Message is an array of vector3f.
- ",ifsb[fff]"  -- Message ends with an array of vector3f.
- ",ifsb[fff]i" -- Invalid.

This should be good enough. NOTE that for maximum compatibility with
other OSC endpoints you should make sure they have support for arrays.

Array support can be removed at compile time by defining COSC_NOARRAY which
may provide a modest performance boost if arrays are not required.


## Building

Using a compiler, for example gcc, if in the source directory:

```
gcc -std=c99 cosc.c
```

and then just add the `-D` defines as required.

Using cmake, if in the source directory:

```
cmake -B build
cmake --build build
```

by default cmake will only build the static library, but if you want to build
everything go for something like this:

```
cmake -B build -DCOSC_BUILD_SHARED=ON -DCOSC_BUILD_EXAMPLES=ON -DCOSC_BUILD_TESTS=ON
cmake --build build
cmake --build build --target docs
```

will build static library, shared library, examples unit tests and generate
Doxygen documentation. You can also add `-D` defines as required to the
first cmake command.


## Requirements

- C99 or C++11 compatible compiler.
- 32-bit signed and unsigned integer support.

Optional:

- 64-bit integers.
- IEEE 754 single precision (32-bit) float.
- IEEE 754 double precision (64-bit) float.
- Standard C library (or standard C++ library).
- Standard C headers (or C++ headers).
- cmake for building.


## Defines at build/include

Defined at compile and include time:

- `COSC_NOSTDLIB` for no use of the standard library.
    This will also remove the dump functions.
- `COSC_NOPATTERN` to remove the pattern matching functions.
- `COSC_NOSWAP` for no endian swapping.
- `COSC_NOARRAY` to remove the support for arrays.
- `COSC_NODUMP` to remove the dump functions.
- `COSC_NOWRITER` to remove the writer functions.
- `COSC_NOREADER` to remove the reader functions.
- `COSC_NOTIMETAG` to remove timetag conversion functions.
- `COSC_NOFLTCONV` to remove float conversion functions.
- `COSC_NOSTDINT` for no inclusion of `stdint.h` (or `cstdint` if C++).
- `COSC_NOINT64` to typedef `cosc_int64` and `cosc_uint64` types as `cosc_64bits`.
- `COSC_NOFLOAT32` to typedef `cosc_float32` as `cosc_uint32`.
- `COSC_NOFLOAT64` to typedef `cosc_float64` as `cosc_uint64`.
- `COSC_TYPE_UINT32` used to override typedef `cosc_uint32`.
- `COSC_TYPE_INT32` used to override typedef `cosc_int32`.
- `COSC_TYPE_FLOAT32` used to override typedef `cosc_float32`.
- `COSC_TYPE_UINT64` used to override typedef `cosc_uint64`.
- `COSC_TYPE_INT64` used to override typedef `cosc_int64`.
- `COSC_TYPE_FLOAT64` used to override typedef `cosc_float64`.


## Example uses

At it's lowest level you can just write the raw data:

```c
    char buffer[1024] = {0};
    cosc_int32 ret, offset = 0;

    // Write it
    ret = cosc_write_string(buffer, sizeof(buffer), "/address", 1024, NULL);
    offset += ret;
    ret = cosc_write_string(buffer + offset, sizeof(buffer) - offset, ",if", 1024, NULL);
    offset += ret;
    ret = cosc_write_int32(buffer + offset, sizeof(buffer) - offset, 1234);
    offset += ret;
    ret = cosc_write_float32(buffer + offset, sizeof(buffer) - offset, 12.34);
    offset += ret;

    // Read it
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
```

Write and read full message using a struct:

```c
    char buffer[1024];
    union cosc_value values[11] = {
        {.i = 0x12345678},
        {.f = 12.34f},
        {.s = {.s="Hello World!", .length = 1024}},
    };
    struct cosc_message message;
    message.address = "/hello_world";
    message.address_n = 12;
    message.typetag = ",ifs";
    message.typetag_n = 4;
    message.values.write = values;
    message.values_n = 3;

    cosc_write_message(
        buffer, sizeof(buffer), &message,
        true, NULL
    );
    cosc_read_message(
        buffer, sizeof(buffer), &message,
        &packet_size, &value_count
    );
```

We could put a bundle ahead of it. This is the simple way of writing
and reading OSC data, but there's a convenience API if you want to
do more advanced nesting of messages in bundles etc.

```c
    char buffer[1024];
    struct cosc_writer writer;
    cosc_writer_setup(
        &writer,
        buffer, sizeof(buffer), // Store to this buffer.
        levels, 4, // Provide 4 levels, 1 is minimum.
        0 // Zero flags.
    );
    struct cosc_reader reader;
    cosc_reader_setup(
        &reader,
        buffer, cosc_writer_get_size(&writer), // Load from this buffer.
        levels, 4, // Levels.
        0 // No flags.
    );
```

and then using the writer/reader API to basically push or pop data
from the buffer.

### 64-bit types on non-64 systems.

Handling 64-bit types on systems without them involves a struct with
hi (most significant) and lo (least significant) members.

```c
struct cosc_64bits
{
    cosc_uint32 hi;
    cosc_uint32 lo;
};
```

There's a few conversion helper functions, but you are free to manipulate
the bits for `cosc_uint64`, `cosc_int64` and `cosc_float64` this way.


## License

```unparsed
cosc
Copyright 2025 Peter Gebauer

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```
