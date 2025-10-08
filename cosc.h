/**
 * @file cosc.h
 * @brief OSC library for C99 or C++11.
 * @copyright Copyright 2025 Peter Gebauer (MIT license)
 *
 * @section build_options Build options
 *
 * Defined at compile AND include time:
 *
 * - COSC_NOSTDLIB for no use of the standard library.
 *   This will also remove the dump functions.
 * - COSC_NOPATTERN to remove the pattern matching functions.
 * - COSC_NOSWAP for no endian swapping.
 * - COSC_NOARRAY to remove the support for arrays.
 * - COSC_NOSTDINT to not include `stdint.h` (or <cstdint> if C++).
 * - COSC_NODUMP to remove the dump functions.
 * - COSC_NOWRITER to remove the writer functions.
 * - COSC_NOREADER to remove the reader functions.
 * - COSC_NOINT64 to typedef `cosc_int64` and `cosc_uint64` as @ref cosc_64bits.
 * - COSC_NOFLOAT32 to typedef `cosc_float32` as @ref cosc_uint32.
 * - COSC_NOFLOAT64 to typedef `cosc_float64` as @ref cosc_64bits.
 *   or @ref cosc_64bits if COSC_NOINT64 is also defined.
 *
 * Type overrides (also at compile AND include time):
 *
 * - COSC_TYPE_UINT32 used to override typedef @ref cosc_uint32.
 * - COSC_TYPE_INT32 used to override typedef @ref cosc_int32.
 * - COSC_TYPE_FLOAT32 used to override typedef @ref cosc_float32.
 * - COSC_TYPE_UINT64 used to override typedef @ref cosc_uint64.
 * - COSC_TYPE_INT64 used to override typedef @ref cosc_int64.
 * - COSC_TYPE_FLOAT64 used to override typedef @ref cosc_float64.
 *
 * NOTE that type overrides will not work for types affected by
 * COSC_NOINT64, COSC_FLOAT32 or COSC_NOFLOAT64 when those
 * are defined.
 *
 * @section license License
 *
 * ```unparsed
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
#ifndef COSC_H
#define COSC_H

/**
 * @example single_message.c
 */

/**
 * Used to declare linkage for functions.
 * @def COSC_API
 */

#ifndef COSC_API
#ifdef COSC_BUILD_SHARED
#ifdef _MSC_VER
#define COSC_API __declspec(dllexport)
#else
#define COSC_API
#endif
#else /* COSC_BUILD_SHARED */
#if defined(__clang__) || defined(__GNUC__)
#define COSC_API __attribute__ ((visibility ("hidden")))
#else
#define COSC_API
#endif
#endif /* COSC_BUILD_SHARED */
#endif /* COSC_API */

/**
 * Used to typedef @ref cosc_uint32.
 * @def COSC_TYPE_UINT32
 * @note If defined it must be at both compile and include time.
 */

/**
 * Used to typedef @ref cosc_int32.
 * @def COSC_TYPE_INT32
 * @note If defined it must be at both compile and include time.
 */

/**
 * Used to typedef @ref cosc_float32.
 * @def COSC_TYPE_FLOAT32
 * @note If defined it must be at both compile and include time.
 */

/**
 * Used to typedef @ref cosc_uint64.
 * @def COSC_TYPE_UINT64
 * @note If defined it must be at both compile and include time.
 */

/**
 * Used to typedef @ref cosc_int64.
 * @def COSC_TYPE_INT64
 * @note If defined it must be at both compile and include time.
 */

/**
 * Used to typedef @ref cosc_float64.
 * @def COSC_TYPE_FLOAT64
 * @note If defined it must be at both compile and include time.
 */

#ifndef COSC_NOSTDINT

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#ifndef COSC_TYPE_INT32
#define COSC_TYPE_INT32 int32_t
#endif
#ifndef COSC_TYPE_UINT32
#define COSC_TYPE_UINT32 uint32_t
#endif

#ifndef COSC_NOINT64
#ifndef COSC_TYPE_INT64
#define COSC_TYPE_INT64 int64_t
#endif
#else
#define COSC_TYPE_INT64 struct cosc_64bits
#endif

#ifndef COSC_NOINT64
#ifndef COSC_TYPE_UINT64
#define COSC_TYPE_UINT64 uint64_t
#endif
#else
#define COSC_TYPE_UINT64 struct cosc_64bits
#endif

#else /* COSC_NOSTDINT */

#ifndef COSC_TYPE_INT32
#define COSC_TYPE_INT32 int
#endif
#ifndef COSC_TYPE_UINT32
#define COSC_TYPE_UINT32 unsigned int
#endif

#ifndef COSC_NOINT64
#ifndef COSC_TYPE_INT64
#define COSC_TYPE_INT64 long long
#endif
#else
#define COSC_TYPE_INT64 struct cosc_64bits
#endif

#ifndef COSC_NOINT64
#ifndef COSC_TYPE_UINT64
#define COSC_TYPE_UINT64 unsigned long long
#endif
#else
#define COSC_TYPE_UINT64 struct cosc_64bits
#endif

#endif /* COSC_NOSTDINT */

#ifndef COSC_NOFLOAT32
#ifndef COSC_TYPE_FLOAT32
#define COSC_TYPE_FLOAT32 float
#endif
#else
#define COSC_TYPE_FLOAT32 COSC_TYPE_UINT32
#endif

#ifndef COSC_NOFLOAT64
#ifndef COSC_TYPE_FLOAT64
#define COSC_TYPE_FLOAT64 double
#endif
#else
#define COSC_TYPE_FLOAT64 COSC_TYPE_UINT64
#endif

/**
 * Unsigned 32-bit integer.
 */
typedef COSC_TYPE_UINT32 cosc_uint32;

/**
 * Signed 32-bit integer.
 */
typedef COSC_TYPE_INT32 cosc_int32;

/**
 * Used when 64-bit types are not available.
 */
struct cosc_64bits
{

    /**
     * Most significant 32 bits.
     */
    cosc_uint32 hi;

    /**
     * Least significant 32 bits.
     */
    cosc_uint32 lo;

};

/**
 * Signed 64-bit integer.
 */
typedef COSC_TYPE_INT64 cosc_int64;

/**
 * Unsigned 64-bit integer.
 */
typedef COSC_TYPE_UINT64 cosc_uint64;

/**
 * 32-bit floating point.
 */
typedef COSC_TYPE_FLOAT32 cosc_float32;

/**
 * 64-bit floating point.
 */
typedef COSC_TYPE_FLOAT64 cosc_float64;

/**
 * The maximum 4 byte aligned size that fits in a signed 32-bit integer.
 */
#define COSC_SIZE_MAX 2147483644

/**
 * Get the required zero pad size for strings that require
 * a zero terminator and 4 byte alignment.
 * @param sz The byte size of the string, excluding the zero terminator.
 * @returns The number of zeroes to pad.
 */
