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
- Handle 64-bit values on systems without 64-bit types.
- Handle floating point values on systems without floating point types.
- No dynamic allocations.
- Use of standard library is optional.
- Use of `stdint.h` is optional.
- Ability to typedef custom type for integer and floats.
- C99 and C++11 compliant.
- No optimizations relying on undefined behavior or assumptions
  about alignment restrictions.
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
the end of the message. It would be possible to scan ahead and
calculate the exact size of the array based on the trailing types,

Example uses of arrays in a typetag:

- ",[fff]"      -- Message is an array of vector3f.
- ",ifsb[fff]"  -- Message ends with an array of vector3f.
- ",ifsb[fff]i" -- Invalid.

This should be good enough. NOTE that for maximum compatibility with
other OSC endpoints you should make sure they have support for arrays.

Array support can be removed at compile time by defining COSC_NOARRAY which
may provide a modest performance boost if arrays are not required.


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
- `COSC_NOSTDINT` for no inclusion of `stdint.h` (or <cstdint> if C++).
- `COSC_NOINT64` to typedef `cosc_int64` and `cosc_uint64` types as `cosc_64bits`.
- `COSC_NOFLOAT32` to typedef `cosc_float32` as `cosc_uint32`.
- `COSC_NOFLOAT64` to typedef `cosc_float64` as `cosc_uint64`.
- `COSC_TYPE_UINT32` used to override typedef `cosc_uint32`.
- `COSC_TYPE_INT32` used to override typedef `cosc_int32`.
- `COSC_TYPE_FLOAT32` used to override typedef `cosc_float32`.
- `COSC_TYPE_UINT64` used to override typedef `cosc_uint64`.
- `COSC_TYPE_INT64` used to override typedef `cosc_int64`.
- `COSC_TYPE_FLOAT64` used to override typedef `cosc_float64`.


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
