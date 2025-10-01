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
 * - COSC_NOEXTRAS to remove the address and typetag
 *   related functions.
 * - COSC_NOSWAP for no endian swapping.
 * - COSC_NOARRAY to remove the support for arrays.
 * - COSC_NOSTDINT to not include `stdint.h`.
 * - COSC_NODUMP to remove the dump functions.
 * - COSC_NO64 to typedef all 64-bit types as @ref cosc_64bits.
 * - COSC_NOFLOAT32 to typedef cosc_float32 as @ref cosc_uint32.
 * - COSC_NOFLOAT64 to typedef cosc_float64 as @ref cosc_uint64
 *   or @ref cosc_64bits if COSC_NO64 is also defined.
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
 * NOTE that type overrides for 64-bit types does not work
 * if COSC_NO64 is defined in which case all 64-bit types
 * will be typedef as @ref cosc_64bits.
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
#ifdef _MSC_VER
#define COSC_API __declspec(dllexport)
#else
#define COSC_API
#endif
#endif

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

#if defined(COSC_NO64) && !defined(COSC_NOFLOAT64)
#define COSC_NOFLOAT64
#endif

#ifndef COSC_NOSTDINT

#include <stdint.h>

#ifndef COSC_TYPE_INT32
#define COSC_TYPE_INT32 int32_t
#endif
#ifndef COSC_TYPE_UINT32
#define COSC_TYPE_UINT32 uint32_t
#endif

#ifndef COSC_NO64
#ifndef COSC_TYPE_INT64
#define COSC_TYPE_INT64 int64_t
#endif
#else
#define COSC_TYPE_INT64 struct cosc_64bits
#endif

#ifndef COSC_NO64
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

#ifndef COSC_NO64
#ifndef COSC_TYPE_INT64
#define COSC_TYPE_INT64 long long
#endif
#else
#define COSC_TYPE_INT64 struct cosc_64bits
#endif

#ifndef COSC_NO64
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
    char c;

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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef COSC_NOEXTRAS

/**
 * Check if an address character is valid.
 * @param c The character.
 * @returns Non-zero if valid or zero if invalid.
 */
COSC_API cosc_int32 cosc_address_char_validate(
    char c
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
 * @note This function will not be available if COSC_NOEXTRAS
 * is defined.
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
 * @note This function will not be available if COSC_NOEXTRAS
 * is defined.
 */
COSC_API cosc_int32 cosc_typetag_char_validate(
    char c
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
 * @note This function will not be available if COSC_NOEXTRAS
 * is defined.
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
 * @note This function will not be available if COSC_NOEXTRAS
 * is defined.
 */
COSC_API cosc_int32 cosc_typetag_payload(
    char *s,
    cosc_int32 s_n,
    const char *typetag,
    cosc_int32 typetag_n,
    cosc_int32 *array_members
);

/**
 * Check if an pattern character is valid.
 * @param c The character.
 * @returns Non-zero if valid or zero if invalid.
 * @note This function will not be available if COSC_NOEXTRAS
 * is defined.
 */
COSC_API cosc_int32 cosc_pattern_char_validate(
    char c
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
 * @note This function will not be available if COSC_NOEXTRAS
 * is defined.
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
 * @note This function will not be available if COSC_NOEXTRAS
 * is defined.
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
 * @note This function will not be available if COSC_NOEXTRAS
 * is defined.
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

#endif /* !COSC_NOEXTRAS */

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
 * @warning Beware when COSC_NO64 is defined! Because I failed to implement
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
 * @param s The string.
 * @param n Read at most this many bytes from @p s.
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
    const char *s,
    cosc_int32 n
);

/**
 * Read a string.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
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
    cosc_int32 *length
);

/**
 * Write a blob.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param data The blob data, may be NULL to fill with zeroes.
 * @param data_size The size of the blob data.
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
    const void *data,
    cosc_int32 data_size
);

/**
 * Read a blob.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param[out] data If non-NULL and the function does not return a
 * negative error code a pointer to the data is stored here.
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
    char value
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
    char *value
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
 * size descriptor prefix.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 * @note You can use cosc_write_int32() to store a new @p psize at
 * the beginning of the buffer at a later time.
 * @see COSC_PADALIGN().
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
 * NULL if there is no size descriptor prefix.
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
 * @see COSC_PADALIGN().
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
 * @param[out] value_offsets If non-NULL an array of integers with at least
 * as many members as the accepted number of values (@p values_n) is
 * expected and will be filled with the value offsets. Note that no more
 * than the value stored in @p value_count offsets are stored.
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
    cosc_int32 *value_count,
    cosc_int32 *value_offsets
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
 * @param[out] value_offsets If non-NULL an array of integers with at least
 * as many members as the accepted number of values (@p values_n) is
 * expected and will be filled with the value offsets. Note that no more
 * than the value stored in @p value_count offsets are stored.
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
    cosc_int32 *value_count,
    cosc_int32 *value_offsets
);

/**
 * Write an OSC message.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param message The message or NULL for an empty message.
 * @param prefix Non-zero to prefix the message with a 32-bit signed
 * size integer.
 * @param[out] value_count If non-NULL the number of written values is
 * stored here.
 * @param[out] value_offsets If non-NULL an array of integers with at least
 * as many members as the accepted number of values (@p values_n) is
 * expected and will be filled with the value offsets. Note that no more
 * than the value stored in @p value_count offsets are stored.
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
    cosc_int32 prefix,
    cosc_int32 *value_count,
    cosc_int32 *value_offsets
);

/**
 * Read an OSC message.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param message The message or NULL to discard the message.
 * @param prefix Non-zero to expect a message with a 32-bit signed
 * size integer prefix.
 * @param[out] value_count If non-NULL the number of read values is
 * stored here.
 * @param[out] value_offsets If non-NULL an array of integers with at least
 * as many members as the accepted number of values (message->values_n) is
 * expected and will be filled with the value offsets. Note that no more
 * than the value stored in @p value_count offsets are stored.
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
    cosc_int32 prefix,
    cosc_int32 *value_count,
    cosc_int32 *value_offsets
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
 * @note This function will not be available if COSC_NOSTDLIB, COSC_NOEXTRAS
 * or COSC_NODUMP are defined.
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
 * @note This function will not be available if COSC_NOSTDLIB, COSC_NOEXTRAS
 * or COSC_NODUMP are defined.
 */
COSC_API cosc_int32 cosc_message_dump(
    char *s,
    cosc_int32 n,
    const struct cosc_message *message
);

#endif /* !COSC_NOSTDLIB && !COSC_NODUMP */

#ifdef __cplusplus
}
#endif

#endif