#define COSC_PADMUST(sz) (4 - ((cosc_uint32)(sz) & 3))

/**
 * Get the required zero pad size for 4 byte alignment.
 * @param sz The byte size of the object.
 * @returns The number of zeroes to pad.
 */
#define COSC_PAD(sz) (COSC_PADMUST(sz) & 3)

/**
 * The level type is a bundle.
 */
#define COSC_LEVEL_TYPE_BUNDLE 'B'

/**
 * The level type is a message.
 */
#define COSC_LEVEL_TYPE_MESSAGE 'M'

/**
 * The level type is a blob.
 */
#define COSC_LEVEL_TYPE_BLOB 'b'

/**
 * A flag indicating that all bundles and messages
 * should have a 32-bit size integer packet size prefix.
 * @note If this flag is not used the first bundle or message
 * in the buffer will have no size prefix.
 */
#define COSC_SERIAL_PSIZE 1

/**
 * A flag indicating that the serial is owned by a reader.
 */
#define COSC_SERIAL_READ 0x80

/**
 * Buffer overrun.
 */
#define COSC_EOVERRUN -2

/**
 * Size exceeds @ref COSC_SIZE_MAX.
 */
#define COSC_ESIZEMAX -3

/**
 * Invalid type.
 */
#define COSC_ETYPE -4

/**
 * Invalid packet size.
 */
#define COSC_EPSIZE -5

/**
 * Invalid argument or operation.
 */
#define COSC_EINVAL -6

/**
 * Writer or reader reached maximum level.
 */
#define COSC_ELEVELMAX -7

/**
 * Invalid operation for the current level type.
 */
#define COSC_ELEVELTYPE -8

/**
 * Trying to chain multiple bundles or messages without packet size flag.
 */
#define COSC_EPSIZEFLAG -9

/**
 * Trying to write or read a message member of the wrong or invalid type.
 */
#define COSC_EMSGTYPE -10

/**
 * A value.
 */
union cosc_value
{

    /**
     * 32-bit signed integer.
     */
    cosc_int32 i;

    /**
     * 32-bit floating point.
     */
    cosc_float32 f;

    /**
     * String and symbol.
     */
    struct
    {

        /**
         * A pointer to the string, when writing NULL indicates
         * an empty string, when reading the string is never NULL even
         * if the string has zero length.
         */
        const char *s;

        /**
         * When writing read at most this many bytes from @p s, when reading
         * this is the length excluding the zero terminator.
         */
        cosc_int32 length;

    } s;

    /**
     * Blob.
     */
    struct
    {

        /**
         * A pointer to the data, when writing NULL will fill write zeroes,
         * when reading NULL indicates a zero size blob.
         */
        const void *b;

        /**
         * The size of the blob data in bytes.
         */
        cosc_int32 size;

    } b;

    /**
     * 64-bit signed integer.
     */
    cosc_int64 h;

    /**
     * 64-bit unsigned integer (timetag).
     */
    cosc_uint64 t;

    /**
     * 64-bit floating point.
     */
    cosc_float64 d;

    /**
     * 8-bit unsigned integer (ASCII character).
     */
    cosc_int32 c;

    /**
     * 32-bit unsigned integer.
     */
    cosc_uint32 r;

    /**
     * MIDI message.
     * @note The last byte isn't really used in MIDI.
     */
    unsigned char m[4];

};

/**
 * A message.
 */
struct cosc_message
{

    /**
     * The address.
     */
    const char *address;

    /**
     * When writing read at most this many bytes from the address,
     * when reading the length excluding the zero terminator.
     */
    cosc_int32 address_n;

    /**
     * The typetag.
     */
    const char *typetag;

    /**
     * When writing read at most this many bytes from the typetag,
     * when reading the length excluding the zero terminator.
     */
    cosc_int32 typetag_n;

    /**
     * Const safety.
     */
    union
    {

        /**
         * Write values.
         */
        const union cosc_value *write;

        /**
         * Read values.
         */
        union cosc_value *read;

    } values;

    /**
     * Write/read at most this many values.
     */
    cosc_int32 values_n;

};

/**
 * Used to manage the levels of a serializer.
 */
struct cosc_level
{

    /**
     * The level type.
     */
    cosc_int32 type;

    /**
     * The buffer byte offset of the level start.
     */
    cosc_int32 start;

    /**
     * The level byte size.
     * @note When writing this starts at zero and increase when something
     * is written to the level, but when reading this is the packet size.
     */
    cosc_int32 size;

    /**
     * The packet size, will always be 0 when reading or -1 if psize
     * is note used at all.
     */
    cosc_int32 psize;

    /**
     * The buffer byte offset of the typetag start (from the beginning
     * of the buffer).
     * @note Only applicable if type is message.
     */
    cosc_int32 ttstart;

    /**
     * The buffer byte offset of the typetag end, including it's
     * zero terminator and pad zero bytes (i.e where the data payload starts).
     * @note Only applicable if type is message.
     */
    cosc_int32 ttend;

    /**
     * The current position in the typetag.
     * @note Only applicable if type is message.
     */
    cosc_int32 ttindex;

};

/**
 * Common members for both writers and readers.
 */
struct cosc_serial
{

    /**
     * Maximum number of bytes the buffer has available.
     */
    cosc_int32 buffer_size;

    /**
     * The number of written or read bytes.
     */
    cosc_int32 size;

    /**
     * Flags to control operations.
     */
    cosc_uint32 flags;

    /**
     * A pointer to an array of levels.
     */
    struct cosc_level *levels;

    /**
     * Maximum level.
     */
    cosc_int32 level_max;

    /**
     * Current level.
     */
    cosc_int32 level;

};

/**
 * Used to serialize OSC.
 */
struct cosc_writer
{

    /**
     * Base serializer.
     */
    struct cosc_serial serial;

    /**
     * Buffer data.
     */
    unsigned char *buffer;

};

/**
 * Used to deserialize OSC.
 */
struct cosc_reader
{

    /**
     * Base serializer.
     */
    struct cosc_serial serial;

    /**
     * Buffer data.
     */
    const unsigned char *buffer;

};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Feature test for 64-bit integer support.
 * @returns Non-zero if cosc was built with 64-bit integer support.
 */
COSC_API cosc_int32 cosc_feature_int64(void);

/**
 * Feature test for 32-bit float support.
 * @returns Non-zero if cosc was built with 32-bit float support.
 */
COSC_API cosc_int32 cosc_feature_float32(void);

/**
 * Feature test for 64-bit float support.
 * @returns Non-zero if cosc was built with 64-bit float support.
 */
COSC_API cosc_int32 cosc_feature_float64(void);

/**
 * Feature test for endian swapping.
 * @returns Non-zero if cosc was built with endian swapping.
 */
COSC_API cosc_int32 cosc_feature_swap(void);

/**
 * Feature test for array support.
 * @returns Non-zero if cosc was built with array support.
 */
