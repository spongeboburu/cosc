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
 * - COSC_NOSTDINT to not include `stdint.h` (or `cstdint` if C++).
 * - COSC_NODUMP to remove the dump functions.
 * - COSC_NOTIMETAG to remove timetag conversion functions.
 * - COSC_NOFLTCONV to remove float conversion functions.
 * - COSC_NOWRITER to remove the writer functions.
 * - COSC_NOREADER to remove the reader functions.
 * - COSC_NOINT64 to typedef `cosc_int64` and `cosc_uint64` as @ref cosc_64bits.
 * - COSC_NOFLOAT32 to typedef `cosc_float32` as @ref cosc_uint32.
 * - COSC_NOFLOAT64 to typedef `cosc_float64` as @ref cosc_64bits.
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
 * @example message.c
 */

/**
 * @example bundle.c
 */

/**
 * @example serial.c
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
 * @note Must be exactly 32 bits in width.
 */

/**
 * Used to typedef @ref cosc_int32.
 * @def COSC_TYPE_INT32
 * @note If defined it must be at both compile and include time.
 * @note Must be exactly 32 bits in width.
 */

/**
 * Used to typedef @ref cosc_float32.
 * @def COSC_TYPE_FLOAT32
 * @note If defined it must be at both compile and include time.
 * @note Must be exactly 32 bits in width.
 */

/**
 * Used to typedef @ref cosc_uint64.
 * @def COSC_TYPE_UINT64
 * @note If defined it must be at both compile and include time.
 * @note Must be exactly 64 bits in width.
 */

/**
 * Used to typedef @ref cosc_int64.
 * @def COSC_TYPE_INT64
 * @note If defined it must be at both compile and include time.
 * @note Must be exactly 64 bits in width.
 */

/**
 * Used to typedef @ref cosc_float64.
 * @def COSC_TYPE_FLOAT64
 * @note If defined it must be at both compile and include time.
 * @note Must be exactly 64 bits in width.
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
#define COSC_TYPE_FLOAT64 struct cosc_64bits
#endif

/**
 * Maximum value of a signed 32-bit integer.
 */
#define COSC_INT32MAX 2147483647

/**
 * Minimum value of a signed 32-bit integer.
 */
#define COSC_INT32MIN ((-2147483647)-1)

/**
 * Minimum value of an unsigned 32-bit integer.
 */
#define COSC_UINT32MAX 4294967295

/**
 * Maximum value of a signed 64-bit integer.
 */
#define COSC_INT64MAX 9223372036854775807LL

/**
 * Minimum value of a signed 64-bit integer.
 */
#define COSC_INT64MIN ((-9223372036854775807LL)-1)

/**
 * Minimum value of an unsigned 32-bit integer.
 */
#define COSC_UINT64MAX 18446744073709551615ULL

/**
 * Unsigned 32-bit integer.
 */
typedef COSC_TYPE_UINT32 cosc_uint32;

/**
 * Signed 32-bit integer.
 */
typedef COSC_TYPE_INT32 cosc_int32;

/**
 * Helper initializer when defining @ref cosc_64bits structs.
 * @param hi_ The 32 most significant bits.
 * @param lo_ The 32 least significant bits.
 * @returns Object initializer code.
 */
#define COSC_64BITS_INIT(hi_, lo_) {{(hi_), (lo_)}}

/**
 * Helper to set the members of a @ref cosc_64bits struct.
 * @param bits_ A pointer to the @ref cosc_64bits struct.
 * @param hi_ The 32 most significant bits.
 * @param lo_ The 32 least significant bits.
 */
#define COSC_64BITS_SET(bits_, hi_, lo_) do { COSC_64BITS_HI(bits_) = (hi_); COSC_64BITS_LO(bits_) = (lo_); } while (0)

/**
 * Helper to access the high member of a @ref cosc_64bits struct.
 * @param bits_ A pointer to the @ref cosc_64bits struct.
 */
#define COSC_64BITS_HI(bits_) ((bits_)->w[0])

