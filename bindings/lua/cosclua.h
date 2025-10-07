/**
 * @file cosclua.h
 * @brief Lua bindings for cosc.
 * @copyright Copyright 2025 Peter Gebauer (MIT license)
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
#ifndef COSCLUA_H
#define COSCLUA_H

#include <lua.h>

/**
 * Used to declare linkage for functions.
 * @def COSC_API
 */

#ifndef COSCAPI_LUA
#ifdef COSC_BUILD_SHARED
#ifdef _MSC_VER
#define COSCAPI_LUA __declspec(dllexport)
#else
#define COSCAPI_LUA
#endif
#else /* COSC_BUILD_SHARED */
#if defined(__clang__) || defined(__GNUC__)
#define COSCAPI_LUA __attribute__ ((visibility ("hidden")))
#else
#define COSCAPI_LUA
#endif
#endif /* COSC_BUILD_SHARED */
#endif /* COSCAPI_LUA */

/**
 * The name of the registered metatable for the buffer object.
 */
#define COSCLUA_BUFFER_METANAME "cosclua_buffer"

/**
 * Max size of a buffer, 2147483644 bytes.
 * @note This is the largest multiple for that fits a signed 32-bit integer.
 */
#define COSCLUA_BUFFER_MAX 2147483644

/**
 * Buffer allocation page size.
 */
#define COSCLUA_BUFFER_PAGE_SIZE 128

/**
 * The buffer object userdata struct.
 */
struct cosclua_buffer
{

    /**
     * Bytes.
     */
    void *bytes;

    /**
     * Max allocation.
     */
    cosc_int32 alloc_max;

    /**
     * Size, i.e number of used bytes.
     */
    cosc_int32 size;

    /**
     * The number of allocated bytes.
     */
    cosc_int32 alloc;

    /**
     * Buffer page size.
     */
    cosc_int32 page_size;

    /**
     * Offset.
     */
    cosc_int32 offset;

};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Opens the cosclua library and pushes a table to the stack.
 * @param L The Lua state.
 * @returns The number of objects pushed to the stack (1).
 */
COSCAPI_LUA int luaopen_cosclua(
    lua_State *L
);

/**
 * Round up to the nearest multiple of 4.
 * @param sz The value to align.
 * @returns The aligned value.
 * @note The returned value will be clamped betwen
 * 0 and @ref COSCLUA_BUFFER_MAX.
 */
COSCAPI_LUA lua_Integer cosclua_align(
    lua_Integer sz
);

/**
 * Create a new buffer and push it onto the stack.
 * @param L The Lua state.
 * @param alloc_max The maximum number of bytes this buffer can allocate
 * or zero or a negative value will use the default
 * defined in @ref COSCLUA_BUFFER_MAX.
 * @param page_size The buffer will grow/shrink by a multiple of this
 * value, will be implicitly 4 byte aligned.
 * @param prealloc Preallocate this many bytes, will be clamped
 * to @p alloc_max.
 * @returns A pointer to the new cosclua_buffer userdata value.
 * @note `[-0,+1,m]`
 */
COSCAPI_LUA struct cosclua_buffer *cosclua_newbuffer(
    lua_State *L,
    lua_Integer alloc_max,
    lua_Integer page_size,
    lua_Integer prealloc
);

/**
 * Return a pointer to the cosclua_buffer value at the
 * specified index.
 * @param L The Lua state.
 * @param index The stack index.
 * @returns A pointer to the cosclua_buffer userdata value at the
 * specified @p index or NULL if the value is not a cosclua_buffer.
 * @note `[-0,+0,-]`
 */
COSCAPI_LUA struct cosclua_buffer *cosclua_tobuffer(
    lua_State *L,
    int index
);

/**
 * Checks whether the function argument arg is a cosclua_buffer value
 * and returns it.
 * @param L The Lua state.
 * @param arg The argument with the first argument being 1.
 * @returns A pointer to the cosclua_buffer userdata value at the
 * specified.
 * @note `[-0,+0,v]`
 * @note If the value at @p index is not a cosclua_buffer value the
 * function will raise an error.
 */
COSCAPI_LUA struct cosclua_buffer *cosclua_checkbuffer(
    lua_State *L,
    int arg
);

/**
 * Get the max allocation for the buffer.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @returns The max allocation in bytes.
 * @note `[-0,+0,-]`
 */
COSCAPI_LUA lua_Integer cosclua_buffergetallocmax(
    lua_State *L,
    const struct cosclua_buffer *buffer
);

/**
 * Get the current allocation size.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @returns The current allocation in bytes.
 * @note `[-0,+0,-]`
 */
COSCAPI_LUA lua_Integer cosclua_buffergetalloc(
    lua_State *L,
    const struct cosclua_buffer *buffer
);

/**
 * Get the page size of the buffer.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @returns The page size.
 * @note `[-0,+0,-]`
 */