COSC_API cosc_int32 cosc_feature_array(void);

/**
 * Feature test for pattern support.
 * @returns Non-zero if cosc was built with pattern support.
 */
COSC_API cosc_int32 cosc_feature_pattern(void);

/**
 * Feature test for writer support.
 * @returns Non-zero if cosc was built with writer support.
 */
COSC_API cosc_int32 cosc_feature_writer(void);

/**
 * Feature test for reader support.
 * @returns Non-zero if cosc was built with reader support.
 */
COSC_API cosc_int32 cosc_feature_reader(void);

/**
 * Check if an address character is valid.
 * @param c The character.
 * @returns Non-zero if valid or zero if invalid.
 */
COSC_API cosc_int32 cosc_address_char_validate(
    cosc_int32 c
);

/**
 * Scan address for invalid characters.
 * @param address The address.
 * @param address_n Read at most this many bytes from @p address.
 * @param[out] invalid If non-NULL and the function returns 0
 * the index of the invalid character is stored here.
 * @returns Non-zero if valid or zero if invalid.
 * @note If the address is valid the value stored to @p invalid
 * will be -1.
 * @note Invalid characters are "#*,?[]{}" and any ASCII <= 32.
 * @see cosc_typetag_validate() and cosc_pattern_validate().
 */
COSC_API cosc_int32 cosc_address_validate(
    const char *address,
    cosc_int32 address_n,
    cosc_int32 *invalid
);

/**
 * Check if an typetag character is valid.
 * @param c The character.
 * @returns Non-zero if valid or zero if invalid.
 */
COSC_API cosc_int32 cosc_typetag_char_validate(
    cosc_int32 c
);

/**
 * Scan typetag for invalid characters.
 * @param typetag The typetag.
 * @param typetag_n Read at most this many bytes from @p typetag.
 * @param[out] invalid If non-NULL and the function returns 0
 * the index of the invalid character is stored here.
 * @returns Non-zero if valid or zero if invalid.
 * @note If the typetag is valid or @p s_n <= 0 or @p s is empty the value
 * stored to @p invalid will be -1.
 * @note If the function returns 0 and the value stored to @p invalid
 * is >= @p s_n it means the typetag ended unexpectedly, probably
 * due to an unclosed array '[' marker.
 * @note Typetags must always start with ',', even if they contain
 * no types.
 * @see cosc_address_validate() and cosc_pattern_validate().
 */
COSC_API cosc_int32 cosc_typetag_validate(
    const char *typetag,
    cosc_int32 typetag_n,
    cosc_int32 *invalid
);

/**
 * Scan typetag for types with payload, ignoring the comma prefix, array
 * syntax and types T, F, N and I.
 * @param[out] s If non-NULL store the payload types here.
 * @param s_n Store at most this many bytes to @p s.
 * @param typetag The typetag.
 * @param typetag_n Read at most this many bytes from @p typetag.
 * @param[out] array_members If non-NULL store the number of payload types
 * that belong to an array, i.e is inside the array syntax.
 * @returns The total number of types with payload found in @p typetag.
 * @note This function does not exit early or report any errors for
 * invalid typetags.
 * @note This function will zero terminate @p s, make sure it is wide
 * enough to accept all types + 1.
 * @note The function may return a value greater than or equal to @p s_n
 * indicating that some types may have been truncated to fit @p s.
 * @note If cosc was built with COSC_NOARRAY the value stored
 * to @p array_members will always be 0.
 */
COSC_API cosc_int32 cosc_typetag_payload(
    char *s,
    cosc_int32 s_n,
    const char *typetag,
    cosc_int32 typetag_n,
    cosc_int32 *array_members
);

#ifndef COSC_NOPATTERN

/**
 * Check if an pattern character is valid.
 * @param c The character.
 * @returns Non-zero if valid or zero if invalid.
 * @remark This function is not available if COSC_NOPATTERN
 * was defined when compiling.
 */
COSC_API cosc_int32 cosc_pattern_char_validate(
    cosc_int32 c
);

/**
 * Scan matching pattern for invalid characters.
 * @param s The pattern.
 * @param s_n Read at most this many bytes from @p s.
 * @param[out] invalid If non-NULL and the function returns 0
 * the index of the invalid character is stored here.
 * @returns Non-zero if valid or zero if invalid.
 * @note If the pattern is valid or @p s_n <= 0 or @p s is empty the value
 * stored to @p invalid will be -1.
 * @note If the function returns 0 and the value stored to @p invalid
 * is >= @p s_n it means the pattern ended unexpectedly, probably
 * due to an unclosed '[' or '{' marker.
 * @see cosc_address_validate(), cosc_typetag_validate() and
 * cosc_pattern_match().
 * @note When matching typetags the comma prefix and any array
 * syntax characters are removed from the matching.
 * @remark This function is not available if COSC_NOPATTERN
 * was defined when compiling.
 */
COSC_API cosc_int32 cosc_pattern_validate(
    const char *s,
    cosc_int32 s_n,
    cosc_int32 *invalid
);

/**
 * Match an address or typetag.
 * @param s The address or typetag to match with.
 * @param s_n Read at most this many bytes from @p s.
 * @param pattern The matching pattern.
 * @param pattern_n Read at most this many bytes from @p pattern.
 * @returns Non-zero when matching or zero for no match.
 * @note Because square brackets collide with the array syntax
 * of OSC typetags the array brackets are ignored when matching.
 * @note For typetags the comma prefix and array brackets are ignored
 * and should be omitted from the @p pattern.
 * @remark This function is not available if COSC_NOPATTERN
 * was defined when compiling.
 *
 * Pattern syntax:
 *
 * - `*` match zero or more characters.
 * - `[]` match any character inside square brackets.
 * - `{}` match any comma separated string inside curly brackets.
 * - '?' match a single character.
 *
 * Extended syntax for typetags, not part of the OSC specification:
 *
 * - '#' for typetags match a numeric value ('i', 'f', 'r', 'h', 't' and 'd').
 * - '#' for addresses match a numeric, base-10 digit (0-9).
 * - 'B' match a boolean ('T' or 'F').
 *
 * When matching addresses '#' is simply ignored.
 */
COSC_API cosc_int32 cosc_pattern_match(
    const char *s,
    cosc_int32 s_n,
    const char *pattern,
    cosc_int32 pattern_n
);

/**
 * Match an address and typetag.
 * @param buffer The buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param apattern The matching pattern for the address.
 * @param apattern_n Read at most this many bytes from @p apattern.
 * @param tpattern The matching pattern for the typetag.
 * @param tpattern_n Read at most this many bytes from @p apattern.
 * @param prefix Non-zero to expect a message with a 32-bit signed
 * size integer prefix.
 * @returns Non-zero when matching or zero for no match.
 * @remark This function is not available if COSC_NOPATTERN
 * was defined when compiling.
 */
