/**
 * cosclua51
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
 */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "cosc.h"

// FIXME: slice
// FIXME: appendstring, insertstring and copystring
// FIXME: tostring, tostringslice
// FIXME: the rest of the Lua API.

#ifdef COSC_NOSTDLIB
static void *cosclua_memcpy(void *dest, const void *src, lua_Integer n)
{
    for (lua_Integer i = 0; i < n; i++)
        ((unsigned char *)dest)[i] = ((const unsigned char *)src)[i];
    return dest;
}
static void *cosclua_memset(void *s, int c, lua_Integer n)
{
    for (lua_Integer i = 0; i < n; i++)
        ((unsigned char *)s)[i] = c;
    return s;
}
#else
#include <string.h>
#include <stdlib.h>
#define cosclua_memcpy memcpy
#define cosclua_memset memset
#endif

#include "cosclua.h"

#define COSCLUA_SWAP64(value_) do { cosc_uint32 tmp = value->hi; value->hi = value->lo; value->lo = tmp; } while (0)

#define COSCLUA_CHECKBUFFEROFFSET(buffer, offset) do { if ((offset) < 0 || (offset) >= (buffer)->size) return -1; } while (0)

static lua_Integer cosclua_clamp(lua_Integer i, lua_Integer min_, lua_Integer max_)
{
    if (i > max_)
        return max_;
    if (i < min_)
        return min_;
    return i;
}

static lua_Integer cosclua_sizebypage(lua_Integer size, lua_Integer page_size)
{
    if (size <= 0 || page_size <= 0)
        return 0;
    page_size = cosclua_clamp(page_size, 0, COSCLUA_BUFFER_MAX);
    page_size += COSC_PAD(page_size);
    int add = (size % page_size) != 0;
    size /= page_size;
    size += add;
    if (size > COSCLUA_BUFFER_MAX / page_size)
        return COSCLUA_BUFFER_MAX;
    size *= page_size;
    return size;
}

static void cosclua_clamprange(lua_Integer *start, lua_Integer *length, lua_Integer size)
{
    if (*start < 0)
        *start  = 0;
    if (*start >= size)
        *length = 0;
    else if  (*length < 0 || *length > size - *start)
        *length = size - *start;
}

static const char *cosclua_strerror(int error_code)
{
    if (error_code >= 0)
        return "";
    switch (error_code)
    {
    case COSC_EOVERRUN: return "buffer overrun";
    case COSC_ESIZEMAX: return "COSC_SIZE_MAX exceeded";
    case COSC_ETYPE: return "invalid typetag";
    case COSC_EPSIZE: return "invalid packet size";
    }
    return "generic error";
}

static void *cosclua_alloc(lua_State *L, void *mem, size_t osize, size_t nsize)
{
    void *ud = NULL;
    lua_Alloc alloc = lua_getallocf(L, &ud);
    return alloc(ud, mem, osize, nsize);
}

static void cosclua_dealloc(lua_State *L, void *mem, size_t size)
{
    void *ud = NULL;
    lua_Alloc alloc = lua_getallocf(L, &ud);
    alloc(ud, mem, size, 0);
}

//
// Lua API
//

static int buffer_gc(lua_State *L)
{
    struct cosclua_buffer *buffer = luaL_checkudata(L, 1, "cosclua_buffer");
    if (buffer != NULL)
        free(buffer->bytes);
    return 0;
}

static int buffer_get_alloc_max(lua_State *L)
{
    struct cosclua_buffer *buffer = luaL_checkudata(L, 1, "cosclua_buffer");
    lua_pushinteger(L, cosclua_buffergetallocmax(L, buffer));
    return 1;
}

static int buffer_get_size(lua_State *L)
{
    struct cosclua_buffer *buffer = luaL_checkudata(L, 1, "cosclua_buffer");
    lua_pushinteger(L, cosclua_buffergetsize(L, buffer));
    return 1;
}

static int buffer_set_size(lua_State *L)
{
    struct cosclua_buffer *buffer = luaL_checkudata(L, 1, "cosclua_buffer");
    lua_Integer size = luaL_checkinteger(L, 2);
    lua_Integer ret = cosclua_buffersetsize(L, buffer, size);
    if (ret < 0)
        return luaL_error(L, "%s", cosclua_strerror(ret));
    lua_pushinteger(L, ret);
    return 1;
}

static int buffer_add_size(lua_State *L)
{
    struct cosclua_buffer *buffer = luaL_checkudata(L, 1, "cosclua_buffer");
    lua_Integer size = luaL_checkinteger(L, 2);
    lua_Integer ret = cosclua_bufferaddsize(L, buffer, size);
    if (ret < 0)
        return luaL_error(L, "%s", cosclua_strerror(ret));
    lua_pushinteger(L, ret);
    return 1;
}