/**
 * Helper to access the low member of a @ref cosc_64bits struct.
 * @param bits_ A pointer to the @ref cosc_64bits struct.
 */
#define COSC_64BITS_LO(bits_) ((bits_)->w[1])

/**
 * Initializer cosc_int64 or cosc_uint64 as zero.
 * @def COSC_INT64_INIT_ZERO
 * @param ptr A pointer to the cosc_int64 or cosc_uint64.
 */

/**
 * Set cosc_int64 or cosc_uint64 as zero.
 * @def COSC_INT64_SET_ZERO
 * @param ptr A pointer to the cosc_int64 or cosc_uint64.
 */

#ifndef COSC_NOINT64
#define COSC_INT64_INIT_ZERO 0
#define COSC_INT64_SET_ZERO(ptr) (*(ptr) = 0)
#else
#define COSC_INT64_INIT_ZERO COSC_64BITS_INIT(0, 0)
#define COSC_INT64_SET_ZERO(ptr) COSC_64BITS_SET(ptr, 0, 0)
#endif

/**
 * Initializer cosc_float64 or cosc_ufloat64 as zero.
 * @def COSC_FLOAT64_INIT_ZERO
 * @param ptr A pofloater to the cosc_float64 or cosc_ufloat64.
 */

/**
 * Set cosc_float64 or cosc_ufloat64 as zero.
 * @def COSC_FLOAT64_SET_ZERO
 * @param ptr A pofloater to the cosc_float64 or cosc_ufloat64.
 */

#ifndef COSC_NOFLOAT64
#define COSC_FLOAT64_INIT_ZERO 0
#define COSC_FLOAT64_SET_ZERO(ptr) (*(ptr) = 0)
#else
#define COSC_FLOAT64_INIT_ZERO COSC_64BITS_INIT(0, 0)
#define COSC_FLOAT64_SET_ZERO(ptr) COSC_64BITS_SET(ptr, 0, 0)
#endif

/**
 * Used when 64-bit types are not available.
 */
struct cosc_64bits
{

    /**
     * Most significant bits first then the least significant, i.e
     * big endian layout for the words.
     */
    cosc_uint32 w[2];

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
 * Tell the serial that the first bundle or message
 * is prefixed with a packet size.
 */
#define COSC_SERIAL_PSIZE 1

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
 * Macro to check if a serial is a writer.
 * @param serial_ A pointer to the serial.
 * @returns True or false.
 */
#define COSC_SERIAL_ISWRITER(serial_) ((serial_)->wbuffer != 0)

/**
 * Macro to check if a serial is a reader.
 * @param serial_ A pointer to the serial.
 * @returns True or false.
 */
#define COSC_SERIAL_ISREADER(serial_) ((serial_)->rbuffer != 0)

/**
 * Used to manage the levels of a serial.
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
     * The level max size.
     * @note When writing this is the buffer size - start,
     * when reading this is either buffer size - start or
     * the packet size.
     */
    cosc_int32 size_max;

    /**
     * The number of written or read bytes.
     */
    cosc_int32 size;

    /**
     * The buffer byte offset of the typetag start.
     * @note Only used if type is message.
     */
    cosc_int32 ttstart;

    /**
     * The buffer byte offset of the typetag end, including it's
     * zero terminator and pad zero bytes (i.e where the data payload starts).
     * @note Only applicable if type is message.
     */
    cosc_int32 ttend;

    /**
     * The current index in the typetag.
     * @note Only used if type is message.
     */
    cosc_int32 ttindex;

};

/**
 * A serial.
 */
struct cosc_serial
{

    /**
     * Write buffer, NULL if reading.
     */
    unsigned char *wbuffer;

    /**
     * Read buffer, NULL if writing.
     */
    const unsigned char *rbuffer;

    /**
     * Maximum number of bytes the buffer has available.
     */
    cosc_int32 buffer_size;

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

    /**
     * The number of written/read bytes, not including levels not yet ended.
     */
    cosc_int32 size;