COSC_API cosc_int32 cosc_signature_match(
    const void *buffer,
    cosc_int32 size,
    const char *apattern,
    cosc_int32 apattern_n,
    const char *tpattern,
    cosc_int32 tpattern_n,
    cosc_int32 prefix
);

#endif /* !COSC_NOPATTERN */

/**
 * Convert a timetag to seconds and nanoseconds.
 * @param timetag The timetag.
 * @param[out] nanos If non-NULL store the nanoseconds here.
 * @returns The seconds.
 * @note Because a timetag's fraction of a second is greater
 * than a nanosecond the returned @p nanos will have
 * a loss of precision.
 */
COSC_API cosc_uint32 cosc_timetag_to_time(
    cosc_uint64 timetag,
    cosc_uint32 *nanos
);

/**
 * Convert time to a timetag.
 * @param seconds The time in seconds.
 * @param nanos The time nanos.
 * @returns The timetag.
 * @warning Beware when COSC_NOINT64 is defined! Because I failed to implement
 * a 64-bit division using only 32 bit integers the resolution of @p nanos
 * is reduced to 1/10th milliseconds instead of 1/4 nanoseconds.
 */
COSC_API cosc_uint64 cosc_timetag_from_time(
    cosc_uint32 seconds,
    cosc_uint32 nanos
);

/**
 * Write a 32-bit, big endian unsigned integer.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param value The value.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 */
COSC_API cosc_int32 cosc_write_uint32(
    void *buffer,
    cosc_int32 size,
    cosc_uint32 value
);

/**
 * Read a 32-bit big endian unsigned integer.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 */
COSC_API cosc_int32 cosc_read_uint32(
    const void *buffer,
    cosc_int32 size,
    cosc_uint32 *value
);

/**
 * Write a 32-bit big endian signed integer.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param value The value.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 */
COSC_API cosc_int32 cosc_write_int32(
    void *buffer,
    cosc_int32 size,
    cosc_int32 value
);

/**
 * Read a 32-bit big endian signed integer.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 */
COSC_API cosc_int32 cosc_read_int32(
    const void *buffer,
    cosc_int32 size,
    cosc_int32 *value
);

/**
 * Write a 32-bit big endian floating point.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param value The value.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 */
COSC_API cosc_int32 cosc_write_float32(
    void *buffer,
    cosc_int32 size,
    cosc_float32 value
);

/**
 * Read a 32-bit big endian floating point.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 */
COSC_API cosc_int32 cosc_read_float32(
    const void *buffer,
    cosc_int32 size,
    cosc_float32 *value
);

/**
 * Write a 64-bit, big endian unsigned integer.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param value The value.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 */
COSC_API cosc_int32 cosc_write_uint64(
    void *buffer,
    cosc_int32 size,
    cosc_uint64 value
);

/**
 * Read a 64-bit big endian unsigned integer.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 */
COSC_API cosc_int32 cosc_read_uint64(
    const void *buffer,
    cosc_int32 size,
    cosc_uint64 *value
);

/**
 * Write a 64-bit big endian signed integer.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param value The value.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 */
COSC_API cosc_int32 cosc_write_int64(
    void *buffer,
    cosc_int32 size,
    cosc_int64 value
);

/**
 * Read a 64-bit big endian signed integer.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 */
COSC_API cosc_int32 cosc_read_int64(
    const void *buffer,
    cosc_int32 size,
    cosc_int64 *value
);

/**
 * Write a 64-bit big endian floating point.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param value The value.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 */
COSC_API cosc_int32 cosc_write_float64(
    void *buffer,
    cosc_int32 size,
    cosc_float64 value
);

/**
 * Read a 64-bit big endian floating point.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 */
COSC_API cosc_int32 cosc_read_float64(
    const void *buffer,
    cosc_int32 size,
    cosc_float64 *value
);

/**
 * Write a string.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param value The string.
 * @param value_n Read at most this many bytes from @p s.
 * @param[out] length If non-NULL and the function does not return a
 * negative error code the length of the string, excluding the zero
 * terminator, is stored here.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 * - @ref COSC_ESIZEMAX if the string exceeds @ref COSC_SIZE_MAX.
 */
COSC_API cosc_int32 cosc_write_string(
    void *buffer,
    cosc_int32 size,
    const char *value,
    cosc_int32 value_n,
    cosc_int32 *length
);

/**
 * Read a string.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] value If non-NULL store the string here.
 * @param value_n Store at most this many bytes to @p value.
 * @param[out] length If non-NULL and the function does not return a
 * negative error code the length of the string, excluding the zero
 * terminator, is stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 */
COSC_API cosc_int32 cosc_read_string(
    const void *buffer,
    cosc_int32 size,
    char *value,
    cosc_int32 value_n,
    cosc_int32 *length
);

/**
 * Write a blob.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param value The blob data, may be NULL to fill with zeroes.
 * @param value_n The size of the blob data.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 * - @ref COSC_ESIZEMAX if the blob exceeds @ref COSC_SIZE_MAX.
 */
COSC_API cosc_int32 cosc_write_blob(
    void *buffer,
    cosc_int32 size,
    const void *value,
    cosc_int32 value_n
);

/**
 * Read a blob.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code store the blob data here.
 * @param value_n Store at most this many bytes to @p value.
 * @param[out] data If non-NULL and the function does not return a negative
 * error code a pointer to the first byte of the blob data is stored here,
 * or NULL if the blob has zero bytes.
 * @param[out] data_size If non-NULL and the function does not return a
 * negative error code the size of the blob data is stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 * - @ref COSC_ESIZEMAX if the @p data_size exceeds @ref COSC_SIZE_MAX.
 * - @ref COSC_EPSIZE if the @p data_size is invalid.
 */
COSC_API cosc_int32 cosc_read_blob(
    const void *buffer,
    cosc_int32 size,
    void *value,
    cosc_int32 value_n,
    const void **data,
    cosc_int32 *data_size
);

/**
 * Write an ASCII character.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param value The value.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 */
COSC_API cosc_int32 cosc_write_char(
    void *buffer,
    cosc_int32 size,
    cosc_int32 value
);

/**
 * Read an ASCII character.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 */
COSC_API cosc_int32 cosc_read_char(
    const void *buffer,
    cosc_int32 size,
    cosc_int32 *value
);

/**
 * Write a MIDI message.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param value The value or NULL to fill the message with zeroes.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 * @note The last byte isn't used by actual MIDI.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 */
COSC_API cosc_int32 cosc_write_midi(
    void *buffer,
    cosc_int32 size,
    const unsigned char value[4]
);

/**
 * Read an ASCII character.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 * @note The last byte isn't used by actual MIDI.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 */
COSC_API cosc_int32 cosc_read_midi(
    const void *buffer,
    cosc_int32 size,
    unsigned char value[4]
);