static int buffer_tell(lua_State *L)
{
    struct cosclua_buffer *buffer = luaL_checkudata(L, 1, "cosclua_buffer");
    lua_pushinteger(L, cosclua_buffertell(L, buffer));
    return 1;
}

static int buffer_seek(lua_State *L)
{
    struct cosclua_buffer *buffer = luaL_checkudata(L, 1, "cosclua_buffer");
    lua_Integer offset = luaL_checkinteger(L, 2);
    lua_Integer ret = cosclua_bufferseek(L, buffer, offset);
    if (ret < 0)
        return luaL_error(L, "%s", cosclua_strerror(ret));
    lua_pushinteger(L, ret);
    return 1;
}

static int buffer_seekstart(lua_State *L)
{
    struct cosclua_buffer *buffer = luaL_checkudata(L, 1, "cosclua_buffer");
    lua_pushinteger(L, cosclua_bufferseekstart(L, buffer));
    return 1;
}

static int buffer_seekend(lua_State *L)
{
    struct cosclua_buffer *buffer = luaL_checkudata(L, 1, "cosclua_buffer");
    lua_pushinteger(L, cosclua_bufferseekend(L, buffer));
    return 1;
}

static int buffer_new(lua_State *L)
{
    return 0;
}

static const struct luaL_Reg BUFFER_FUNCS[] = {
    {"__gc", buffer_gc},
    {"__len", buffer_get_size},
    {"get_alloc_max", buffer_get_alloc_max},
    {"set_size", buffer_set_size},
    {"add_size", buffer_add_size},
    {"tell", buffer_tell},
    {"seek", buffer_seek},
    {"seekstart", buffer_seekstart},
    {"seekend", buffer_seekend},
    {NULL, NULL}
};

static const struct luaL_Reg PUBLIC_FUNCS[] = {
    // {"address_char_validate", cosclua_address_char_validate},
    // {"address_validate", cosclua_address_validate},
    // {"typetag_char_validate", cosclua_typetag_char_validate},
    // {"typetag_validate", cosclua_typetag_validate},
    // {"typetag_payload", cosclua_typetag_payload},
    // {"pattern_char_validate", cosclua_pattern_char_validate},
    // {"pattern_validate", cosclua_pattern_validate},
    // {"pattern_match", cosclua_pattern_match},
    // {"signature_match", cosclua_signature_match},
    // {"timetag_to_time", cosclua_timetag_to_time},
    // {"timetag_from_time", cosclua_timetag_from_time},
    {"buffer", buffer_new},
    {NULL, NULL}
};

//
// C API
//

int luaopen_cosclua(lua_State *L)
{
    luaL_newlib(L, PUBLIC_FUNCS);
    return 1;
}

lua_Integer cosclua_align(
    lua_Integer sz
)
{
    if (sz < 0)
        return 0;
    if (sz >= COSCLUA_BUFFER_MAX)
        return COSCLUA_BUFFER_MAX;
    return sz + COSC_PAD(sz);
}

struct cosclua_buffer *cosclua_newbuffer(
    lua_State *L,
    lua_Integer alloc_max,
    lua_Integer page_size,
    lua_Integer prealloc
)
{
    struct cosclua_buffer *buffer = lua_newuserdatauv(L, sizeof(struct cosclua_buffer), 0);
    if (alloc_max <= 0)
        buffer->alloc_max = COSCLUA_BUFFER_MAX;
    else
        buffer->alloc_max = cosclua_clamp(alloc_max, 0, COSCLUA_BUFFER_MAX);
    if (page_size <= 0)
        buffer->page_size = COSCLUA_BUFFER_PAGE_SIZE;
    else
        buffer->page_size = cosclua_align(cosclua_clamp(page_size, 0, COSCLUA_BUFFER_MAX));
    if (prealloc > 0)
    {
        prealloc = cosclua_sizebypage(cosclua_clamp(page_size, 0, COSCLUA_BUFFER_MAX), buffer->page_size);
        if (prealloc > buffer->alloc_max)
            prealloc = buffer->alloc_max;
        buffer->bytes = cosclua_alloc(L, NULL, 0, prealloc);
        buffer->alloc = prealloc;
        cosclua_memset(buffer->bytes, 0, buffer->alloc);
    }
    else
    {
        buffer->bytes = 0;
        buffer->alloc = 0;
    }
    buffer->size = 0;
    buffer->offset = 0;
    if (luaL_newmetatable(L, COSCLUA_BUFFER_METANAME))
    {
        luaL_setfuncs(L, BUFFER_FUNCS, 0);
    }
    lua_setmetatable(L, -2);
    return buffer;
}

