/**
 * @file doxygen.h
 * @brief Doxygen documentation for macros defined at compile time.
 * @note This file is not needed for building or using the cosc.
 */

/**
 * Disable use of standard libraries (except `stdint.h`).
 */
#define COSC_NOSTDLIB

/**
 * Disable inclusion of `stdint.h` (or `cstdint` if C++).
 */
#define COSC_NOSTDINT

/**
 * Disables 64-bit integer support and replaces the type with a two word struct
 * (@ref cosc_64bits).
 */
#define COSC_NOINT64

/**
 * Disables 32-bit float support and replaces the type with @ref cosc_uint32.
 */
#define COSC_NOFLOAT32

/**
 * Disables 64-bit float support and replaces the type with @ref cosc_64bits.
 */
#define COSC_NOFLOAT64

/**
 * Disable pattern validation and matching functions.
 */
#define COSC_NOPATTERN

/**
 * Disable endian swapping.
 */
#define COSC_NOSWAP

/**
 * Disable support for arrays.
 */
#define COSC_NOARRAY

/**
 * Disable dumping functions.
 */
#define COSC_NODUMP

/**
 * Disable timetaging functions.
 */
#define COSC_NOTIMETAG

/**
 * Disable float conversion functions.
 */
#define COSC_NOFLTCONV

/**
 * Disable serial writer functions.
 */
#define COSC_NOWRITER

/**
 * Disable serial reader functions.
 */
#define COSC_NOREADER