/**
 * Write an OSC bundle head.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param timetag The timetag of the bundle.
 * @param psize If >= 0 write a signed 32-bit integer at the start describing
 * the byte size of the message or a negative value if there is no
 * packet size descriptor prefix.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 * @note You can use cosc_write_int32() to store a new @p psize at
 * the beginning of the buffer at a later time.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 * - @ref COSC_ESIZEMAX if @p psize + the bundle data exceeds @ref COSC_SIZE_MAX.
 */
COSC_API cosc_int32 cosc_write_bundle(
    void *buffer,
    cosc_int32 size,
    cosc_uint64 timetag,
    cosc_int32 psize
);

/**
 * Read an OSC bundle head.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] timetag If non-NULL and the function does not return
 * a negative error code the timetag is stored here.
 * @param[out] psize If non-NULL and a signed 32-bit integer at the
 * start describing the byte size of the message is stored here,
 * NULL if there is no packet size descriptor prefix.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 * @note If @p size is non-NULL and the @p size < 4 the value
 * stored to @p size will be 0.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 * - @ref COSC_EPSIZE if @p psize is invalid.
 * - @ref COSC_ETYPE if @p buffer does not have a "#bundle" head.
 */
COSC_API cosc_int32 cosc_read_bundle(
    const void *buffer,
    cosc_int32 size,
    cosc_uint64 *timetag,
    cosc_int32 *psize
);

/**
 * Write an OSC message signature.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param address The address.
 * @param address_n Read at most this many bytes from @p address.
 * @param typetag The typetag.
 * @param typetag_n Read at most this many bytes from @p typetag.
 * @param psize If >= 0 write a signed 32-bit integer at the start describing
 * the byte size of the message or a negative value if there is no
 * size descriptor prefix.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 * @note You can use cosc_write_int32() to store a new @p psize at
 * the beginning of the buffer at a later time.
 * @note This function does NOT validate @p address or @p typetag.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 * - @ref COSC_ESIZEMAX if the size exceeds @ref COSC_SIZE_MAX.
 */
COSC_API cosc_int32 cosc_write_signature(
    void *buffer,
    cosc_int32 size,
    const char *address,
    cosc_int32 address_n,
    const char *typetag,
    cosc_int32 typetag_n,
    cosc_int32 psize
);

/**
 * Read an OSC message signature.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] address If non-NULL and the function does not return
 * a negative error code the address pointer is stored here.
 * @param[out] address_n If non-NULL and the function does not return
 * a negative error code the address length, excluding the zero terminator,
 * is stored here.
 * @param[out] typetag If non-NULL and the function does not return
 * a negative error code the typetag pointer is stored here.
 * @param[out] typetag_n If non-NULL and the function does not return
 * a negative error code the typetag length, excluding the zero terminator,
 * is stored here.
 * @param[out] psize If non-NULL and the function does not return
 * a negative error code a signed 32-bit integer at the start describing
 * the byte size of the message is stored here, NULL if there is no
 * size descriptor prefix.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 * @note This function does NOT validate @p address or @p typetag.
 *
 * Possible error codes:
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 * - @ref COSC_ESIZEMAX if the size exceeds @ref COSC_SIZE_MAX.
 * - @ref COSC_EPSIZE if @p psize is invalid.
 */
COSC_API cosc_int32 cosc_read_signature(
    const void *buffer,
    cosc_int32 size,
    const char **address,
    cosc_int32 *address_n,
    const char **typetag,
    cosc_int32 *typetag_n,
    cosc_int32 *psize
);

/**
 * Write an OSC value.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param type The type.
 * @param value The value or NULL for zeroed/empty.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 * - @ref COSC_ESIZEMAX if @p size > @ref COSC_SIZE_MAX.
 * - @ref COSC_ETYPE if @p type is invalid.
 */
COSC_API cosc_int32 cosc_write_value(
    void *buffer,
    cosc_int32 size,
    char type,
    const union cosc_value *value
);

/**
 * Read an OSC value.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param type The type.
 * @param[out] value If non-NULL and the functon does not return a
 * negative error code store the read values here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 * - @ref COSC_ESIZEMAX if @p size > @ref COSC_SIZE_MAX.
 * - @ref COSC_ETYPE if @p type is invalid.
 */
COSC_API cosc_int32 cosc_read_value(
    const void *buffer,
    cosc_int32 size,
    char type,
    union cosc_value *value
);

/**
 * Write OSC values.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param typetag The typetag.
 * @param typetag_n Read at most this many bytes from @p typetag.
 * @param values The values.
 * @param values_n Read at most this many members from @p values.
 * @param[out] value_count If non-NULL the number of written values is
 * stored here.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 * @note If @p values_n is less than the values specified by @p typetag
 * the remaining values are written as zero/empty.
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 * - @ref COSC_ESIZEMAX if @p size > @ref COSC_SIZE_MAX.
 * - @ref COSC_ETYPE if @p typetag is invalid.
 */
COSC_API cosc_int32 cosc_write_values(
    void *buffer,
    cosc_int32 size,
    const char *typetag,
    cosc_int32 typetag_n,
    const union cosc_value *values,
    cosc_int32 values_n,
    cosc_int32 *value_count
);

/**
 * Read OSC values.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param typetag The typetag.
 * @param typetag_n Read at most this many bytes from @p typetag.
 * @param[out] values If non-NULL store the read values here.
 * @param values_n Store at most this many members to @p values.
 * @param[out] value_count If non-NULL the number of read values is
 * stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 * @note If @p values_n is less than the values specified by @p typetag
 * the remaining values are discarded.
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 * - @ref COSC_ESIZEMAX if @p size > @ref COSC_SIZE_MAX.
 * - @ref COSC_ETYPE if @p type is invalid.
 */
COSC_API cosc_int32 cosc_read_values(
    const void *buffer,
    cosc_int32 size,
    const char *typetag,
    cosc_int32 typetag_n,
    union cosc_value *values,
    cosc_int32 values_n,
    cosc_int32 *value_count
);

/**
 * Write an OSC message.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param message The message or NULL for an empty message.
 * @param use_psize Non-zero to add a 32-bit packet size integer before
 * the actual message.
 * @param[out] value_count If non-NULL the number of written values is
 * stored here.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 * @note The message address is NOT validated.
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 * - @ref COSC_ESIZEMAX if @p size > @ref COSC_SIZE_MAX.
 * - @ref COSC_ETYPE if message typetag is invalid.
 */
COSC_API cosc_int32 cosc_write_message(
    void *buffer,
    cosc_int32 size,
    const struct cosc_message *message,
    cosc_int32 use_psize,
    cosc_int32 *value_count
);

/**
 * Read an OSC message.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param message The message or NULL to discard the message.
 * @param psize If non-NULL read a 32 bit packet size integer
 * before the message and store it here, may be 0 if the function
 * fails before reading it. NULL if you are not expecting any packet
 * size integer at the start of the buffer.
 * @param[out] value_count If non-NULL the number of read values is
 * stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 * @note The message address is NOT validated.
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 * - @ref COSC_ESIZEMAX if @p size > @ref COSC_SIZE_MAX.
 * - @ref COSC_ETYPE if mesage typetag is invalid.
 * - @ref COSC_EPSIZE if the size
 */