struct cosclua_buffer *cosclua_tobuffer(
    lua_State *L,
    int index
)
{
    struct cosclua_buffer *buffer = NULL;
    if (lua_isuserdata(L, index))
    {
        int type = luaL_getmetafield(L, index, "__name");
        if (type == LUA_TSTRING && strcmp(lua_tostring(L, -1), COSCLUA_BUFFER_METANAME) == 0)
            buffer = lua_touserdata(L, -1);
        if (type != LUA_TNIL)
            lua_pop(L, 1);
    }
    return buffer;
}

struct cosclua_buffer *cosclua_checkbuffer(
    lua_State *L,
    int arg
)
{
    return luaL_checkudata(L, arg, COSCLUA_BUFFER_METANAME);
}

lua_Integer cosclua_buffergetallocmax(
    lua_State *L,
    const struct cosclua_buffer *buffer
)
{
    return buffer->alloc_max;
}

lua_Integer cosclua_buffergetalloc(
    lua_State *L,
    const struct cosclua_buffer *buffer
)
{
    return buffer->alloc;
}

lua_Integer cosclua_buffergetpagesize(
    lua_State *L,
    const struct cosclua_buffer *buffer
)
{
    return buffer->page_size;
}

lua_Integer cosclua_buffergetsize(
    lua_State *L,
    const struct cosclua_buffer *buffer
)
{
    return buffer->size;
}

lua_Integer cosclua_buffersetsize(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer size
)
{
    if (size <= 0)
    {
        cosclua_dealloc(L, buffer->bytes, buffer->alloc);
        buffer->bytes = NULL;
        buffer->size = 0;
        buffer->alloc = 0;
        buffer->offset = 0;
        return 0;
    }
    if (size > COSCLUA_BUFFER_MAX || size > buffer->alloc_max)
        return COSC_ESIZEMAX;
    lua_Integer alloc = cosclua_sizebypage(size, buffer->page_size);
    if (alloc > buffer->alloc_max)
        alloc = buffer->alloc_max;
    buffer->bytes = cosclua_alloc(L, buffer->bytes, buffer->alloc, alloc);
    buffer->alloc = alloc;
    buffer->size = size;
    if (buffer->offset > buffer->size)
        buffer->offset = buffer->size;
    return buffer->size;
}

lua_Integer cosclua_bufferaddsize(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer add_size
)
{
    if (add_size < 0)
        return 0;
    if (add_size > COSCLUA_BUFFER_MAX - buffer->size)
        return COSC_ESIZEMAX;
    return cosclua_buffersetsize(L, buffer, buffer->size + add_size);
}

lua_Integer cosclua_buffertell(
    lua_State *L,
    const struct cosclua_buffer *buffer
)
{
    return buffer->offset;
}

lua_Integer cosclua_bufferseek(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer offset
)
{
    if (buffer->size <= 0)
        buffer->offset = 0;
    else if (offset < 0)
    {
        offset %= buffer->size;
        offset += buffer->size;
    }
    if (offset < 0 || offset > buffer->size)
        return COSC_EOVERRUN;
    return buffer->offset = offset;
}

lua_Integer cosclua_bufferseekstart(
    lua_State *L,
    struct cosclua_buffer *buffer
)
{
    return cosclua_bufferseek(L, buffer, 0);
}

lua_Integer cosclua_bufferseekend(
    lua_State *L,
    struct cosclua_buffer *buffer
)
{
    return cosclua_bufferseek(L, buffer, buffer->size);
}

lua_Integer cosclua_bufferappend(
    lua_State *L,
    struct cosclua_buffer *buffer,
    const struct cosclua_buffer *src,
    lua_Integer start,
    lua_Integer length
)
{
    cosclua_clamprange(&start, &length, src->size);
    if (length <= 0)
        return 0;
    lua_Integer old_size = buffer->size;
    lua_Integer ret = cosclua_bufferaddsize(L, buffer, length);
    if (ret < 0)
        return ret;
    cosclua_memcpy((char *)buffer->bytes + old_size, (const char *)src->bytes + start, length);
    return length;
}

lua_Integer cosclua_bufferinsert(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer offset,
    const struct cosclua_buffer *src,
    lua_Integer start,
    lua_Integer length
)
{
    if (offset < 0 || offset >= buffer->size)
        return -1;
    cosclua_clamprange(&start, &length, src->size);
    if (length <= 0)
        return 0;
    lua_Integer old_size = buffer->size;
    lua_Integer ret = cosclua_bufferaddsize(L, buffer, length);
    if (ret < 0)
        return ret;
    lua_Integer bytes_to_move = old_size - offset - length;
    for (lua_Integer i = 1; i <= bytes_to_move; i++)
        ((unsigned char *)buffer->bytes)[buffer->size - i] = ((const unsigned char *)buffer->bytes)[offset + length - i];
    cosclua_memcpy((char *)buffer->bytes + offset, (const char *)src->bytes + start, length);
    return length;
}