COSCAPI_LUA lua_Integer cosclua_buffergetpagesize(
    lua_State *L,
    const struct cosclua_buffer *buffer
);

/**
 * Get the byte size of the buffer, i.e the number of used bytes.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @returns The size, i.e number of bytes used.
 * @note `[-0,+0,-]`
 */
COSCAPI_LUA lua_Integer cosclua_buffergetsize(
    lua_State *L,
    const struct cosclua_buffer *buffer
);

/**
 * Set the byte size of the buffer, i.e the number of used bytes.
 * allocate more bytes of needed.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @param size The new size.
 * @returns The new size on success or a negative error code on failure.
 * @note `[-0,+0,m]`
 * @note If the current offset exceeds the new size it will be set to the
 * end of the buffer.
 *
 * Error codes:
 *
 * - @ref COSC_ESIZEMAX if @p size exceeds cosclua_getallocmax().
 */
COSCAPI_LUA lua_Integer cosclua_buffersetsize(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer size
);

/**
 * Add size to the buffer, i.e the number of used bytes, and allocate more
 * bytes of needed.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @param add_size The number of bytes to add to size.
 * @returns The new size on success or a negative error code on failure.
 * @note `[-0,+0,m]`
 *
 * Error codes:
 *
 * - @ref COSC_ESIZEMAX if the new buffer size exceeds cosclua_getallocmax().
 */
COSCAPI_LUA lua_Integer cosclua_bufferaddsize(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer add_size
);

/**
 * Get the current byte offset in the buffer.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @returns The byte offset.
 * @note `[-0,+0,-]`
 */
COSCAPI_LUA lua_Integer cosclua_buffertell(
    lua_State *L,
    const struct cosclua_buffer *buffer
);

/**
 * Set the current byte offset in the buffer.
 * allocate more bytes of needed.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @param offset The new offset, if negative the offset will wrap
 * from the end of the buffer size.
 * @returns The new offset on success or a negative error code on failure.
 * @note `[-0,+0,-]`
 *
 * Error codes:
 *
 * - -1 @p offset is out of bounds.
 */
COSCAPI_LUA lua_Integer cosclua_bufferseek(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer offset
);

/**
 * Set the current byte offset to the start of the buffer.
 * allocate more bytes of needed.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @returns The new offset (always 0).
 * @note `[-0,+0,-]`
 */
COSCAPI_LUA lua_Integer cosclua_bufferseekstart(
    lua_State *L,
    struct cosclua_buffer *buffer
);

/**
 * Set the current byte offset to the end of the buffer.
 * allocate more bytes of needed.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @returns The new offset (same as buffer size).
 * @note `[-0,+0,-]`
 */
COSCAPI_LUA lua_Integer cosclua_bufferseekend(
    lua_State *L,
    struct cosclua_buffer *buffer
);

/**
 * Append a range of bytes from another buffer.
 * @param L The Lua state.
 * @param buffer The buffer that will be appended to.
 * @param src The buffer to append from.
 * @param start The first byte in @p src to start appending from.
 * @param length The number of bytes to append or
 * a negative value to append all remaining bytes after @p start.
 * @returns The number of appended on success or a negative error
 * code on failure.
 * @note `[-0,+0,m]`
 * @note @p length will be truncated if it exceeds @p src size.
 * @note Since it's unknown if this is considered a read or write operation
 * the internal offset is unmodified by this call. If this is considered
 * a proper write operation consider calling cosclua_bufferseekend()
 * after a successful call to this function.
 *
 * Error codes:
 *
 * - @ref COSC_ESIZEMAX if the new buffer size exceeds cosclua_getallocmax().
 */
COSCAPI_LUA lua_Integer cosclua_bufferappend(
    lua_State *L,
    struct cosclua_buffer *buffer,
    const struct cosclua_buffer *src,
    lua_Integer start,
    lua_Integer length
);

/**
 * Insert a range of bytes from another buffer.
 * @param L The Lua state.
 * @param buffer The buffer that will be insert into.
 * @param offset The offset in @p buffer to insert the other range.
 * @param src The buffer to insert from.
 * @param start The first byte in @p src to start inserting from.
 * @param length The number of bytes to insert or a negative
 * value to insert all remaining bytes after @p start.
 * @returns The number of inserted on success or a negative error
 * code on failure.
 * @note `[-0,+0,m]`
 * @note @p length will be truncated if it exceeds @p src size.
 * @note Since it's unknown if this is considered a read or write operation
 * the internal offset is unmodified by this call. If this is considered
 * a proper write operation consider calling cosclua_bufferseekend()
 * after a successful call to this function.
 *
 * Error codes:
 *
 * - @ref COSC_ESIZEMAX if the new buffer size exceeds cosclua_getallocmax().
 * - -1 if @p offset is out of bounds.
 */
COSCAPI_LUA lua_Integer cosclua_bufferinsert(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer offset,
    const struct cosclua_buffer *src,
    lua_Integer start,
    lua_Integer length
);