COSC_API cosc_int32 cosc_read_message(
    const void *buffer,
    cosc_int32 size,
    struct cosc_message *message,
    cosc_int32 *psize,
    cosc_int32 *value_count
);

#if !defined(COSC_NOSTDLIB) && !defined(COSC_NODUMP)

/**
 * Dump an OSC value to string.
 * @param[out] s If non-NULL store to this string.
 * @param n Store at most this many bytes to @p s.
 * @param type The OSC typetag type.
 * @param value The value.
 * @returns The length of the string, excluding the zero terminator.
 * @note The returned length may be greater than or equal to @p n indicating
 * that the string was truncated.
 * @remark This function is not available if COSC_NOSTDLIB or COSC_NODUMP
 * were defined when compiling.
 */
COSC_API cosc_int32 cosc_value_dump(
    char *s,
    cosc_int32 n,
    char type,
    const union cosc_value *value
);

/**
 * Dump an OSC message to string.
 * @param[out] s If non-NULL store to this string.
 * @param n Store at most this many bytes to @p s.
 * @param message The message.
 * @returns The length of the string, excluding the zero terminator.
 * @note The returned length may be greater than or equal to @p n indicating
 * that the string was truncated.
 * @remark This function is not available if COSC_NOSTDLIB or COSC_NODUMP
 * were defined when compiling.
 */
COSC_API cosc_int32 cosc_message_dump(
    char *s,
    cosc_int32 n,
    const struct cosc_message *message
);

#endif /* !COSC_NOSTDLIB && !COSC_NODUMP */

#if !defined(COSC_NOWRITER) && !defined(COSC_NOREADER)

// FIXME: maybe not have functions directly for serial.

#endif /* !COSC_NOWRITER && !COSC_NOREADER */

#ifndef COSC_NOWRITER

/**
 * Helper to setup a writer.
 * @param[out] writer The writer struct to initialize.
 * @param buffer The buffer bytes will be written to, must not
 * be NULL.
 * @param buffer_size Store at most this many bytes to @p buffer.
 * @param levels A pointer to an array with at least one member.
 * @param level_max The maximum number of levels, will be implicitly
 * clamped to a minimum value of 1.
 * @param flags Flags to control the behavior of the writer,
 * see the COSC_SERIAL_* macros.
 * @note @ref COSC_SERIAL_READ is implicitly removed from @p flags.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
COSC_API void cosc_writer_setup(
    struct cosc_writer *writer,
    void *buffer,
    cosc_int32 buffer_size,
    struct cosc_level *levels,
    cosc_int32 level_max,
    cosc_uint32 flags
);

/**
 * Reset the writer levels and sizes, making it available for
 * completely new OSC data.
 * @param writer The writer.
 * @remark This function is not available if both COSC_NOWRITER was
 * defined when compiling.
 */
COSC_API void cosc_writer_reset(
    struct cosc_writer *writer
);

/**
 * Get the size of the buffer used by the writer.
 * @param writer The writer.
 * @returns The buffer size in bytes.
 * @remark This function is not available if both COSC_NOWRITER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_writer_get_buffer_size(
    const struct cosc_writer *writer
);

/**
 * Get the number of written bytes including any open bundles,
 * messages and blobs.
 * @param writer The writer.
 * @returns The written/read byte size.
 * @remark This function is not available if both COSC_NOWRITER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_writer_get_size(
    const struct cosc_writer *writer
);

/**
 * Get the maximum number of levels the writer has.
 * @param writer The writer.
 * @returns The maximum number of levels the writer has.
 * @remark This function is not available if both COSC_NOWRITER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_writer_get_level_max(
    const struct cosc_writer *writer
);

/**
 * Get the number of open levels of the writer.
 * @param writer The writer.
 * @returns The current level of the writer, will be 0
 * if no level has been started.
 * @remark This function is not available if both COSC_NOWRITER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_writer_get_level(
    const struct cosc_writer *writer
);

/**
 * Get the byte size of the current level of the writer.
 * @param writer The writer.
 * @returns The byte size of the current level of the writer, will be 0
 * if no level has been started.
 * @remark This function is not available if both COSC_NOWRITER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_writer_get_level_size(
    const struct cosc_writer *writer
);

/**
 * Get the type of the current level of the writer.
 * @param writer The writer.
 * @returns The type of the current level of the writer, will be 0
 * if no level has been started.
 * @see @ref COSC_LEVEL_TYPE_BUNDLE, @ref COSC_LEVEL_TYPE_MSSAGE
 * and @ref COSC_LEVEL_TYPE_BLOB.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_writer_get_level_type(
    const struct cosc_writer *writer
);

/**
 * Get the message typetag type of the current level of the writer.
 * @param writer The writer.
 * @returns The type of the current level of the writer, will be 0
 * if no level has been started or if the current level is not a message.
 * @note If this function returns 0 it means the message typetag
 * has ended and there are no more types left.
 * @remark This function is not available if both COSC_NOWRITER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_writer_get_level_msgtype(
    const struct cosc_writer *writer
);

/**
 * Get the buffer used by the writer.
 * @param writer The writer.
 * @returns A pointer to the start of the buffer.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
COSC_API void *cosc_writer_get_buffer(
    const struct cosc_writer *writer
);

/**
 * Set the buffer used by the writer.
 * @param writer The writer.
 * @param buffer The new buffer.
 * @param size The size of the new buffer.
 * @returns 0 on success or @ref COSC_EINVAL if the new
 * buffer size is maller than the current one.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_writer_set_buffer(
    struct cosc_writer *writer,
    void *buffer,
    cosc_int32 size
);

/**
 * Open a new bundle and add a level.
 * @param writer The writer.
 * @param timetag The timetag.
 * @returns The number of bytes added to the new level on success
 * or a negative error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_writer_open_bundle(
    struct cosc_writer *writer,
    cosc_uint64 timetag
);

/**
 * Open a new message and add a level.
 * @param writer The writer.
 * @param address The address.
 * @param address_n Read at most this many bytes from @p address.
 * @param typetag The typetag.
 * @param typetag_n Read at most this many bytes from @p typetag.
 * @returns The number of bytes added to the new level on success
 * or a negative error code on failure.
 * @note The message address is NOT validated.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_writer_open_message(
    struct cosc_writer *writer,
    const char *address,
    cosc_int32 address_n,
    const char *typetag,
    cosc_int32 typetag_n
);

/**
 * Open a new blob and add a level.
 * @param writer The writer.
 * @returns The number of bytes added to the new level on success
 * or a negative error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_writer_open_blob(
    struct cosc_writer *writer
);

/**
 * Close one or more levels of open bundles, messages and blobs.
 * @param writer The writer.
 * @param levels The number of levels to close.
 * @returns The number of bytes added to the buffer on success
 * or a negative error code on failure.
 * @note If any open levels are messages any remaining message members
 * will be written as empty/zeroed before the message closes.
 * @note If any open levels are blobs it will get padded before closed.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_writer_close(
    struct cosc_writer *writer,
    cosc_int32 levels
);

/**
 * Write an unsigned 32-bit integer to a message and on success
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param value The value.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_uint32(
    struct cosc_writer *writer,
    cosc_uint32 value
);

/**
 * Write a signed 32-bit integer to a message and on success
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param value The value.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_int32(
    struct cosc_writer *writer,
    cosc_int32 value
);

/**
 * Write a 32-bit float to a message and on success
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param value The value.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_float32(
    struct cosc_writer *writer,
    cosc_float32 value
);

/**
 * Write an unsigned 64-bit integer to a message and on success
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param value The value.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_uint64(
    struct cosc_writer *writer,
    cosc_uint64 value
);

/**
 * Write a signed 64-bit integer to a message and on success
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param value The value.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_int64(
    struct cosc_writer *writer,
    cosc_int64 value
);

/**
 * Write a 64-bit float to a message and on success
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param value The value.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_float64(
    struct cosc_writer *writer,
    cosc_float64 value
);

/**
 * Write a string to a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param value The value.
 * @param value_n Read at most this many bytes from @p value.
 * @param[out] length If non-NULL and the function does not return
 * a negative error code the length of the string, excluding the
 * zero terminator, is stored here.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_string(
    struct cosc_writer *writer,
    const char *value,
    cosc_int32 value_n,
    cosc_int32 *length
);

/**
 * Write a blob to a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param value The value or NULL to zero the blob data.
 * @param value_size The size of the blob.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_blob(
    struct cosc_writer *writer,
    const void *value,
    cosc_int32 value_size
);

/**
 * Write an ASCII character to a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param value The value.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_char(
    struct cosc_writer *writer,
    cosc_int32 value
);

/**
 * Write a MIDI message to a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param value The value or NULL to zero the MIDI message.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_midi(
    struct cosc_writer *writer,
    const unsigned char value[4]
);

/**
 * Write bytes to a message blob member and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param value The bytes or NULL to zero the bytes.
 * @param value_n The number of bytes to write.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_bytes(
    struct cosc_writer *writer,
    const void *value,
    cosc_int32 value_n
);

/**
 * Write a value to a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @param type The value type.
 * @param value The value or NULL to zero the value.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_value(
    struct cosc_writer *writer,
    cosc_int32 type,
    const union cosc_value *value
);

/**
 * Write an OSC message to the writer.
 * @param writer The writer.
 * @param message The message or NULL for an empty message.
 * @param[out] value_count If non-NULL the number of written values is
 * stored here.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @note The message address is NOT validated.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_writer_message(
    struct cosc_writer *writer,
    const struct cosc_message *message,
    cosc_int32 *value_count
);

/**
 * Write an empty value to a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param writer The writer.
 * @return The number of written bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOWRITER was defined
 * when compiling.
 */