lua_Integer cosclua_buffercopy(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer offset,
    const struct cosclua_buffer *src,
    lua_Integer start,
    lua_Integer length
)
{
    if (offset < 0 || offset >= buffer->size)
        return -1;
    cosclua_clamprange(&start, &length, src->size);
    if (offset + length > buffer->size)
        length = buffer->size - offset;
    if (length <= 0)
        return 0;
    for (lua_Integer i = 0; i < length; i++)
        ((unsigned char *)buffer)[offset + i] = ((const unsigned char *)src)[start + i];
    return length;
}

lua_Integer cosclua_bufferremove(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer offset,
    lua_Integer length
)
{
    if (offset < 0 || offset >= buffer->size)
        return -1;
    cosclua_clamprange(&offset, &length, buffer->size);
    if (length <= 0)
        return 0;
    lua_Integer bytes_to_move = buffer->size - offset - length;
    for (lua_Integer i = 0; i < bytes_to_move; i++)
        ((unsigned char *)buffer->bytes)[offset + i] = ((const unsigned char *)buffer->bytes)[offset + length + i];
    cosclua_buffersetsize(L, buffer, buffer->size - length);
    if (buffer->offset > buffer->size)
        buffer->offset = buffer->size;
    return length;
}

lua_Integer cosclua_bufferwritebytes(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer length,
    const void *bytes
)
{
    if (length <= 0)
        return 0;
    lua_Integer ret = cosclua_bufferaddsize(L, buffer, length);
    if (ret < 0)
        return ret;
    if (bytes != NULL)
        cosclua_memcpy((char *)buffer->bytes + buffer->offset, bytes, length);
    else
        cosclua_memset((char *)buffer->bytes + buffer->offset, 0, length);
    buffer->offset += length;
    return length;
}

lua_Integer cosclua_bufferreadbytes(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer length,
    void *bytes
)
{
    if (length <= 0)
        return 0;
    if (length > buffer->size - buffer->offset)
        return COSC_EOVERRUN;
    if (bytes != NULL)
        cosclua_memcpy(bytes, (char *)buffer->bytes + buffer->offset, length);
    buffer->offset += length;
    return length;
}

lua_Integer cosclua_bufferwriteuint32(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer value
)
{
    lua_Integer ret = cosclua_bufferaddsize(L, buffer, 4);
    if (ret < 0)
        return ret;
    return cosc_write_uint32((char *)buffer->bytes + buffer->offset, buffer->size - buffer->offset, value);
}

lua_Integer cosclua_bufferreaduint32(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer *value
)
{
    lua_Integer ret = cosclua_bufferaddsize(L, buffer, 4);
    if (ret < 0)
        return ret;
    cosc_uint32 tmp = 0;
    ret = cosc_read_uint32((char *)buffer->bytes + buffer->offset, buffer->size - buffer->offset, &tmp);
    if (value)
    {
        if (sizeof(lua_Integer) == 4)
        {
#ifdef __cplusplus
            *value = *(reinterpret_cast<int *>(&tmp));
#else
            *value = ((union { cosc_uint32 in; cosc_int32 out; }){tmp}).out;
#endif
        }
        else
            *value = tmp;
    }
    return ret;
}

lua_Integer cosclua_bufferwriteint32(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer value
)
{
    lua_Integer ret = cosclua_bufferaddsize(L, buffer, 4);
    if (ret < 0)
        return ret;
    return cosc_write_int32((char *)buffer->bytes + buffer->offset, buffer->size - buffer->offset, value);
}

lua_Integer cosclua_bufferreadint32(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Integer *value
)
{
    lua_Integer ret = cosclua_bufferaddsize(L, buffer, 4);
    if (ret < 0)
        return ret;
    cosc_int32 tmp = 0;
    ret = cosc_read_int32((char *)buffer->bytes + buffer->offset, buffer->size - buffer->offset, &tmp);
    if (value) *value = tmp;
    return ret;
}

lua_Integer cosclua_bufferwritefloat32(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Number value
)
{
    lua_Integer ret = cosclua_bufferaddsize(L, buffer, 4);
    if (ret < 0)
        return ret;
    return cosc_write_float32((char *)buffer->bytes + buffer->offset, buffer->size - buffer->offset, value);
}

lua_Integer cosclua_bufferreadfloat32(
    lua_State *L,
    struct cosclua_buffer *buffer,
    lua_Number *value
)
{
    lua_Integer ret = cosclua_bufferaddsize(L, buffer, 4);
    if (ret < 0)
        return ret;
    cosc_float32 tmp = 0;
    ret = cosc_read_float32((char *)buffer->bytes + buffer->offset, buffer->size - buffer->offset, &tmp);
    if (value)
        *value = tmp;
    return ret;
}