/**
 * Copy bytes from one buffer to another.
 * @param L The Lua state.
 * @param buffer The buffer to copy bytes to.
 * @param offset The offset of where to set the first byte.
 * @param src The buffer to copy from.
 * @param start The first byte to copy.
 * @param length The number of bytes to copy or a negative
 * value to copy all remaining bytes after @p start.
 * @returns The buffer size on success or a negative error code on failure.
 * @note `[-0,0,m]`
 * @note @p length will be truncated if it exceeds eifhter the
 * @p buffer or @p src size.
 */
COSCAPI_LUA lua_Integer cosclua_buffercopy(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer offset,
    const struct cosclua_buffer *src,
    lua_Integer start,
    lua_Integer length
);

/**
 * Remove bytes from a buffer.
 * @param L The Lua state.
 * @param buffer The buffer to remove bytes from.
 * @param offset The offset of the first byte to remove.
 * @param length The number of bytes to remove or a negative
 * value to remove all remaining bytes after @p offset.
 * @returns The number of removed on success or a negative error code
 * on failure.
 * @note `[-0,+0,m]`
 * @note @p length will be truncated if it exceeds @p buffer size.
 * @note If the current byte offset is after @p offset it will be
 * moved back by the number of removed bytes.
 *
 * Error codes:
 *
 * - -1 if @p offset is out of bounds.
 */
COSCAPI_LUA lua_Integer cosclua_bufferremove(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer offset,
    lua_Integer length
);

/**
 * Write bytes directly to the buffer.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @param length The number of bytes to write.
 * @param bytes Write these bytes or NULL to zero the bytes.
 * @returns The number of written bytes on success or a negative error
 * code on failure.
 * @note `[-0,0,m]`
 * @note On success the internal offset will be moved forward
 * by the return value of this function.
 *
 * Error codes:
 *
 * - @ref COSC_ESIZEMAX if the new buffer size exceeds cosclua_getallocmax().
 */
COSCAPI_LUA lua_Integer cosclua_bufferwritebytes(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer length,
    const void *bytes
);

/**
 * Read bytes directly from the buffer.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @param length The number of bytes to read.
 * @param[out] bytes If non-NULL store the read bytes here.
 * @returns The number of read bytes on success or a negative error
 * code on failure.
 * @note `[-0,0,-]`
 * @note On success the internal offset will be moved forward
 * by the return value of this function.
 *
 * Error codes:
 *
 * - @ref COSC_EOVERRUN if the size exceeds cosclua_getsize().
 */
COSCAPI_LUA lua_Integer cosclua_bufferreadbytes(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer length,
    void *bytes
);

/**
 * Write an unsigned 32-bit integer to the buffer.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @param value The value.
 * @returns The number of written bytes on success or a negative error
 * code on failure.
 * @note `[-0,0,m]`
 * @note On success the internal offset will be moved forward
 * by the return value of this function.
 *
 * Error codes:
 *
 * - @ref COSC_ESIZEMAX if the new buffer size exceeds cosclua_getallocmax().
 */
COSCAPI_LUA lua_Integer cosclua_bufferwriteuint32(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer value
);

/**
 * Read an unsigned 32-bit integer from the buffer.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @param[out] value If non-NULL store the value here.
 * @returns The number of read bytes on success or a negative error
 * code on failure.
 * @note `[-0,0,-]`
 * @note On success the internal offset will be moved forward
 * by the return value of this function.
 * @note If lua_Integer is a signed 32 bit integer the value
 * stored to @p value is bit copied using type punning in C99
 * or reinterpret_cast in C++11.
 *
 * Error codes:
 *
 * - @ref COSC_EOVERRUN if the size exceeds cosclua_getsize().
 */
COSCAPI_LUA lua_Integer cosclua_bufferreaduint32(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer *value
);

/**
 * Write a signed 32-bit integer to the buffer.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @param value The value.
 * @returns The number of written bytes on success or a negative error
 * code on failure.
 * @note `[-0,0,m]`
 * @note On success the internal offset will be moved forward
 * by the return value of this function.
 *
 * Error codes:
 *
 * - @ref COSC_ESIZEMAX if the new buffer size exceeds cosclua_getallocmax().
 */
COSCAPI_LUA lua_Integer cosclua_bufferwriteint32(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer value
);

/**
 * Read a signed 32-bit integer from the buffer.
 * @param L The Lua state.
 * @param buffer The buffer.
 * @param[out] value If non-NULL store the value here.
 * @returns The number of read bytes on success or a negative error
 * code on failure.
 * @note `[-0,0,-]`
 * @note On success the internal offset will be moved forward
 * by the return value of this function.
 *
 * Error codes:
 *
 * - @ref COSC_EOVERRUN if the size exceeds cosclua_getsize().
 */
COSCAPI_LUA lua_Integer cosclua_bufferreadint32(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer *value
);

#ifdef __cplusplus
}
#endif

#endif