cosc_int32 cosc_writer_skip(
    struct cosc_writer *writer
);

#endif /* !COSC_NOWRITER */

#ifndef COSC_NOREADER

/**
 * Helper to setup a reader.
 * @param[out] reader The reader struct to initialize.
 * @param buffer The buffer bytes will be read from, must not
 * be NULL.
 * @param buffer_size Store at most this many bytes to @p buffer.
 * @param levels A pointer to an array with at least one member.
 * @param level_max The maximum number of levels, will be implicitly
 * clamped to a minimum value of 1.
 * @param flags Flags to control the behavior of the reader,
 * see the COSC_SERIAL_* macros.
 * @note @ref COSC_SERIAL_READ is implicitly added to @p flags.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
COSC_API void cosc_reader_setup(
    struct cosc_reader *reader,
    const void *buffer,
    cosc_int32 buffer_size,
    struct cosc_level *levels,
    cosc_int32 level_max,
    cosc_uint32 flags
);

/**
 * Reset the reader levels and sizes, making it available for
 * completely new OSC data.
 * @param reader The reader.
 * @remark This function is not available if both COSC_NOREADER was
 * defined when compiling.
 */
COSC_API void cosc_reader_reset(
    struct cosc_reader *reader
);

/**
 * Get the size of the buffer used by the reader.
 * @param reader The reader.
 * @returns The buffer size in bytes.
 * @remark This function is not available if both COSC_NOREADER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_reader_get_buffer_size(
    const struct cosc_reader *reader
);

/**
 * Get the number of bytes read bytes.
 * @param reader The reader.
 * @returns The read/read byte size.
 * @remark This function is not available if both COSC_NOREADER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_reader_get_size(
    const struct cosc_reader *reader
);

/**
 * Get the packet size for the reader, if level or no packet size is
 * available the buffer size is returned.
 * @param reader The reader.
 * @returns The byte size of the current level of the reader, will be -1
 * if no level has been started or if no packet size is used.
 * @remark This function is not available if both COSC_NOREADER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_reader_get_psize(
    const struct cosc_reader *reader
);

/**
 * Get the maximum number of levels the reader has.
 * @param reader The reader.
 * @returns The maximum number of levels the reader has.
 * @remark This function is not available if both COSC_NOREADER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_reader_get_level_max(
    const struct cosc_reader *reader
);

/**
 * Get the number of open levels of the reader.
 * @param reader The reader.
 * @returns The current level of the reader, will be 0
 * if no level has been started.
 * @remark This function is not available if both COSC_NOREADER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_reader_get_level(
    const struct cosc_reader *reader
);

/**
 * Get the byte size of the current level of the reader.
 * @param reader The reader.
 * @returns The byte size of the current level of the reader, will be 0
 * if no level has been started.
 * @remark This function is not available if both COSC_NOREADER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_reader_get_level_size(
    const struct cosc_reader *reader
);

/**
 * Get the packet size for the current level, if level or no packet size is
 * available -1 is returned.
 * @param reader The reader.
 * @returns The byte size of the current level of the reader, will be -1
 * if no level has been started or if no packet size is used.
 * @remark This function is not available if both COSC_NOREADER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_reader_get_level_psize(
    const struct cosc_reader *reader
);

/**
 * Get the type of the current level of the reader.
 * @param reader The reader.
 * @returns The type of the current level of the reader, will be 0
 * if no level has been started.
 * @see @ref COSC_LEVEL_TYPE_BUNDLE, @ref COSC_LEVEL_TYPE_MSSAGE
 * and @ref COSC_LEVEL_TYPE_BLOB.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_reader_get_level_type(
    const struct cosc_reader *reader
);

/**
 * Get the message typetag type of the current level of the reader.
 * @param reader The reader.
 * @returns The type of the current level of the reader, will be 0
 * if no level has been started or if the current level is not a message.
 * @note If this function returns 0 it means the message typetag
 * has ended and there are no more types left.
 * @remark This function is not available if both COSC_NOREADER was
 * defined when compiling.
 */