    /**
     * Serial flags.
     */
    cosc_uint32 flags;

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
 * Feature test for timetag conversion support.
 * @returns Non-zero if cosc was built with timetag support.
 */
COSC_API cosc_int32 cosc_feature_timetag(void);

/**
 * Feature test for float conversion support.
 * @returns Non-zero if cosc was built with float support.
 */
COSC_API cosc_int32 cosc_feature_fltconv(void);

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
 * If big endian was detected when building this function returns non-zero,
 * otherwise zero.
 * @returns Non-zero if big endian was detected when building,
 * otherwise zero.
 */
COSC_API cosc_int32 cosc_big_endian(void);

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
 * - '#' for typetags match a numeric type ('i', 'f', 'r', 'h', 't' and 'd').
 * - '#' for addresses match a base-10 digit (0-9).
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

#ifndef COSC_NOTIMETAG

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
 */
COSC_API cosc_uint64 cosc_time_to_timetag(
    cosc_uint32 seconds,
    cosc_uint32 nanos
);

#endif /* !COSC_NOTIMETAG */

#ifndef COSC_NOFLTCONV

/**
 * Convert a 64-bit float to a 32-bit float.
 * @param value The 64-bit float.
 * @returns The 32-bit float.
 * @note This function is only useful if either or both COSC_NOFLOAT32
 * and COSC_FLOAT64 were defined when building.
 * @remark This function is not available if cosc was built
 * with COSC_NOFLTCONV.
 */
COSC_API cosc_float32 cosc_float64_to_float32(
    cosc_float64 value
);

/**
 * Convert a 32-bit float to a 64-bit float.
 * @param value The 32-bit float.
 * @returns The 64-bit float.
 * @note This function is only useful if either or both COSC_NOFLOAT32
 * and COSC_FLOAT64 were defined when building.
 * @remark This function is not available if cosc was built
 * with COSC_NOFLTCONV.
 */
COSC_API cosc_float64 cosc_float32_to_float64(
    cosc_float32 value
);

#endif /* !COSC_NOFLTCONV */

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
 * @param psize 0 for no packet size integer, < 0 to write a packet
 * size integer based on the signature data or > 0 to set the
 * packet size to a specific value.
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
 * - @ref COSC_EPSIZE if @p size > 0 and is invalid or too small.
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
 * @param psize 0 for no packet size integer, < 0 to write a packet
 * size integer based on the signature data or > 0 to set the
 * packet size to a specific value.
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
 * - @ref COSC_EPSIZE if @p size > 0 and is invalid or too small.
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
 * @param types The types, the starting comma may be omitted.
 * @param types_n Read at most this many bytes from @p types.
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
 * - @ref COSC_ETYPE if on of the @p types is invalid.
 */
COSC_API cosc_int32 cosc_write_values(
    void *buffer,
    cosc_int32 size,
    const char *types,
    cosc_int32 types_n,
    const union cosc_value *values,
    cosc_int32 values_n,
    cosc_int32 *value_count
);

/**
 * Read OSC values.
 * @param buffer Read bytes from this buffer.
 * @param size Read at most this many bytes from @p buffer.
 * @param types The types, the starting comma may be omitted.
 * @param types_n Read at most this many bytes from @p types.
 * @param[out] values If non-NULL store the read values here.
 * @param values_n Store at most this many members to @p values.
 * @param[out] value_count If non-NULL the number of read values is
 * stored here.
 * @param exit_early If non-zero and the @p types have an array specified
 * the function will stop reading more array members and exit early even
 * if there are more bytes left in @p buffer.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 * @note If @p values_n is less than the values specified by @p types
 * the remaining values are discarded.
 * @note The value of @p exit_early has no effect if COSC_NOARRAY was
 * defined when building.
 *
 * - @ref COSC_EOVERRUN if @p size is too small.
 * - @ref COSC_ESIZEMAX if @p size > @ref COSC_SIZE_MAX.
 * - @ref COSC_ETYPE if @p type is invalid.
 */
COSC_API cosc_int32 cosc_read_values(
    const void *buffer,
    cosc_int32 size,
    const char *types,
    cosc_int32 types_n,
    union cosc_value *values,
    cosc_int32 values_n,
    cosc_int32 *value_count,
    cosc_int32 exit_early
);

/**
 * Write an OSC message.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param message The message or NULL for an empty message.
 * @param psize 0 for no packet size integer, < 0 to write a packet
 * size integer based on the signature data or > 0 to set the
 * packet size to a specific value.
 * @param[out] value_count If non-NULL the number of written values is
 * stored here.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 * @note The message address is NOT validated.
 * @note If message.values_n is less than the values specified
 * in message.typetag the rest of the values will be written as zero/empty.
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 * - @ref COSC_ESIZEMAX if @p size > @ref COSC_SIZE_MAX.
 * - @ref COSC_ETYPE if message typetag is invalid.
 * - @ref COSC_EPSIZE if @p size > 0 and is invalid or too small.
 */
COSC_API cosc_int32 cosc_write_message(
    void *buffer,
    cosc_int32 size,
    const struct cosc_message *message,
    cosc_int32 psize,
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
 * @param exit_early If non-zero and the @p types have an array specified
 * the function will stop reading more array members and exit early even
 * if there are more bytes left in @p buffer.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 * @note The message address is NOT validated.
 * @note If message.values_n is less than the values specified
 * in message.typetag the rest of the values will be read and discarded.
 * @note The value of @p exit_early has no effect if COSC_NOARRAY was
 * defined when building.
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
    cosc_int32 *value_count,
    cosc_int32 exit_early
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

#if !defined(COSC_NOWRITER) || !defined(COSC_NOREADER)

/**
 * The maximum number of bytes that can be written/read to/from the buffer.
 * @param serial The serial.
 * @returns The maximum number of bytes to write/read.
 * @remark This function is not available if both COSC_NOWRITER
 * and COSC_NOREADER were defined when compiling.
 */
COSC_API cosc_int32 cosc_serial_get_buffer_size(
    const struct cosc_serial *serial
);

/**
 * Get the number of written or read bytes.
 * @param serial The serial.
 * @returns The number of written/read bytes.
 * @remark This function is not available if both COSC_NOWRITER
 * and COSC_NOREADER were defined when compiling.
 */
COSC_API cosc_int32 cosc_serial_get_size(
    const struct cosc_serial *serial
);

/**
 * Get the current typetag type of a message level.
 * @param serial The serial.
 * @returns The current message typetag type, @ref COSC_ELEVELTYPE
 * if the current leve is not a message or 0 if the typetag has
 * reached it's end.
 * @remark This function is not available if both COSC_NOWRITER
 * and COSC_NOREADER were defined when compiling.
 */
COSC_API cosc_int32 cosc_serial_get_msgtype(
    const struct cosc_serial *serial
);

/**
 * Reset serial.
 * @param serial The serial.
 */
COSC_API void cosc_serial_reset(
    struct cosc_serial *serial
);

#endif /* !COSC_NOWRITER && !COSC_NOREADER */

#ifndef COSC_NOWRITER

/**
 * Setup a serial for writing.
 * @param[out] serial The serial.
 * @param buffer A writable buffer, must not be NULL.
 * @param buffer_size The size of the buffer.
 * @param levels Provided levels, must point to an array
 * of levels with at least one member.
 * @param level_max The number of provided levels, must
 * be at least 1.
 * @param flags Serial flags, see COSC_SERIAL_* macros.
 * @see @ref COSC_SERIAL_PSIZE.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 */
COSC_API void cosc_writer_setup(
    struct cosc_serial *serial,
    void *buffer,
    cosc_int32 buffer_size,
    struct cosc_level *levels,
    cosc_int32 level_max,
    cosc_uint32 flags
);

/**
 * Start a new bundle level.
 * @param serial The serial.
 * @param timetag A timetag for the bundle.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELMAX the maximum number of levels is reached.
 * - @ref COSC_ELEVELTYPE if the current level does not accept a bundle.
 */
COSC_API cosc_int32 cosc_writer_start_bundle(
    struct cosc_serial *serial,
    cosc_uint64 timetag
);

/**
 * End a bundle level.
 * @param serial The serial.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_ELEVELTYPE if the current level is not a bundle.
 */
COSC_API cosc_int32 cosc_writer_end_bundle(
    struct cosc_serial *serial
);

/**
 * Start a new message level.
 * @param serial The serial.
 * @param address The address.
 * @param address_n Read at most this many bytes from @p address.
 * @param typetag The typetag.
 * @param typetag_n Read at most this many bytes from @p typetag.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELMAX the maximum number of levels is reached.
 * - @ref COSC_ELEVELTYPE if the current level does not accept a message.
 */
COSC_API cosc_int32 cosc_writer_start_message(
    struct cosc_serial *serial,
    const char *address,
    cosc_int32 address_n,
    const char *typetag,
    cosc_int32 typetag_n
);

/**
 * End a message level.
 * @param serial The serial.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_ELEVELTYPE if the current level is not a message.
 */
COSC_API cosc_int32 cosc_writer_end_message(
    struct cosc_serial *serial
);

/**
 * Start a new blob level.
 * @param serial The serial.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 * @note To write raw bytes to a started blob level use
 * cosc_writer_bytes().
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELMAX the maximum number of levels is reached.
 * - @ref COSC_ELEVELTYPE if the current level does not accept a blob.
 */
COSC_API cosc_int32 cosc_writer_start_blob(
    struct cosc_serial *serial
);

/**
 * End a blob level.
 * @param serial The serial.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer, this
 *   can happen if the blob needs to add pad bytes.
 * - @ref COSC_ELEVELTYPE if the current level is not a blob.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_end_blob(
    struct cosc_serial *serial
);

/**
 * Write an unsigned 32-bit integer.
 * @param serial The serial.
 * @param value The value.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_uint32(
    struct cosc_serial *serial,
    cosc_uint32 value
);

/**
 * Write a signed 32-bit integer.
 * @param serial The serial.
 * @param value The value.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_int32(
    struct cosc_serial *serial,
    cosc_int32 value
);

/**
 * Write a 32-bit float.
 * @param serial The serial.
 * @param value The value.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_float32(
    struct cosc_serial *serial,
    cosc_float32 value
);

/**
 * Write an unsigned 64-bit integer.
 * @param serial The serial.
 * @param value The value.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_uint64(
    struct cosc_serial *serial,
    cosc_uint64 value
);

/**
 * Write a signed 64-bit integer.
 * @param serial The serial.
 * @param value The value.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_int64(
    struct cosc_serial *serial,
    cosc_int64 value
);

/**
 * Write a 64-bit float.
 * @param serial The serial.
 * @param value The value.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_float64(
    struct cosc_serial *serial,
    cosc_float64 value
);

/**
 * Write a string.
 * @param serial The serial.
 * @param value The value.
 * @param value_n Read at most this many bytes from @p value.
 * @param[out] length If non-NULL and the function does not return
 * a negative error code the length, excluding the zero terminator,
 * of the string is stored here.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_string(
    struct cosc_serial *serial,
    const char *value,
    cosc_int32 value_n,
    cosc_int32 *length
);

/**
 * Write a blob.
 * @param serial The serial.
 * @param value The blob data.
 * @param value_n The byte size of the blob data.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @note To write raw bytes to a started blob level use
 * cosc_writer_bytes().
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_blob(
    struct cosc_serial *serial,
    const void *value,
    cosc_int32 value_n
);

/**
 * Write an ASCII character.
 * @param serial The serial.
 * @param value The value.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_char(
    struct cosc_serial *serial,
    cosc_int32 value
);

/**
 * Write a MIDI message.
 * @param serial The serial.
 * @param value The value or NULL for all zeroes.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @note Normally the fourth byte isn't used in MIDI.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_midi(
    struct cosc_serial *serial,
    const unsigned char value[4]
);

/**
 * Write a value.
 * @param serial The serial.
 * @param type The value typetag type.
 * @param value The value or NULL for zero/empty.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @note Normally the fourth byte isn't used in MIDI.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 * - @ref COSC_ETYPE if @p type is invalid.
 */
COSC_API cosc_int32 cosc_writer_value(
    struct cosc_serial *serial,
    cosc_int32 type,
    const union cosc_value *value
);

/**
 * Write an OSC message.
 * @param[out] buffer If non-NULL store the OSC data here, if NULL
 * then no bytes are stored.
 * @param size Store at most this many bytes to @p buffer.
 * @param message The message or NULL for an empty message.
 * @param[out] value_count If non-NULL the number of written values is
 * stored here.
 * @returns The number of written bytes if @p buffer is non-NULL,
 * the required size if @p buffer is NULL or a negative error code
 * if the operation fails.
 * @note The message address is NOT validated.
 * @note If message.values_n is less than the values specified
 * in message.typetag the rest of the values will be written as zero/empty.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * - @ref COSC_EOVERRUN if @p buffer is non-NULL and @p size is too small.
 * - @ref COSC_ESIZEMAX if @p size > @ref COSC_SIZE_MAX.
 * - @ref COSC_ETYPE if message typetag is invalid.
 * - @ref COSC_EPSIZE if @p size > 0 and is invalid or too small.
 */
COSC_API cosc_int32 cosc_writer_message(
    struct cosc_serial *serial,
    const struct cosc_message *message,
    cosc_int32 *value_count
);

/**
 * Write bytes to a started blob level.
 * @param serial The serial.
 * @param value The bytes or NULL to zero the bytes.
 * @param value_n The number of bytes to write.
 * @returns The number of written bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 */
COSC_API cosc_int32 cosc_writer_bytes(
    struct cosc_serial *serial,
    const void *value,
    cosc_int32 value_n
);

/**
 * Skip a value, if the value has payload it will be zeroes.
 * @param serial The serial.
 * @returns The number of written bytes or a negative error
 * code on failure or 0 if there are no more types in the
 * message typetag.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_writer_skip(
    struct cosc_serial *serial
);

/**
 * Repeat typetag from the beginning of an array start, where the array
 * starts with a '['.
 * @param serial The serial.
 * @returns 0 on success or a negative error code on failure.
 * @note If cosc was built with COSC_NOARRAY defined this function does
 * nothing and returns 0 or @ref COSC_EINVAL.
 * @remark This function is not available if COSC_NOWRITER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a reader.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE the current message typetag type is not ']' or
 *   if the typetag has no valid array start.
 */
COSC_API cosc_int32 cosc_writer_repeat(
    struct cosc_serial *serial
);

#endif /* !COSC_NOWRITER */

#ifndef COSC_NOREADER

/**
 * Setup a serial for reading.
 * @param[out] serial The serial.
 * @param buffer A writable buffer, must not be NULL.
 * @param buffer_size The size of the buffer.
 * @param levels Provided levels, must point to an array
 * of levels with at least one member.
 * @param level_max The number of provided levels, must
 * be at least 1.
 * @param flags Serial flags, see COSC_SERIAL_* macros.
 * @see @ref COSC_SERIAL_PSIZE.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 */
COSC_API void cosc_reader_setup(
    struct cosc_serial *serial,
    const void *buffer,
    cosc_int32 buffer_size,
    struct cosc_level *levels,
    cosc_int32 level_max,
    cosc_uint32 flags
);

/**
 * Check if the buffer has a bundle at the current read size.
 * @param serial The serial.
 * @param[out] timetag If non-NULL and the function does not return a
 * negative error code the timestamp is stored here.
 * @param psize If non-NULL and the function does not return a negative
 * error code stored the packet size here, will be 0 if there is no
 * packet size.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept a bundle.
 * - @ref COSC_ETYPE if it's not a bundle.
 */
COSC_API cosc_int32 cosc_reader_peek_bundle(
    struct cosc_serial *serial,
    cosc_uint64 *timetag,
    cosc_int32 *psize
);

/**
 * Start a new bundle level.
 * @param serial The serial.
 * @param[out] timetag If non-NULL and the function does not return a
 * negative error code the timestamp is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELMAX the maximum number of levels is reached.
 * - @ref COSC_ELEVELTYPE if the current level does not accept a bundle.
 * - @ref COSC_ETYPE if it's not a bundle.
 */
COSC_API cosc_int32 cosc_reader_start_bundle(
    struct cosc_serial *serial,
    cosc_uint64 *timetag
);

/**
 * End a bundle level.
 * @param serial The serial.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_ELEVELTYPE if the current level is not a bundle.
 */
COSC_API cosc_int32 cosc_reader_end_bundle(
    struct cosc_serial *serial
);

/**
 * Start a new message level.
 * @param serial The serial.
 * @param[out] address If non-NULL and the function does not return a
 * negative error code a pointer to the address is stored here.
 * @param[out] address_n If non-NULL and the function does not return a
 * negative error code the length, excluding the zero terminator,
 * of the address is stored here.
 * @param[out] typetag If non-NULL and the function does not return a
 * negative error code a pointer to the typetag is stored here.
 * @param[out] typetag_n If non-NULL and the function does not return a
 * negative error code the length, excluding the zero terminator,
 * of the typetag is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELMAX the maximum number of levels is reached.
 * - @ref COSC_ELEVELTYPE if the current level does not accept a message.
 */
COSC_API cosc_int32 cosc_reader_start_message(
    struct cosc_serial *serial,
    const char **address,
    cosc_int32 *address_n,
    const char **typetag,
    cosc_int32 *typetag_n
);

/**
 * End a message level.
 * @param serial The serial.
 * @param exit_early If non-zero and the @p types have an array specified
 * the function will stop reading more array members and exit early even
 * if there are more bytes left in @p buffer.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_ELEVELTYPE if the current level is not a message.
 */
COSC_API cosc_int32 cosc_reader_end_message(
    struct cosc_serial *serial,
    cosc_int32 exit_early
);

/**
 * Start a new blob level.
 * @param serial The serial.
 * @param[out] value_size If non-NULL and the function does not return
 * a negative error code the BLOB data size is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @note To read raw bytes to a started blob level use
 * cosc_reader_bytes().
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELMAX the maximum number of levels is reached.
 * - @ref COSC_ELEVELTYPE if the current level does not accept a blob.
 */
COSC_API cosc_int32 cosc_reader_start_blob(
    struct cosc_serial *serial,
    cosc_int32 *value_size
);

/**
 * End a blob level.
 * @param serial The serial.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer, this
 *   can happen if the blob needs to add pad bytes.
 * - @ref COSC_ELEVELTYPE if the current level is not a blob.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_end_blob(
    struct cosc_serial *serial
);

/**
 * Write an unsigned 32-bit integer.
 * @param serial The serial.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_uint32(
    struct cosc_serial *serial,
    cosc_uint32 *value
);

/**
 * Write a signed 32-bit integer.
 * @param serial The serial.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_int32(
    struct cosc_serial *serial,
    cosc_int32 *value
);

/**
 * Write a 32-bit float.
 * @param serial The serial.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_float32(
    struct cosc_serial *serial,
    cosc_float32 *value
);

/**
 * Write an unsigned 64-bit integer.
 * @param serial The serial.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_uint64(
    struct cosc_serial *serial,
    cosc_uint64 *value
);

/**
 * Write a signed 64-bit integer.
 * @param serial The serial.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_int64(
    struct cosc_serial *serial,
    cosc_int64 *value
);

/**
 * Write a 64-bit float.
 * @param serial The serial.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_float64(
    struct cosc_serial *serial,
    cosc_float64 *value
);

/**
 * Write a string.
 * @param serial The serial.
 * @param[out] value If non-NULL store the string here.
 * @param value_n Store at most this many bytes to @p value.
 * @param[out] length If non-NULL and the function does not return a
 * negative error code the length of the string, excluding the zero
 * terminator, is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_string(
    struct cosc_serial *serial,
    char *value,
    cosc_int32 value_n,
    cosc_int32 *length
);

/**
 * Write a blob.
 * @param serial The serial.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code store the blob data here.
 * @param value_n Store at most this many bytes to @p value.
 * @param[out] data If non-NULL and the function does not return a negative
 * error code a pointer to the first byte of the blob data is stored here,
 * or NULL if the blob has zero bytes.
 * @param[out] data_size If non-NULL and the function does not return a
 * negative error code the size of the blob data is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @note To read raw bytes to a started blob level use
 * cosc_reader_bytes().
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_blob(
    struct cosc_serial *serial,
    void *value,
    cosc_int32 value_n,
    const void **data,
    cosc_int32 *data_size
);

/**
 * Write an ASCII character.
 * @param serial The serial.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_char(
    struct cosc_serial *serial,
    cosc_int32 *value
);

/**
 * Write a MIDI message.
 * @param serial The serial.
 * @param[out] value If non-NULL and the function does not return a
 * negative error code the value is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @note Normally the fourth byte isn't used in MIDI.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_midi(
    struct cosc_serial *serial,
    unsigned char value[4]
);

/**
 * Read a value.
 * @param serial The serial.
 * @param type The value typetag type.
 * @param[out] value If non-NULL and the function does not return
 * a negative error code the value is stored here.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @note Normally the fourth byte isn't used in MIDI.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 * - @ref COSC_ETYPE if @p type is invalid.
 */
COSC_API cosc_int32 cosc_reader_value(
    struct cosc_serial *serial,
    cosc_int32 type,
    union cosc_value *value
);

/**
 * Read an OSC message.
 * @param serial The serial.
 * @param message The message or NULL to discard the message.
 * @param[out] value_count If non-NULL the number of read values is
 * stored here.
 * @param exit_early If non-zero and the @p types have an array specified
 * the function will stop reading more array members and exit early even
 * if there are more bytes left in @p buffer.
 * @returns The number of read bytes or a negative error code if the
 * operation fails.
 * @note The message address is NOT validated.
 * @note If message.values_n is less than the values specified
 * in message.typetag the rest of the values will be read and discarded.
 * @note The value of @p exit_early has no effect if COSC_NOARRAY was
 * defined when building.
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_ETYPE if the message typetag is invalid.
 */
COSC_API cosc_int32 cosc_reader_message(
    struct cosc_serial *serial,
    struct cosc_message *message,
    cosc_int32 *value_count,
    cosc_int32 exit_early
);

/**
 * Read bytes to a started blob level.
 * @param serial The serial.
 * @param[out] value If non-NULL store read bytes here
 * or NULL to discard the bytes.
 * @param value_n The number of bytes to read.
 * @returns The number of read bytes or a negative error
 * code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 */
COSC_API cosc_int32 cosc_reader_bytes(
    struct cosc_serial *serial,
    void *value,
    cosc_int32 value_n
);

/**
 * Skip a value, if the value has payload it will be zeroes.
 * @param serial The serial.
 * @returns The number of read bytes or a negative error
 * code on failure or 0 if there are no more types in the
 * message typetag.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE if trying to add the value to a message where
 *   the value type does not match the typetag.
 */
COSC_API cosc_int32 cosc_reader_skip(
    struct cosc_serial *serial
);

/**
 * Repeat typetag from the beginning of an array start, not including the
 * starting '[' character.
 * @param serial The serial.
 * @returns 0 on success or a negative error code on failure.
 * @remark This function is not available if COSC_NOREADER
 * was defined when compiling.
 *
 * Error codes:
 *
 * - @ref COSC_EINVAL if the serial was setup as a writer.
 * - @ref COSC_EOVERRUN if the operation will overrun the buffer.
 * - @ref COSC_ELEVELTYPE if the current level does not accept the value.
 * - @ref COSC_EMSGTYPE the current message typetag type is not ']'.
 */
COSC_API cosc_int32 cosc_reader_repeat(
    struct cosc_serial *serial
);

#endif /* !COSC_NOREADER */

#ifdef __cplusplus
}
#endif

#endif