COSC_API cosc_int32 cosc_reader_get_level_msgtype(
    const struct cosc_reader *reader
);

/**
 * Get the buffer used by the reader.
 * @param reader The reader.
 * @returns A pointer to the start of the buffer.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
COSC_API const void *cosc_reader_get_buffer(
    const struct cosc_reader *reader
);

/**
 * Open a new bundle and add a level.
 * @param reader The reader.
 * @param[out] timetag If non-NULL and the function does not return
 * a negative error code the timetag is stored here.
 * @param[out] timetag If non-NULL and the function does not return
 * a negative error code the timetag is stored here.
 * @param[out] psize If non-NULL and the function does not return
 * a negative error code the bundle size is stored here. If this is
 * a single bundle read without the @ref COSC_SERIAL_PSIZE flag
 * the value stored is 0.
 * @returns The number of bytes increased to the new level on success
 * or a negative error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_reader_open_bundle(
    struct cosc_reader *reader,
    cosc_uint64 *timetag,
    cosc_int32 *psize
);

/**
 * Open a new message and add a level.
 * @param reader The reader.
 * @param[out] address If non-NULL and the function does not return
 * a negative error code the address pointer is stored here.
 * @param[out] address_n If non-NULL and the function does not return
 * a negative error code the address length, excluding the zero terminator,
 * is stored here.
 * @param[out] typetag If non-NULL and the function does not return
 * a negative error code the typetag pointer is stored here.
 * @param[out] typetag_n If non-NULL and the function does not return
 * a negative error code the typetag length, excluding the zero terminator,
 * is stored here.
 * @param[out] psize If non-NULL and the function does not return
 * a negative error code the bundle size is stored here. If this is
 * a single message read without the @ref COSC_SERIAL_PSIZE flag
 * the value stored is 0.
 * @returns The number of bytes increased to the new level on success
 * or a negative error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_reader_open_message(
    struct cosc_reader *reader,
    const char **address,
    cosc_int32 *address_n,
    const char **typetag,
    cosc_int32 *typetag_n,
    cosc_int32 *psize
);

/**
 * Open a new blob and add a level.
 * @param reader The reader.
 * @param[out] psize If non-NULL and the function does not return
 * a negative error code the blob size is stored here.
 * @returns The number of bytes increased to the new level on success
 * or a negative error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_reader_open_blob(
    struct cosc_reader *reader,
    cosc_int32 *psize
);

/**
 * Close one or more levels of open bundles, messages and blobs.
 * @param reader The reader.
 * @param levels The number of levels to close, zero or a negative
 * value for all.
 * @returns The number of bytes increased to the buffer on success
 * or a negative error code on failure.
 * @note If any open levels are messages any remaining message members
 * will be read as empty/zeroed before the message closes.
 * @note If any open levels are blobs it will get padded before closed.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_reader_close(
    struct cosc_reader *reader,
    cosc_int32 levels
);

/**
 * Read an unsigned 32-bit integer from a message and on success
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out] value If non-NULL and the call is successful
 * the value is stored here.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_uint32(
    struct cosc_reader *reader,
    cosc_uint32 *value
);

/**
 * Read a signed 32-bit integer from a message and on success
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out] value If non-NULL and the call is successful
 * the value is stored here.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_int32(
    struct cosc_reader *reader,
    cosc_int32 *value
);

/**
 * Read a 32-bit float from a message and on success
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out] value If non-NULL and the call is successful
 * the value is stored here.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_float32(
    struct cosc_reader *reader,
    cosc_float32 *value
);

/**
 * Read an unsigned 64-bit integer from a message and on success
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out] value If non-NULL and the call is successful
 * the value is stored here.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_uint64(
    struct cosc_reader *reader,
    cosc_uint64 *value
);

/**
 * Read a signed 64-bit integer from a message and on success
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out] value If non-NULL and the call is successful
 * the value is stored here.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_int64(
    struct cosc_reader *reader,
    cosc_int64 *value
);

/**
 * Read a 64-bit float from a message and on success
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out] value If non-NULL and the call is successful
 * the value is stored here.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_float64(
    struct cosc_reader *reader,
    cosc_float64 *value
);

/**
 * Read a string from a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out value If non-NULL store the string here.
 * @param value_n Read at most this many bytes from @p value.
 * @param[ptr] ptr If non-NULL and the function does not return
 * a negative error code a pointer to the string is stored here.
 * @param[out] length If non-NULL and the function does not return
 * a negative error code the length of the string, excluding the
 * zero terminator, is stored here.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_string(
    struct cosc_reader *reader,
    char *value,
    cosc_int32 value_n,
    const char **ptr,
    cosc_int32 *length
);

/**
 * Read a blob from a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out] value If non-NULL store the data here.
 * @param value_n Store at most this many bytes to @p value.
 * @param[out] data If non-NULL and the function is successful store
 * a pointer to the data here.
 * @param[out] data_size If non-NULL and the call is successful
 * the byte size of the data is stored here.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_blob(
    struct cosc_reader *reader,
    void *value,
    cosc_int32 value_n,
    const void **data,
    cosc_int32 *data_size
);

/**
 * Read an ASCII character from a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out] value If non-NULL and the call is successful
 * the value is stored here.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_char(
    struct cosc_reader *reader,
    cosc_int32 *value
);

/**
 * Read a MIDI message from a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out] value If non-NULL and the call is successful
 * the value is stored here.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_midi(
    struct cosc_reader *reader,
    unsigned char value[4]
);

/**
 * Read bytes from a message blob member and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out] value If non-NULL store bytes here.
 * @param value_n The number of bytes to read.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_bytes(
    struct cosc_reader *reader,
    void *value,
    cosc_int32 value_n
);

/**
 * Read a value from a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @param[out] type If non-NULL and the function does not return a
 * negative error code the type is stored here.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_value(
    struct cosc_reader *reader,
    cosc_int32 *type,
    union cosc_value *value
);

/**
 * Read an OSC message from the reader.
 * @param reader The reader.
 * @param message The message or NULL to discard the message.
 * @param[out] value_count If non-NULL the number of read values is
 * stored here.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 * @note The message address is NOT validated.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
COSC_API cosc_int32 cosc_reader_message(
    struct cosc_reader *reader,
    struct cosc_message *message,
    cosc_int32 *value_count
);

/**
 * Read an skip a value in a message and on success proceed
 * to the next member of the message typetag.
 * proceed to the next member of the message typetag.
 * @param reader The reader.
 * @return The number of read bytes on success or a negative
 * error code on failure.
 * @remark This function is not available if COSC_NOREADER was defined
 * when compiling.
 */
cosc_int32 cosc_reader_skip(
    struct cosc_reader *reader
);

#endif /* !COSC_NOREADER */

#ifdef __cplusplus
}
#endif

#endif
