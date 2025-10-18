/**
 * cosc
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

#include "cosc.h"

#ifdef COSC_NOSTDLIB
static void *cosc_memcpy(void *dest, const void *src, cosc_int32 n)
{
    for (cosc_int32 i = 0; i < n; i++)
        ((unsigned char *)dest)[i] = ((const unsigned char *)src)[i];
    return dest;
}
static void *cosc_memset(void *s, int c, cosc_int32 n)
{
    for (cosc_int32 i = 0; i < n; i++)
        ((unsigned char *)s)[i] = c;
    return s;
}
static cosc_int32 cosc_memcmp(const void *a, const void *b, cosc_int32 n)
{
    for (cosc_int32 i = 0; i < n; i++)
    {
        if (((const unsigned char *)a)[i] < ((const unsigned char *)b)[i])
            return -1;
        else if (((const unsigned char *)a)[i] > ((const unsigned char *)b)[i])
            return 1;
    }
    return 0;
}
#elif defined(__cplusplus)
#include <cstring>
#define cosc_memcpy std::memcpy
#define cosc_memset std::memset
#define cosc_memcmp std::memcmp
#else
#include <string.h>
#define cosc_memcpy memcpy
#define cosc_memset memset
#define cosc_memcmp memcmp
#endif

#define COSC_COPY32(dst, src) \
    do { \
        ((unsigned char *)(dst))[0] = ((const unsigned char *)(src))[0]; \
        ((unsigned char *)(dst))[1] = ((const unsigned char *)(src))[1]; \
        ((unsigned char *)(dst))[2] = ((const unsigned char *)(src))[2]; \
        ((unsigned char *)(dst))[3] = ((const unsigned char *)(src))[3]; \
    } while (0)
#define COSC_COPY64(dst, src) \
    do { \
        ((unsigned char *)(dst))[0] = ((const unsigned char *)(src))[0]; \
        ((unsigned char *)(dst))[1] = ((const unsigned char *)(src))[1]; \
        ((unsigned char *)(dst))[2] = ((const unsigned char *)(src))[2]; \
        ((unsigned char *)(dst))[3] = ((const unsigned char *)(src))[3]; \
        ((unsigned char *)(dst))[4] = ((const unsigned char *)(src))[4]; \
        ((unsigned char *)(dst))[5] = ((const unsigned char *)(src))[5]; \
        ((unsigned char *)(dst))[6] = ((const unsigned char *)(src))[6]; \
        ((unsigned char *)(dst))[7] = ((const unsigned char *)(src))[7]; \
    } while (0)
#define COSC_COPY64SWAP(dst, src) \
    do { \
        ((unsigned char *)(dst))[0] = ((const unsigned char *)(src))[7]; \
        ((unsigned char *)(dst))[1] = ((const unsigned char *)(src))[6]; \
        ((unsigned char *)(dst))[2] = ((const unsigned char *)(src))[5]; \
        ((unsigned char *)(dst))[3] = ((const unsigned char *)(src))[4]; \
        ((unsigned char *)(dst))[4] = ((const unsigned char *)(src))[3]; \
        ((unsigned char *)(dst))[5] = ((const unsigned char *)(src))[2]; \
        ((unsigned char *)(dst))[6] = ((const unsigned char *)(src))[1]; \
        ((unsigned char *)(dst))[7] = ((const unsigned char *)(src))[0]; \
    } while (0)

#if __cplusplus >= 202002L && !defined(COSC_NOSTDLIB)
#include <bit>
#define COSC_PUN(type_in_, type_out_, value_) (std::bit_cast<type_out_>((type_in_)(value_)))
#elif defined(__cplusplus)
template <typename IN, typename OUT>
inline OUT cosc_pun(IN in) { OUT out; cosc_memcpy(&out, &in, sizeof(out)); return out; }
#define COSC_PUN(type_in_, type_out_, value_) cosc_pun<type_in_, type_out_>((type_in_)(value_))
#else
#define COSC_PUN(type_in_, type_out_, value_) (((union { type_in_ in; type_out_ out; }){.in=(value_)}).out)
#endif

inline static void cosc_store_uint32(
    void *buffer,
    cosc_uint32 value
)
{
#ifdef COSC_NOSWAP
    COSC_COPY32(buffer, &value);
#else
    ((unsigned char *)(buffer))[0] = ((value) & 0xff000000) >> 24;
    ((unsigned char *)(buffer))[1] = ((value) & 0xff0000) >> 16;
    ((unsigned char *)(buffer))[2] = ((value) & 0xff00) >> 8;
    ((unsigned char *)(buffer))[3] = (value) & 0xff;
#endif
}

inline static cosc_uint32 cosc_load_uint32(
    const void *buffer
)
{
#ifdef COSC_NOSWAP
    cosc_uint32 tmp;
    COSC_COPY32(&tmp, buffer);
    return tmp;
#else
    return (
        ((cosc_uint32)((const unsigned char *)(buffer))[0] << 24)
        | ((cosc_uint32)((const unsigned char *)(buffer))[1] << 16)
        | ((cosc_uint32)((const unsigned char *)(buffer))[2] << 8)
        | ((cosc_uint32)((const unsigned char *)(buffer))[3])
    );
#endif
}

inline static void cosc_store_int32(
    void *buffer,
    cosc_int32 value
)
{
    cosc_store_uint32(buffer, COSC_PUN(cosc_int32, cosc_uint32, value));
}

inline static cosc_int32 cosc_load_int32(
    const void *buffer
)
{
    return COSC_PUN(cosc_uint32, cosc_int32, cosc_load_uint32(buffer));
}

inline static void cosc_store_uint64(
    void *buffer,
    cosc_uint64 value
)
{
#ifdef COSC_NOSWAP
    COSC_COPY64(buffer, &value);
#else
#ifdef COSC_NOINT64
    cosc_store_uint32(buffer, COSC_64BITS_HI(&value));
    cosc_store_uint32((char *)buffer + 4, COSC_64BITS_LO(&value));
#else
    ((unsigned char *)buffer)[0] = (value & 0xff00000000000000ULL) >> 56;
    ((unsigned char *)buffer)[1] = (value & 0xff000000000000ULL) >> 48;
    ((unsigned char *)buffer)[2] = (value & 0xff0000000000ULL) >> 40;
    ((unsigned char *)buffer)[3] = (value & 0xff00000000ULL) >> 32;
    ((unsigned char *)buffer)[4] = (value & 0xff000000) >> 24;
    ((unsigned char *)buffer)[5] = (value & 0xff0000) >> 16;
    ((unsigned char *)buffer)[6] = (value & 0xff00) >> 8;
    ((unsigned char *)buffer)[7] = value & 0xff;
#endif
#endif
}

inline static cosc_uint64 cosc_load_uint64(
    const void *buffer
)
{
#ifdef COSC_NOSWAP
    cosc_uint64 tmp;
    COSC_COPY64(&tmp, buffer);
    return tmp;
#else
#ifdef COSC_NOINT64
    struct cosc_64bits tmp = COSC_64BITS_INIT(cosc_load_uint32(buffer), cosc_load_uint32((char *)buffer + 4));
    return tmp;
#else
    return (
        ((cosc_uint64)((const unsigned char *)buffer)[0] << 56)
        | ((cosc_uint64)((const unsigned char *)buffer)[1] << 48)
        | ((cosc_uint64)((const unsigned char *)buffer)[2] << 40)
        | ((cosc_uint64)((const unsigned char *)buffer)[3] << 32)
        | ((cosc_uint64)((const unsigned char *)buffer)[4] << 24)
        | ((cosc_uint64)((const unsigned char *)buffer)[5] << 16)
        | ((cosc_uint64)((const unsigned char *)buffer)[6] << 8)
        | ((cosc_uint64)((const unsigned char *)buffer)[7])
    );
#endif
#endif
}

#if defined(COSC_NOINT64) || defined(COSC_NOTIMETAG)

static struct cosc_64bits cosc_mul64(
    cosc_uint32 a,
    cosc_uint32 b
)
{
    cosc_uint32 product[4];
    cosc_uint32 factor_a[2] = {
        a & 0xffff,
        (a & 0xffff0000) >> 16
    };
    cosc_uint32 factor_b[2] = {
        b & 0xffff,
        (b & 0xffff0000) >> 16
    };
    product[0] = factor_a[0] * factor_b[0];
    product[1] = (product[0] & 0xffff0000) >> 16;
    product[0] &= 0xffff;

    product[1] += factor_a[1] * factor_b[0];
    product[2] = (product[1] & 0xffff0000) >> 16;
    product[1] &= 0xffff;

    product[1] += factor_a[0] * factor_b[1];
    product[2] += (product[1] & 0xffff0000) >> 16;
    product[1] &= 0xffff;
    product[3] = (product[2] & 0xffff0000) >> 16;
    product[2] &= 0xffff;

    product[2] += factor_a[1] * factor_b[1];
    product[3] += (product[2] & 0xffff0000) >> 16;
    product[2] &= 0xffff;
    product[3] &= 0xffff;

    struct cosc_64bits res = COSC_64BITS_INIT(
        (product[3] << 16) | product[2],
        (product[1] << 16) | product[0]
    );
    return res;
}

static void cosc_div64(struct cosc_64bits *dividend, cosc_uint32 divisor)
{
    struct cosc_64bits q = COSC_64BITS_INIT(0, 0);
    struct cosc_64bits r = COSC_64BITS_INIT(0, 0);
    if (divisor > 0)
    {
        for (cosc_int32 i = 63; i >= 0; i--)
        {
            COSC_64BITS_HI(&r) <<= 1;
            COSC_64BITS_HI(&r) |= (COSC_64BITS_LO(&r) & 0x80000000) >> 31;
            COSC_64BITS_LO(&r) <<= 1;
            if (i < 32)
                COSC_64BITS_LO(&r) |= (COSC_64BITS_LO(dividend) & (1U << i)) >> i;
            else
                COSC_64BITS_LO(&r) |= (COSC_64BITS_HI(dividend) & (1U << (i - 32))) >> (i - 32);
            if (COSC_64BITS_LO(&r) >= divisor || COSC_64BITS_HI(&r) > 0)
            {
                if (COSC_64BITS_LO(&r) < divisor)
                    COSC_64BITS_HI(&r)--;
                COSC_64BITS_LO(&r) -= divisor;
                if (i < 32)
                    COSC_64BITS_LO(&q) |= 1U << i;
                else
                    COSC_64BITS_HI(&q) |= 1U << (i - 32);
            }
        }
    }
    *dividend = q;
}

static void cosc_add64(struct cosc_64bits *augend, cosc_uint32 addend)
{
    if (addend > 0xffffffff - COSC_64BITS_LO(augend))
        COSC_64BITS_HI(augend)++;
    COSC_64BITS_LO(augend) += addend;
}

#endif /* COSC_NOINT64 || COSC_NOTIMETAG */

#ifndef COSC_NOPATTERN

static cosc_int32 cosc_strncmp(const char *a, cosc_int32 a_n, const char *b, cosc_int32 b_n)
{
    if (a_n <= 0 && b_n <= 0)
        return 0;
    if (a_n <= 0 && b_n > 0 && *b != 0)
        return -1;
    if (b_n <= 0 && a_n > 0 && *a != 0)
        return -1;
    for (cosc_int32 i = 0; i < a_n && i < b_n; i++)
    {
        if (a[i] < b[i])
            return -1;
        else if (a[i] > b[i])
            return 1;
        if (a[i] == 0)
            break;
    }
    return 0;
}

static cosc_int32 cosc_charset_match(
    char character,
    const char *charset,
    cosc_int32 charset_n,
    cosc_int32 *forward
)
{
    cosc_int32 len = 0;
    if (charset_n < 2 || charset[len] != '[' || !character)
    {
        if (forward)
            *forward = len;
        return 0;
    }
    len++;
    cosc_int32 found = -1;
    while (len < charset_n && charset[len] != 0 && charset[len] != ']')
    {
        if (charset[len] == character)
            found = len;
        len++;
    }
    if (len < charset_n && charset[len] == ']'
        && (found >= 0 || charset[len - 1] == '['))
    {
        len++;
        if (forward)
            *forward = len;
        return 1;
    }
    if (forward)
        *forward = len;
    return 0;
}

static cosc_int32 cosc_stringset_match(
    const char *s,
    cosc_int32 s_n,
    const char *stringset,
    cosc_int32 stringset_n,
    cosc_int32 *forward,
    cosc_int32 *s_forward
)
{
    cosc_int32 len = 0, slen = 0;
    if (stringset_n < 2 || stringset[len] != '{')
    {
        if (forward)
            *forward = 0;
        if (s_forward)
            *s_forward = 0;
        return 0;
    }
    len++;
    while (len < stringset_n && stringset[len] != 0 && stringset[len] != '}')
    {
        cosc_int32 end = len;
        while (end < stringset_n && stringset[end] != 0 && stringset[end] != '}' && stringset[end] != ',')
            end++;
        slen = end - len;
        if (cosc_strncmp(stringset + len, slen, s, s_n) == 0)
        {
            len = end;
            while (len < stringset_n && stringset[len] != 0 && stringset[len] != '}')
                len++;
            break;
        }
        else
            slen = 0;
        len = end + 1;
    }
    if (len < stringset_n && stringset[len] == '}')
    {
        len++;
        if (forward)
            *forward = len;
        if (s_forward)
            *s_forward = slen;
        return 1;
    }
    if (forward)
        *forward = len;
    if (s_forward)
        *s_forward = 0;
    return 0;
}

#endif /* !COSC_NOPATTERN */

static cosc_int32 cosc_type_is_valid(
    char type,
    cosc_int32 is_pattern
)
{
    switch (type)
    {
    case 'i':
    case 'f':
    case 's':
    case 'b':
    case 'h':
    case 't':
    case 'd':
    case 'S':
    case 'c':
    case 'r':
    case 'm':
    case 'T':
    case 'F':
    case 'N':
    case 'I':
        return 1;
    }
    if (is_pattern)
    {
        switch (type)
        {
        case '*':
        case '?':
        case '[':
        case '{':
        case '#':
            return 1;
        }
    }
    return 0;
}

static cosc_int32 cosc_type_is_payload(char type)
{
    switch (type)
    {
    case 'i':
    case 'f':
    case 's':
    case 'b':
    case 'h':
    case 't':
    case 'd':
    case 'S':
    case 'c':
    case 'r':
    case 'm':
        return 1;
    }
    return 0;
}

//
// Public below.
//

cosc_int32 cosc_feature_int64(void)
{
#ifdef COSC_NOINT64
    return 0;
#else
    return 1;
#endif
}

cosc_int32 cosc_feature_float32(void)
{
#ifdef COSC_NOFLOAT32
    return 0;
#else
    return 1;
#endif
}

cosc_int32 cosc_feature_float64(void)
{
#ifdef COSC_NOFLOAT64
    return 0;
#else
    return 1;
#endif
}

cosc_int32 cosc_feature_swap(void)
{
#ifdef COSC_NOSWAP
    return 0;
#else
    return 1;
#endif
}

cosc_int32 cosc_feature_array(void)
{
#ifdef COSC_NOARRAY
    return 0;
#else
    return 1;
#endif
}

cosc_int32 cosc_feature_pattern(void)
{
#ifdef COSC_NOPATTERN
    return 0;
#else
    return 1;
#endif
}

cosc_int32 cosc_feature_timetag(void)
{
#ifdef COSC_NOTIMETAG
    return 0;
#else
    return 1;
#endif
}

cosc_int32 cosc_feature_fltconv(void)
{
#ifdef COSC_NOTIMETAG
    return 0;
#else
    return 1;
#endif
}

cosc_int32 cosc_feature_writer(void)
{
#ifdef COSC_NOWRITER
    return 0;
#else
    return 1;
#endif
}

cosc_int32 cosc_feature_reader(void)
{
#ifdef COSC_NOREADER
    return 0;
#else
    return 1;
#endif
}

cosc_int32 cosc_big_endian(void)
{
    const cosc_uint32 u = 1;
    return ((const unsigned char *)&u)[3];
}

cosc_int32 cosc_address_char_validate(
    cosc_int32 c
)
{
    switch (c)
    {
    case ' ':
    case '#':
    case '*':
    case ',':
    case '?':
    case '[':
    case ']':
    case '{':
    case '}':
        return 0;
    }
    return c > 32 ? c : 0;
}

cosc_int32 cosc_address_validate(
    const char *address,
    cosc_int32 address_n,
    cosc_int32 *invalid
)
{
    cosc_int32 len = 0;
    if (address_n <= 0 || *address == 0)
    {
        if (invalid)
            *invalid = -1;
        return 1;
    }
    while (len < address_n && address[len] != 0)
    {
        if (!cosc_address_char_validate(address[len]))
        {
            if (invalid)
                *invalid = len;
            return 0;
        }
        len++;
    }
    if (len >= COSC_SIZE_MAX)
    {
        if (invalid)
            *invalid = COSC_SIZE_MAX;
        return 0;
    }
    if (invalid)
        *invalid = -1;
    return 1;
}

cosc_int32 cosc_typetag_char_validate(
    cosc_int32 c
)
{
    return cosc_type_is_valid(c, 0);
}

cosc_int32 cosc_typetag_validate(
    const char *typetag,
    cosc_int32 typetag_n,
    cosc_int32 *invalid
)
{
    cosc_int32 len = 0;
    if (typetag_n <= 0 || *typetag == 0)
    {
        if (invalid)
            *invalid = -1;
        return 0;
    }
    if (*typetag != ',')
    {
        if (invalid)
            *invalid = 0;
        return 0;
    }
    len++;
#ifndef COSC_NOARRAY
    char array = 0;
#endif
    while (len < typetag_n && typetag[len] != 0)
    {
#ifndef COSC_NOARRAY
        if (array == ']')
            break;
        if (typetag[len] == '[')
        {
            if (array)
            {
                if (invalid)
                    *invalid = len;
                return 0;
            }
            array = '[';
            len++;
            continue;
        }
        if (typetag[len] == ']')
        {
            len++;
            array = ']';
            continue;
        }
#endif
        if (!cosc_type_is_valid(typetag[len], 0))
        {
            if (invalid)
                *invalid = len;
            return 0;
        }
        len++;
    }
#ifndef COSC_NOARRAY
    if (array == '[')
    {
        if (invalid)
            *invalid = len;
        return 0;
    }
#endif
    if (len >= COSC_SIZE_MAX)
    {
        if (invalid)
            *invalid = COSC_SIZE_MAX;
        return 0;
    }
    if (invalid)
        *invalid = -1;
    return 1;
}

cosc_int32 cosc_typetag_payload(
    char *s,
    cosc_int32 s_n,
    const char *typetag,
    cosc_int32 typetag_n,
    cosc_int32 *array_members
)
{
    cosc_int32 len = 0;
    cosc_int32 payloads = 0;
    cosc_int32 array_count = 0;
#ifndef COSC_NOARRAY
    char array_started = 0;
#endif
    if (typetag)
    {
        while (len < typetag_n && typetag[len] != 0)
        {
#ifndef COSC_NOARRAY
            if (typetag[len] == '[')
                array_started = 1;
#endif
            if (cosc_type_is_payload(typetag[len]))
            {
                if (s && payloads < s_n)
                    s[payloads] = typetag[len];
                payloads++;
#ifndef COSC_NOARRAY
                if (array_started)
                    array_count++;
#endif
            }
            len++;
        }
    }
    if (s && s_n > 0)
        s[payloads < s_n ? payloads : s_n - 1] = 0;
    if (array_members)
        *array_members = array_count;
    return payloads;
}

#ifndef COSC_NOPATTERN

cosc_int32 cosc_pattern_char_validate(
    cosc_int32 c
)
{
    return cosc_type_is_valid(c, 1);
}

cosc_int32 cosc_pattern_validate(
    const char *s,
    cosc_int32 s_n,
    cosc_int32 *invalid
)
{
    cosc_int32 len = 0;
    if (s_n <= 0 || *s == 0)
    {
        if (invalid)
            *invalid = -1;
        return 1;
    }
    char open = 0;
    while (len < s_n && s[len] != 0)
    {
        if (s[len] <= 32)
        {
            if (invalid)
                *invalid = len;
            return 0;
        }
        if (s[len] == '[' || s[len] == '{')
        {
            if (open)
            {
                if (invalid)
                    *invalid = len;
                return 0;
            }
            open = s[len];
        }
        else if (s[len] == ']' || s[len] == '}')
        {
            if (s[len] != open)
            {
                if (invalid)
                    *invalid = len;
                return 0;
            }
            open = 0;
        }
        len++;
    }
    if (len >= COSC_SIZE_MAX)
    {
        if (invalid)
            *invalid = COSC_SIZE_MAX;
        return 0;
    }
    if (open)
    {
        if (invalid)
            *invalid = len;
        return 0;
    }
    if (invalid)
        *invalid = -1;
    return 1;
}

cosc_int32 cosc_pattern_match(
    const char *s,
    cosc_int32 s_n,
    const char *pattern,
    cosc_int32 pattern_n
)
{
    cosc_int32 s_offset = 0, p_offset = 0, slen, plen;
    char is_typetag;
    if ((s_n <= 0 || *s == 0) && (pattern_n <= 0 || *pattern == 0))
        return 1;
    if (s_n > 0 && *s == ',')
    {
        is_typetag = 1;
        s_offset++;
    }
    else
        is_typetag = 0;
    if (pattern_n > 0 && *pattern == ',' && is_typetag)
        p_offset++;
    while (
        s_offset < s_n && s[s_offset] != 0
        && p_offset < pattern_n && pattern[p_offset] != 0)
    {
#ifndef COSC_NOARRAY
        if (is_typetag && (s[s_offset] == '[' || s[s_offset] == ']'))
        {
            s_offset++;
            continue;
        }
#endif
        if (pattern[p_offset] == '?')
        {
            p_offset++;
            s_offset++;
        }
        else if (pattern[p_offset] == '#')
        {
            if (is_typetag)
            {
                switch (s[s_offset])
                {
                case 'i':
                case 'r':
                case 'f':
                case 'h':
                case 't':
                case 'd':
                    break;
                default:
                    return 0;
                }
            }
            else if (s[s_offset] < '0' || s[s_offset] > '9')
                return 0;
            p_offset++;
            s_offset++;
        }
        else if (pattern[p_offset] == 'B')
        {
            if (!is_typetag || (s[s_offset] != 'T' && s[s_offset] != 'F'))
                return 0;
            p_offset++;
            s_offset++;
        }
        else if (pattern[p_offset] == '*')
        {
            while (p_offset < pattern_n && pattern[p_offset] == '*')
                p_offset++;
            if (p_offset >= pattern_n || pattern[p_offset] == 0)
                return 1;
            while (s_offset < s_n && s[s_offset] != 0 && s[s_offset] != pattern[p_offset])
                s_offset++;
            if (s_offset >= s_n || s[s_offset] != pattern[p_offset])
                return 0;
            p_offset++;
            s_offset++;
        }
        else if (pattern[p_offset] == '[')
        {
            if (!cosc_charset_match(s[s_offset], pattern + p_offset, pattern_n - p_offset, &plen))
                return 0;
            s_offset++;
            p_offset += plen;
        }
        else if (pattern[p_offset] == '{')
        {
            if (!cosc_stringset_match(s + s_offset, s_n - s_offset,  pattern + p_offset, pattern_n - p_offset, &plen, &slen))
                return 0;
            s_offset += slen;
            p_offset += plen;
        }
        else if (s[s_offset] == pattern[p_offset])
        {
            p_offset++;
            s_offset++;
        }
        else
            return 0;
    }
#ifndef COSC_NOARRAY
    if (s_offset < s_n && s[s_offset] == ']')
        s_offset++;
#endif

    while (p_offset < pattern_n)
    {
        if (pattern[p_offset] == '*')
            p_offset++;
        else if (pattern[p_offset] == '[')
        {
            if (!cosc_charset_match(0, pattern + p_offset, pattern_n - p_offset, &plen))
                return 0;
            p_offset += plen;
        }
        else if (pattern[p_offset] == '{')
        {
            if (!cosc_stringset_match("", 0,  pattern + p_offset, pattern_n - p_offset, &plen, &slen))
                return 0;
            s_offset += slen;
            p_offset += plen;
        }
        else if (pattern[p_offset] == 0)
            break;
        else
            return 0;
    }

    if ((s_offset >= s_n || s[s_offset] == 0)
        && (p_offset >= pattern_n || pattern[p_offset] == 0))
        return 1;
    return 0;
}

cosc_int32 cosc_signature_match(
    const void *buffer,
    cosc_int32 size,
    const char *apattern,
    cosc_int32 apattern_n,
    const char *tpattern,
    cosc_int32 tpattern_n,
    cosc_int32 prefix
)
{
    cosc_int32 sz, len;
    if (prefix)
    {
        if (size < 12)
            return 0;
        prefix = cosc_load_int32(buffer);
        if (prefix < 8 || prefix > COSC_SIZE_MAX - 8 || COSC_PAD(prefix))
            return 0;
        buffer = (const char *)buffer + 4;
    }
    else if (size < 8)
        return 0;
    sz = cosc_read_string(buffer, size, 0, 0, &len);
    if (sz < 0)
        return sz;
    if (cosc_pattern_match((const char *)buffer, sz, apattern, apattern_n)
        && cosc_pattern_match((const char *)buffer + sz, size - sz, tpattern, tpattern_n))
        return 1;
    return 0;
}

#endif /* COSC_NOPATTERN */

#ifndef COSC_NOTIMETAG

cosc_uint32 cosc_timetag_to_time(
    cosc_uint64 timetag,
    cosc_uint32 *nanos
)
{
#ifdef COSC_NOINT64
    if (nanos)
    {
        struct cosc_64bits tmp = cosc_mul64(COSC_64BITS_LO(&timetag), 1000000000);
        cosc_add64(&tmp, 500000000);
        *nanos = COSC_64BITS_HI(&tmp); // Same as >> 32.
    }
    return COSC_64BITS_HI(&timetag);
#else
    if (nanos)
    {
        cosc_uint64 tmp = timetag & 0xffffffff;
        tmp *= 1000000000;
        tmp += 500000000;
        tmp >>= 32;
        *nanos = tmp;
    }
    return (timetag & 0xffffffff00000000ULL) >> 32;
#endif
}

cosc_uint64 cosc_time_to_timetag(
    cosc_uint32 seconds,
    cosc_uint32 nanos
)
{
    seconds += nanos / 1000000000;
    nanos %= 1000000000;
#ifdef COSC_NOINT64
    struct cosc_64bits res = COSC_64BITS_INIT(nanos, 0);
    cosc_add64(&res, 0x20000000);
    cosc_div64(&res, 1000000000);
    COSC_64BITS_HI(&res) = seconds;
    return res;
#else
    cosc_uint64 tmp = nanos;
    tmp <<= 32;
    tmp += 0x20000000;
    tmp /= 1000000000;
    tmp |= (cosc_uint64)seconds << 32;
    return tmp;
#endif
}

#endif /* !COSC_NOTIMETAG */

#ifndef COSC_NOFLTCONV

cosc_float32 cosc_float64_to_float32(
    cosc_float64 value
)
{
    // FIXME: sub normal numbers.
#ifndef COSC_NOFLOAT64
#ifndef COSC_NOINT64
    cosc_uint64 tmpbits = COSC_PUN(cosc_float64, cosc_uint64, value);
    struct cosc_64bits bits = COSC_64BITS_INIT(tmpbits >> 32, tmpbits & 0xffffffff);
#else
    struct cosc_64bits bits = COSC_PUN(cosc_float64, struct cosc_64bits, value);
    if (!cosc_big_endian())
    {
        cosc_uint32 tmp = COSC_64BITS_HI(&bits);
        COSC_64BITS_HI(&bits) = COSC_64BITS_LO(&bits);
        COSC_64BITS_LO(&bits) = tmp;
    }
#endif
#else
    struct cosc_64bits bits = value;
#endif
    cosc_uint32 sign = COSC_64BITS_HI(&bits) & 0x80000000;
    cosc_int32 exponent = (COSC_64BITS_HI(&bits) >> 20) & 0x7ff;
    cosc_uint32 fraction = (COSC_64BITS_HI(&bits) & 0xfffff) << 3;
    fraction |= COSC_64BITS_LO(&bits) >> 29;
    if (exponent >= 0x7ff)
    {
        exponent = 0xff;
        fraction = 0;
    }
    else if (exponent > 0)
    {
        exponent -= 1023;
        exponent += 127;
        int round = (COSC_64BITS_LO(&bits) << 3) >= 0x80000000;
        if (round)
        {
            fraction++;
            if (fraction > 0x7fffff)
            {
                exponent++;
                fraction &= fraction;
                if (exponent >= 0xff)
                {
                    exponent = 0xff;
                    fraction = 0;
                }
            }
        }
    }
#ifndef COSC_NOFLOAT32
    return COSC_PUN(cosc_uint32, cosc_float32, sign | ((cosc_uint32)exponent << 23) | fraction);
#else
    return sign | ((cosc_uint32)exponent << 23) | fraction;
#endif
}

cosc_float64 cosc_float32_to_float64(
    cosc_float32 value
)
{
#ifndef COSC_NOFLOAT32
    cosc_uint32 bits = COSC_PUN(cosc_float32, cosc_uint32, value);
#else
    cosc_uint32 bits = value;
#endif
    cosc_uint32 sign = (bits & 0x80000000);
    cosc_int32 exponent = (bits >> 23) & 0xff;
    cosc_uint32 fraction = bits & 0x7fffff;
    if (exponent == 0xff)
    {
        exponent = 0x7ff;
        fraction = 0;
    }
    else if (exponent > 0)
    {
        exponent -= 127;
        exponent += 1023;
    }
    struct cosc_64bits ret = COSC_64BITS_INIT(sign | ((cosc_uint32)exponent << 20), (fraction & 0x7) << 29);
    COSC_64BITS_HI(&ret) |= fraction >> 3;
#if defined(COSC_NOFLOAT64)
    return ret;
#elif defined(COSC_NOINT64)
    if (!cosc_big_endian())
    {
        cosc_uint32 tmp = COSC_64BITS_HI(&ret);
        COSC_64BITS_HI(&ret) = COSC_64BITS_LO(&ret);
        COSC_64BITS_LO(&ret) = tmp;
    }
    return COSC_PUN(struct cosc_64bits, cosc_float64, ret);
#else
    return COSC_PUN(cosc_uint64, cosc_float64, ((cosc_uint64)COSC_64BITS_HI(&ret) << 32) | COSC_64BITS_LO(&ret));
#endif
}

#endif /* COSC_NOFLTCONV */

cosc_int32 cosc_write_uint32(
    void *buffer,
    cosc_int32 size,
    cosc_uint32 value
)
{
    if (buffer)
    {
        if (size < 4) return COSC_EOVERRUN;
        cosc_store_uint32(buffer, value);
    }
    return 4;
}

cosc_int32 cosc_read_uint32(
    const void *buffer,
    cosc_int32 size,
    cosc_uint32 *value
)
{
    if (size < 4)
        return COSC_EOVERRUN;
    if (value) *value = cosc_load_uint32(buffer);
    return 4;
}

cosc_int32 cosc_write_int32(
    void *buffer,
    cosc_int32 size,
    cosc_int32 value
)
{
    if (buffer)
    {
        if (size < 4) return COSC_EOVERRUN;
        cosc_store_uint32(buffer, COSC_PUN(cosc_int32, cosc_uint32, value));
    }
    return 4;
}

cosc_int32 cosc_read_int32(
    const void *buffer,
    cosc_int32 size,
    cosc_int32 *value
)
{
    if (size < 4)
        return COSC_EOVERRUN;
    if (value) *value = COSC_PUN(cosc_uint32, cosc_int32, cosc_load_uint32(buffer));
    return 4;
}

cosc_int32 cosc_write_float32(
    void *buffer,
    cosc_int32 size,
    cosc_float32 value
)
{
    if (buffer)
    {
        if (size < 4) return COSC_EOVERRUN;
        cosc_store_uint32(buffer, COSC_PUN(cosc_float32, cosc_uint32, value));
    }
    return 4;
}

cosc_int32 cosc_read_float32(
    const void *buffer,
    cosc_int32 size,
    cosc_float32 *value
)
{
    if (size < 4)
        return COSC_EOVERRUN;
    if (value) *value = COSC_PUN(cosc_uint32, cosc_float32, cosc_load_uint32(buffer));
    return 4;
}

cosc_int32 cosc_write_uint64(
    void *buffer,
    cosc_int32 size,
    cosc_uint64 value
)
{
    if (buffer)
    {
        if (size < 8) return COSC_EOVERRUN;
        cosc_store_uint64(buffer, value);
    }
    return 8;
}

cosc_int32 cosc_read_uint64(
    const void *buffer,
    cosc_int32 size,
    cosc_uint64 *value
)
{
    if (size < 8)
        return COSC_EOVERRUN;
    if (value) *value = cosc_load_uint64(buffer);
    return 8;
}

cosc_int32 cosc_write_int64(
    void *buffer,
    cosc_int32 size,
    cosc_int64 value
)
{
    if (buffer)
    {
        if (size < 8) return COSC_EOVERRUN;
        cosc_store_uint64(buffer, COSC_PUN(cosc_int64, cosc_uint64, value));
    }
    return 8;
}

cosc_int32 cosc_read_int64(
    const void *buffer,
    cosc_int32 size,
    cosc_int64 *value
)
{
    if (size < 8)
        return COSC_EOVERRUN;
    if (value) *value = COSC_PUN(cosc_uint64, cosc_int64, cosc_load_uint64(buffer));
    return 8;
}

cosc_int32 cosc_write_float64(
    void *buffer,
    cosc_int32 size,
    cosc_float64 value
)
{
    if (buffer)
    {
        if (size < 8)
            return COSC_EOVERRUN;
#if defined(COSC_NOSWAP)
        COSC_COPY64(buffer, &value);
#elif defined(COSC_NOFLOAT64)
        cosc_store_uint32(buffer, COSC_64BITS_HI(&value));
        cosc_store_uint32((char *)buffer + 4, COSC_64BITS_LO(&value));
#elif defined(COSC_NOINT64)
        if (!cosc_big_endian())
            COSC_COPY64SWAP(buffer, &value);
        else
            COSC_COPY64(buffer, &value);
#else
        cosc_store_uint64(buffer, COSC_PUN(cosc_float64, cosc_uint64, value));
#endif
    }
    return 8;
}

cosc_int32 cosc_read_float64(
    const void *buffer,
    cosc_int32 size,
    cosc_float64 *value
)
{
    if (size < 8)
        return COSC_EOVERRUN;
#if defined(COSC_NOSWAP)
    cosc_float64 tmp;
    COSC_COPY64(&tmp, buffer);
    if (value) *value = tmp;
#elif defined(COSC_NOFLOAT64)
    cosc_float64 tmp;
    COSC_64BITS_HI(&tmp) = cosc_load_uint32(buffer);
    COSC_64BITS_LO(&tmp) = cosc_load_uint32((const char *)buffer + 4);
    if (value) *value = tmp;
#elif defined(COSC_NOINT64)
    cosc_float64 tmp;
    if (!cosc_big_endian())
        COSC_COPY64SWAP(&tmp, buffer);
    else
        COSC_COPY64(&tmp, buffer);
    if (value) *value = tmp;
#else
    if (value) *value = COSC_PUN(cosc_uint64, cosc_float64, cosc_load_uint64(buffer));
#endif
    return 8;
}

cosc_int32 cosc_write_string(
    void *buffer,
    cosc_int32 size,
    const char *value,
    cosc_int32 value_n,
    cosc_int32 *length
)
{
    cosc_int32 len = 0, pad;
    if (buffer)
    {
        if (value)
        {
            while (len < value_n && value[len] != 0)
            {
                if (len >= size)
                    return COSC_EOVERRUN;
                ((char *)buffer)[len] = value[len];
                len++;
            }
        }
        pad = COSC_PADMUST(len);
        if (len > COSC_SIZE_MAX - pad)
            return COSC_ESIZEMAX;
        if (len + pad > size)
            return COSC_EOVERRUN;
        cosc_memset((char *)buffer + len, 0, pad);
    }
    else
    {
        if (value)
            while (len < value_n && value[len] != 0)
                len++;
        pad = COSC_PADMUST(len);
        if (len > COSC_SIZE_MAX - pad)
            return COSC_ESIZEMAX;
    }
    if (length)
        *length = len;
    return len + pad;
}

cosc_int32 cosc_read_string(
    const void *buffer,
    cosc_int32 size,
    char *value,
    cosc_int32 value_n,
    cosc_int32 *length
)
{
    cosc_int32 len = 0, pad;
    if (size < 4)
        return COSC_EOVERRUN;
    if (value && value_n > 0)
    {
        while (len < size && ((const char *)buffer)[len] != 0)
        {
            if (len < value_n)
                value[len] = ((const char *)buffer)[len];
            len++;
        }
        value[len < value_n ? len : value_n - 1] = 0;
    }
    else
    {
        while (len < size && ((const char *)buffer)[len] != 0)
            len++;
    }
    pad = COSC_PADMUST(len);
    if (len > COSC_SIZE_MAX - pad)
        return COSC_ESIZEMAX;
    if (len + pad > size)
        return COSC_EOVERRUN;
    if (length)
        *length = len;
    return len + pad;
}

cosc_int32 cosc_write_blob(
    void *buffer,
    cosc_int32 size,
    const void *value,
    cosc_int32 value_n
)
{
    if (value_n < 0)
        value_n = 0;
    else if (value_n > COSC_SIZE_MAX - 4)
        return COSC_ESIZEMAX;
    cosc_int32 pad = COSC_PAD(value_n);
    if (value_n + 4 > COSC_SIZE_MAX - pad)
        return COSC_ESIZEMAX;
    if (buffer)
    {
        if (value_n > size - pad - 4)
            return COSC_EOVERRUN;
        cosc_store_int32(buffer, value_n);
        if (value)
        {
            cosc_memcpy((char *)buffer + 4, value, value_n);
            cosc_memset((char *)buffer + 4 + value_n, 0, pad);
        }
        else
            cosc_memset((char *)buffer + 4, 0, value_n + pad);
    }
    return value_n + pad + 4;
}

cosc_int32 cosc_read_blob(
    const void *buffer,
    cosc_int32 size,
    void *value,
    cosc_int32 value_n,
    const void **data,
    cosc_int32 *data_size
)
{
    if (size < 4)
        return COSC_EOVERRUN;
    cosc_int32 psize = cosc_load_int32(buffer);
    if (psize > COSC_SIZE_MAX - 4)
        return COSC_ESIZEMAX;
    if (psize < 0)
        return COSC_EPSIZE;
    cosc_int32 pad = COSC_PAD(psize);
    if (psize > COSC_SIZE_MAX - pad - 4)
        return COSC_ESIZEMAX;
    if (psize + pad > size - 4)
        return COSC_EOVERRUN;
    if (value && value_n > 0)
        cosc_memcpy(value, (const char *)buffer + 4, psize < value_n ? psize : value_n);
    if (data)
        *data = psize > 0 ? (const char *)buffer + 4 : 0;
    if (data_size)
        *data_size = psize;
    return psize + 4 + pad;
}

cosc_int32 cosc_write_char(
    void *buffer,
    cosc_int32 size,
    cosc_int32 value
)
{
    if (buffer)
    {
        if (size < 4)
            return COSC_EOVERRUN;
        ((unsigned char *)buffer)[0] = value;
        ((unsigned char *)buffer)[1] = 0;
        ((unsigned char *)buffer)[2] = 0;
        ((unsigned char *)buffer)[3] = 0;
    }
    return 4;
}

cosc_int32 cosc_read_char(
    const void *buffer,
    cosc_int32 size,
    cosc_int32 *value
)
{
    if (size < 4)
        return COSC_EOVERRUN;
    if (value)
        *value = *(const char *)buffer;
    return 4;
}

cosc_int32 cosc_write_midi(
    void *buffer,
    cosc_int32 size,
    const unsigned char value[4]
)
{
    if (buffer)
    {
        if (size < 4)
            return COSC_EOVERRUN;
        if (value)
            cosc_memcpy(buffer, value, 4);
        else
            cosc_memset(buffer, 0, 4);
    }
    return 4;
}

cosc_int32 cosc_read_midi(
    const void *buffer,
    cosc_int32 size,
    unsigned char value[4]
)
{
    if (size < 4)
        return COSC_EOVERRUN;
    if (value)
        cosc_memcpy(value, buffer, 4);
    return 4;
}

cosc_int32 cosc_write_bundle(
    void *buffer,
    cosc_int32 size,
    cosc_uint64 timetag,
    cosc_int32 psize
)
{
    cosc_int32 req = psize != 0 ? 20 : 16;
    if (psize > 0)
    {
        if (psize < 16 || COSC_PAD(psize) || psize > COSC_SIZE_MAX - 4)
            return COSC_EPSIZE;
    }
    if (buffer)
    {
        if (size < req)
            return COSC_EOVERRUN;
        if (psize != 0)
        {
            if (psize > 0)
                cosc_store_int32(buffer, psize);
            else
                cosc_store_int32(buffer, 16);
            buffer = (char *)buffer + 4;
        }
        cosc_memcpy(buffer, "#bundle", 8);
        cosc_store_uint64((unsigned char *)buffer + 8, timetag);
    }
    return req;
}

cosc_int32 cosc_read_bundle(
    const void *buffer,
    cosc_int32 size,
    cosc_uint64 *timetag,
    cosc_int32 *psize
)
{
    cosc_int32 req = psize ? 20 : 16;
    if (size < req)
        return COSC_EOVERRUN;
    if (psize)
    {
        *psize = cosc_load_int32(buffer);
        if (*psize < 16 || *psize > COSC_SIZE_MAX - 4 || COSC_PAD(*psize))
            return COSC_EPSIZE;
        if (*psize > size - 4)
            return COSC_EOVERRUN;
        buffer = (const char *)buffer + 4;
    }
    if (cosc_memcmp(buffer, "#bundle", 8) != 0)
        return COSC_ETYPE;
    if (timetag)
        *timetag = cosc_load_uint64((const unsigned char *)buffer + 8);
    return req;
}

cosc_int32 cosc_write_signature(
    void *buffer,
    cosc_int32 size,
    const char *address,
    cosc_int32 address_n,
    const char *typetag,
    cosc_int32 typetag_n,
    cosc_int32 psize
)
{
    cosc_int32 req = 0, sz;
    if (psize > 0)
    {
        if (psize > COSC_SIZE_MAX - 4)
            return COSC_ESIZEMAX;
        if (psize < 8 || COSC_PAD(psize))
            return COSC_EPSIZE;
    }
    if (buffer && size < 12)
        return COSC_EOVERRUN;
    if (psize != 0)
        req += 4;
    if (buffer)
        sz = cosc_write_string((unsigned char *)buffer + req, size - req, address, address_n, 0);
    else
        sz = cosc_write_string(0, 0, address, address_n, 0);
    if (sz < 0)
        return sz;
    if (sz > COSC_SIZE_MAX - req)
        return COSC_ESIZEMAX;
    req += sz;
    if (buffer)
        sz = cosc_write_string((unsigned char *)buffer + req, size - req, typetag, typetag_n, 0);
    else
        sz = cosc_write_string(0, 0, typetag, typetag_n, 0);
    if (sz < 0)
        return sz;
    if (sz > COSC_SIZE_MAX - req)
        return COSC_ESIZEMAX;
    req += sz;
    if (psize > 0)
    {
        if (psize < req - 4 || COSC_PAD(psize) || psize > COSC_SIZE_MAX - 4)
            return COSC_EPSIZE;
        cosc_store_int32(buffer, psize);
    }
    else if (psize < 0)
        cosc_store_int32(buffer, req - 4);
    return req;
}

cosc_int32 cosc_read_signature(
    const void *buffer,
    cosc_int32 size,
    const char **address,
    cosc_int32 *address_n,
    const char **typetag,
    cosc_int32 *typetag_n,
    cosc_int32 *psize
)
{
    cosc_int32 req = 0, sz;
    if (size < 8)
        return COSC_EOVERRUN;
    if (psize)
    {
        *psize = cosc_load_int32(buffer);
        if (*psize < 8 || *psize > COSC_SIZE_MAX - 8 || COSC_PAD(*psize))
            return COSC_EPSIZE;
        if (*psize > size - 4)
            return COSC_EOVERRUN;
        req += 4;
    }
    if (address)
        *address = (const char *)buffer + req;
    sz = cosc_read_string((const char *)buffer + req, size - req, 0, 0, address_n);
    if (sz < 0)
        return sz;
    req += sz;
    if (typetag)
        *typetag = (const char *)buffer + req;
    sz = cosc_read_string((const char *)buffer + req, size - req, 0, 0, typetag_n);
    if (sz < 0)
        return sz;
    req += sz;
    return req;
}

cosc_int32 cosc_write_value(
    void *buffer,
    cosc_int32 size,
    char type,
    const union cosc_value *value
)
{
#if defined(COSC_NOINT64) || defined(COSC_NOFLOAT64)
    static const struct cosc_64bits zero64 = COSC_64BITS_INIT(0, 0);
#endif
    if (buffer)
    {
        switch (type)
        {
        case 'i': return cosc_write_int32(buffer, size, value ? value->i : 0);
        case 'r': return cosc_write_uint32(buffer, size, value ? value->r : 0);
        case 'f': return cosc_write_float32(buffer, size, value ? value->f : 0);
#ifdef COSC_NOINT64
        case 'h': return cosc_write_int64(buffer, size, value ? value->h : zero64);
        case 't': return cosc_write_uint64(buffer, size, value ? value->t : zero64);
#else
        case 'h': return cosc_write_int64(buffer, size, value ? value->h : 0);
        case 't': return cosc_write_uint64(buffer, size, value ? value->t : 0);
#endif
#ifdef COSC_NOFLOAT64
        case 'd': return cosc_write_float64(buffer, size, value ? value->d : zero64);
#else
        case 'd': return cosc_write_float64(buffer, size, value ? value->d : 0);
#endif
        case 'c': return cosc_write_char(buffer, size, value ? value->c : 0);
        case 'm': return cosc_write_midi(buffer, size, value ? value->m : 0);
        case 's':
        case 'S': return cosc_write_string(buffer, size, value ? value->s.s : 0, value ? value->s.length : 0, 0);
        case 'b': return cosc_write_blob(buffer, size, value ? value->b.b : 0, value ? value->b.size : 0);
        case 'T':
        case 'F':
        case 'N':
        case 'I': return 0;
        }
    }
    else
    {
        switch (type)
        {
        case 'i':
        case 'r':
        case 'f':
        case 'c':
        case 'm': return 4;
        case 'h':
        case 't':
        case 'd': return 8;
        case 's':
        case 'S': return cosc_write_string(0, 0, value ? value->s.s : 0, value ? value->s.length : 0, 0);
        case 'b': return  cosc_write_blob(0, 0, value ? value->b.b : 0, value ? value->b.size : 0);
        case 'T':
        case 'F':
        case 'N':
        case 'I': return 0;
        }
    }
    return COSC_ETYPE;
}

cosc_int32 cosc_read_value(
    const void *buffer,
    cosc_int32 size,
    char type,
    union cosc_value *value
)
{
    switch (type)
    {
    case 'i': return cosc_read_int32(buffer, size, value ? &value->i : 0);
    case 'r': return cosc_read_uint32(buffer, size, value ? &value->r : 0);
    case 'f': return cosc_read_float32(buffer, size, value ? &value->f : 0);
    case 'h': return cosc_read_int64(buffer, size, value ? &value->h : 0);
    case 't': return cosc_read_uint64(buffer, size, value ? &value->t : 0);
    case 'd': return cosc_read_float64(buffer, size, value ? &value->d : 0);
    case 'c': return cosc_read_char(buffer, size, value ? &value->c : 0);
    case 'm': return cosc_read_midi(buffer, size, value ? value->m : 0);
    case 's':
    case 'S':
        if (value)
        {
            value->s.s = (const char *)buffer;
            return cosc_read_string(buffer, size, 0, 0, &value->s.length);
        }
        return cosc_read_string(buffer, size, 0, 0, 0);
    case 'b':
        if (value)
            return cosc_read_blob(buffer, size, 0, 0, &value->b.b, &value->b.size);
        return cosc_read_blob(buffer, size, 0, 0, 0, 0);
    case 'T':
    case 'F':
    case 'N':
    case 'I':
        if (value) cosc_memset(value, 0, sizeof(*value));
        return 0;
    }
    return COSC_ETYPE;
}

cosc_int32 cosc_write_values(
    void *buffer,
    cosc_int32 size,
    const char *types,
    cosc_int32 types_n,
    const union cosc_value *values,
    cosc_int32 values_n,
    cosc_int32 *value_count
)
{
    cosc_int32 tlen = 0, vlen = 0, req = 0;
#ifndef COSC_NOARRAY
    cosc_int32 array_start = 0;
#endif
    if (types_n <= 0 || *types == 0)
    {
        if (value_count) *value_count = 0;
        return 0;
    }
    if (*types == ',')
        tlen++;
    cosc_int32 payload = 0;
    while (tlen < types_n && types[tlen] != 0)
    {
        cosc_int32 sz;
#ifndef COSC_NOARRAY
        if (types[tlen] == '[')
        {
            if (array_start)
            {
                if (value_count) *value_count = vlen;
                return COSC_ETYPE;
            }
            tlen++;
            array_start = tlen;
            payload = 0;
            continue;
        }
        if (types[tlen] == ']')
        {
            if (!array_start)
            {
                if (value_count) *value_count = vlen;
                return COSC_ETYPE;
            }
            if (vlen >= values_n || payload == 0)
                break;
            tlen = array_start;
            continue;
        }
#endif
        if (values && vlen < values_n)
            sz = cosc_write_value(buffer, size - req, types[tlen], values + vlen);
        else
            sz = cosc_write_value(buffer, size - req, types[tlen], 0);
        if (sz < 0)
        {
            if (value_count) *value_count = vlen;
            return sz;
        }
        if (sz > COSC_SIZE_MAX - req)
        {
            if (value_count) *value_count = vlen;
            return COSC_EOVERRUN;
        }
        req += sz;
        if (buffer)
            buffer = (char *)buffer + sz;
        tlen++;
        if (sz > 0)
        {
            payload++;
            vlen++;
        }
    }
    if (value_count) *value_count = vlen;
#ifndef COSC_NOARRAY
    if (array_start > 0 && (tlen >= types_n || types[tlen] != ']'))
        return COSC_ETYPE;
#endif
    return req;
}

cosc_int32 cosc_read_values(
    const void *buffer,
    cosc_int32 size,
    const char *types,
    cosc_int32 types_n,
    union cosc_value *values,
    cosc_int32 values_n,
    cosc_int32 *value_count,
    cosc_int32 exit_early
)
{
    cosc_int32 tlen = 0, vlen = 0, req = 0;
#ifndef COSC_NOARRAY
    cosc_int32 array_start = 0;
#endif
    if (types_n <= 0 || *types == 0)
    {
        if (value_count) *value_count = 0;
        return 0;
    }
    if (*types == ',')
        tlen++;
    cosc_int32 payload = 0;
    while (tlen < types_n && types[tlen] != 0)
    {
        cosc_int32 sz;
#ifndef COSC_NOARRAY
        if (types[tlen] == '[')
        {
            if (array_start)
            {
                if (value_count) *value_count = vlen;
                return COSC_ETYPE;
            }
            tlen++;
            array_start = tlen;
            payload = 0;
            continue;
        }
        if (types[tlen] == ']')
        {
            if (!array_start)
            {
                if (value_count) *value_count = vlen;
                return COSC_ETYPE;
            }
            if ((vlen >= values_n && exit_early) || req >= size || payload == 0)
                break;
            tlen = array_start;
            continue;
        }
#endif
        if (values && vlen < values_n)
            sz = cosc_read_value(buffer, size - req, types[tlen], values + vlen);
        else
            sz = cosc_read_value(buffer, size - req, types[tlen], 0);
        if (sz < 0)
        {
            if (value_count) *value_count = vlen;
            return sz;
        }
        if (sz > COSC_SIZE_MAX - req)
        {
            if (value_count) *value_count = vlen;
            return COSC_EOVERRUN;
        }
        req += sz;
        if (buffer)
            buffer = (const char *)buffer + sz;
        tlen++;
        if (sz > 0)
        {
            payload++;
            vlen++;
        }
    }
    if (value_count) *value_count = vlen;
#ifndef COSC_NOARRAY
    if (array_start && (tlen >= types_n || types[tlen] != ']'))
        return COSC_ETYPE;
#endif
    return req;
}

cosc_int32 cosc_write_message(
    void *buffer,
    cosc_int32 size,
    const struct cosc_message *message,
    cosc_int32 psize,
    cosc_int32 *value_count
)
{
    cosc_int32 req = 0, sz;
    struct cosc_message tmp_message = {0, 0, 0, 0, {0}, 0};

    if (!message)
        message = &tmp_message;
    sz = cosc_write_signature(
        buffer ? buffer : 0, buffer ? size - req : 0,
        message->address, message->address_n,
        message->typetag, message->typetag_n,
        psize
    );
    if (sz < 0)
    {
        if (value_count) *value_count = 0;
        return sz;
    }
    if (sz > COSC_SIZE_MAX - req)
    {
        if (value_count) *value_count = 0;
        return COSC_SIZE_MAX;
    }
    req += sz;
    sz = cosc_write_values(
        (unsigned char *)buffer + req, size - req,
        message->typetag, message->typetag_n,
        message->values.write, message->values_n,
        value_count
    );
    if (sz < 0)
        return sz;
    if (sz > COSC_SIZE_MAX - req)
        return COSC_SIZE_MAX;
    req += sz;
    if (psize > 0)
    {
        if (psize < req - 4 || COSC_PAD(psize) || psize > COSC_SIZE_MAX - 4)
            return COSC_EPSIZE;
        cosc_write_int32(buffer, 4, psize);
    }
    else if (psize < 0)
        cosc_write_int32(buffer, 4, req - 4);
    return req;
}

cosc_int32 cosc_read_message(
    const void *buffer,
    cosc_int32 size,
    struct cosc_message *message,
    cosc_int32 *psize,
    cosc_int32 *value_count,
    cosc_int32 exit_early
)
{
    cosc_int32 req = 0, sz;
    struct cosc_message tmp_message;
    if (!message)
        message = &tmp_message;
    sz = cosc_read_signature(
        buffer, size - req,
        &message->address, &message->address_n,
        &message->typetag, &message->typetag_n,
        psize
    );
    if (psize)
    {
        if (*psize < 8 || COSC_PAD(*psize) || *psize > COSC_SIZE_MAX - 4)
            return COSC_EPSIZE;
    }
    if (sz < 0)
    {
        if (value_count) *value_count = 0;
        return sz;
    }
    if (sz > COSC_SIZE_MAX - req)
    {
        if (value_count) *value_count = 0;
        return COSC_SIZE_MAX;
    }
    req += sz;
    buffer = (char *)buffer + sz;
    sz = cosc_read_values(
        buffer, size - req,
        message->typetag, message->typetag_n,
        message->values.read, message->values_n,
        value_count, exit_early
    );
    if (sz < 0)
    {
        if (value_count) *value_count = 0;
        return sz;
    }
    if (sz > COSC_SIZE_MAX - req)
    {
        if (value_count) *value_count = 0;
        return COSC_SIZE_MAX;
    }
    req += sz;
    return req;
}

#if !defined(COSC_NOSTDLIB) && !defined(COSC_NODUMP)

#ifdef __cplusplus
#include <cstdio>
#include <cinttypes>
#else
#include <stdio.h>
#include <inttypes.h>
#endif

cosc_int32 cosc_value_dump(
    char *s,
    cosc_int32 n,
    char type,
    const union cosc_value *value
)
{
    if (!s || n <= 0)
    {
        s = 0;
        n = 0;
    }
    if (!value)
        return snprintf(s, n, "NULL");
    switch (type)
    {
    case 'i': return snprintf(s, n, "%d", value->i);
#ifdef COSC_NOFLOAT32
    case 'f': return snprintf(s, n, "0x%08x", value->f);
#else
    case 'f': return snprintf(s, n, "%f", value->f);
#endif
    case 'S':
    case 's': return snprintf(s, n, "\"%s\"", value->s.s ? value->s.s : "");
#ifdef COSC_NOINT64
    case 'h': return snprintf(s, n, "0x%08x %08x", COSC_64BITS_HI(&value->h), COSC_64BITS_LO(&value->h));
    case 't': return snprintf(s, n, "0x%08x %08x", COSC_64BITS_HI(&value->t), COSC_64BITS_LO(&value->t));
#else
    case 'h': return snprintf(s, n, "%" PRId64, value->h);
    case 't': return snprintf(s, n, "%" PRIu64, value->t);
#endif
#ifdef COSC_NOFLOAT64
    case 'd': return snprintf(s, n, "0x%08x %08x" PRIx64, COSC_64BITS_LO(&value->d), COSC_64BITS_HI(&value->d));
#else
    case 'd': return snprintf(s, n, "%f", value->d);
#endif
    case 'c':
        if (value->c >= 32)
            return snprintf(s, n, "'%c'", value->c);
        return snprintf(s, n, "'\\%02x'", (unsigned char)value->c);
    case 'r': return snprintf(s, n, "%u", value->r);
    case 'm': return snprintf(s, n, "{0x%02x 0x%02x 0x%02x 0x%02x}", value->m[0], value->m[1], value->m[2], value->m[3]);
    case 'T': return snprintf(s, n, "true");
    case 'F': return snprintf(s, n, "false");
    case 'N': return snprintf(s, n, "nil");
    case 'I': return snprintf(s, n, "inf");
    }

    if (type == 'b')
    {
        cosc_int32 len = snprintf(s, n, "(%d){", value->b.size);
        if (value->b.size > 0)
            len += snprintf(len < n ? s + len : 0, len < n ? n - len : 0, "%02x", ((unsigned char *)value->b.b)[0]);
        for (int32_t i = 1; i < value->b.size; i++)
            len += snprintf(len < n ? s + len : 0, len < n ? n - len : 0, " %02x", ((unsigned char *)value->b.b)[i]);
        len += snprintf(len < n ? s + len : 0, len < n ? n - len : 0, "}");
        return len;
    }

    if (s && n > 0)
        s[0] = 0;
    return 0;
}

cosc_int32 cosc_message_dump(
    char *s,
    cosc_int32 n,
    const struct cosc_message *message
)
{
    if (!s || n <= 0)
    {
        s = 0;
        n = 0;
    }
    if (!message)
        return snprintf(s, n, "NULL");
    cosc_int32 value_count = cosc_typetag_payload(0, 0, message->typetag, message->typetag_n, 0);
    if (value_count > message->values_n)
        value_count = message->values_n;
    cosc_int32 len = 0;
    len += snprintf(
        len < n ? s + len : 0, len < n ? n - len : 0, "<\"%s\" \"%s\" (%d)[",
        message->address ? message->address : "",
        message->typetag ? message->typetag : "",
        value_count
    );
    cosc_int32 tindex = 0;
    for (cosc_int32 i = 0; i < value_count; i++)
    {
        while (tindex < message->typetag_n && !cosc_type_is_payload(message->typetag[tindex]))
            tindex++;
        if (tindex >= message->typetag_n)
            break;
        if (i > 0)
            len += snprintf(len < n ? s + len : 0, len < n ? n - len : 0, ", ");
        len += cosc_value_dump(len < n ? s + len : 0, len < n ? n - len : 0, message->typetag[tindex], message->values.read + i);
        tindex++;
    }
    len += snprintf(len < n ? s + len : 0, len < n ? n - len : 0, "]>");
    return len;
}

#endif /* !COSC_NODUMP && !COSC_NOSTDLIB */

#if !defined(COSC_NOWRITER) || !defined(COSC_NOREADER)

#define COSC_SERIAL_DOPSIZE(serial_) ((serial_)->level >= 0 || ((serial_)->flags & COSC_SERIAL_PSIZE))

static void cosc_serial_setup(
    struct cosc_serial *serial,
    void *wbuffer,
    const void *rbuffer,
    cosc_int32 buffer_size,
    struct cosc_level *levels,
    cosc_int32 level_max,
    cosc_uint32 flags
)
{
    if (wbuffer)
    {
        serial->wbuffer = (unsigned char *)wbuffer;
        serial->buffer_size = wbuffer ? (buffer_size < COSC_SIZE_MAX ? buffer_size : COSC_SIZE_MAX) : 0;
        serial->rbuffer = 0;
    }
    else
    {
        serial->rbuffer = (const unsigned char *)rbuffer;
        serial->buffer_size = rbuffer ? (buffer_size < COSC_SIZE_MAX ? buffer_size : COSC_SIZE_MAX) : 0;
        serial->wbuffer = 0;
    }
    serial->levels = levels;
    serial->level_max = levels ? level_max : 0;
    serial->level = -1;
    serial->size = 0;
    serial->flags = flags;
}

static cosc_int32 cosc_serial_next_msgtype(
    struct cosc_serial *serial
)
{
    if (serial->level < 0)
        return 0;
    if (serial->levels[serial->level].type != COSC_LEVEL_TYPE_MESSAGE)
        return 0;
    const unsigned char *buffer = COSC_SERIAL_ISREADER(serial) ? serial->rbuffer : serial->wbuffer;
    cosc_int32 offset = serial->levels[serial->level].ttstart;
    offset += serial->levels[serial->level].ttindex;
    if (!buffer[offset])
        return 0;
    offset++;
    if (offset >= serial->levels[serial->level].ttend)
        return 0;
    serial->levels[serial->level].ttindex = offset - serial->levels[serial->level].ttstart;
    return buffer[offset];
}

static cosc_int32 cosc_serial_get_available(
    const struct cosc_serial *serial
)
{
    if (serial->level < 0)
        return serial->buffer_size - serial->size;
    return serial->levels[serial->level].size_max - serial->levels[serial->level].size;
}

static cosc_int32 cosc_serial_get_offset(
    const struct cosc_serial *serial
)
{
    if (serial->level < 0)
        return serial->size;
    return serial->levels[serial->level].start + serial->levels[serial->level].size;
}

static cosc_int32 cosc_serial_start_level(
    struct cosc_serial *serial,
    cosc_int32 level_type
)
{
    if (serial->level >= serial->level_max - 1)
        return COSC_ELEVELMAX;
    cosc_int32 req_size = 0;
    switch (level_type)
    {
    case COSC_LEVEL_TYPE_BUNDLE:
        if (serial->level >= 0
            && serial->levels[serial->level].type != COSC_LEVEL_TYPE_BLOB)
            return COSC_ELEVELTYPE;
        if (serial->level < 0 && serial->size > 0 && !(serial->flags & COSC_SERIAL_PSIZE))
            return COSC_EPSIZEFLAG;
        req_size = 16;
        break;
    case COSC_LEVEL_TYPE_MESSAGE:
        if (serial->level >= 0
            && serial->levels[serial->level].type != COSC_LEVEL_TYPE_BUNDLE
            && serial->levels[serial->level].type != COSC_LEVEL_TYPE_BLOB)
            return COSC_ELEVELTYPE;
        if (serial->level < 0 && serial->size > 0 && !(serial->flags & COSC_SERIAL_PSIZE))
            return COSC_EPSIZEFLAG;
        req_size = 8;
        break;
    case COSC_LEVEL_TYPE_BLOB:
        if (serial->level < 0)
            return COSC_ELEVELTYPE;
        if (serial->levels[serial->level].type == COSC_LEVEL_TYPE_MESSAGE)
        {
            if (cosc_serial_get_msgtype(serial) != 'b')
                return COSC_EMSGTYPE;
        }
        else if (serial->levels[serial->level].type != COSC_LEVEL_TYPE_BLOB)
            return COSC_ELEVELTYPE;
        break;
    default:
        return COSC_ELEVELTYPE;
    }
    if (serial->level >= 0 || (serial->flags & COSC_SERIAL_PSIZE)
        || level_type == COSC_LEVEL_TYPE_BLOB)
        req_size += 4;

    struct cosc_level *level = serial->levels + serial->level + 1;
    if (serial->level >= 0)
    {
        level->start = serial->levels[serial->level].start;
        level->start += serial->levels[serial->level].size;
        level->size_max = serial->levels[serial->level].size_max - level->start;
    }
    else
    {
        level->start = serial->size;
        level->size_max = serial->buffer_size - serial->size;
    }
    if (req_size > level->size_max)
        return COSC_EOVERRUN;
    level->type = level_type;
    level->size = 0;
    level->ttstart = 0;
    level->ttindex = 0;
    level->ttend = 0;
    serial->level++;
    return serial->level;
}

static void cosc_serial_end_level(
    struct cosc_serial *serial
)
{
    if (serial->level > 0)
        serial->levels[serial->level - 1].size += serial->levels[serial->level].size;
    else
        serial->size += serial->levels[serial->level].size;
    serial->level--;
}

static cosc_int32 cosc_serial_accepts(
    const struct cosc_serial *serial,
    cosc_int32 type
)
{
    if (serial->level < 0)
        return COSC_ELEVELTYPE;
    if (serial->levels[serial->level].type == COSC_LEVEL_TYPE_MESSAGE)
    {
        if (type == 'S')
            type = 's';
        cosc_int32 t = cosc_serial_get_msgtype(serial);
        if (t == 'S')
            t = 's';
        if (t != type)
            return COSC_EMSGTYPE;
    }
    else if (serial->levels[serial->level].type != COSC_LEVEL_TYPE_BLOB)
        return COSC_ELEVELTYPE;
    return serial->levels[serial->level].start + serial->levels[serial->level].size;
}

static cosc_int32 cosc_serial_start_scalar(
    struct cosc_serial *serial,
    cosc_int32 type,
    cosc_int32 size
)
{
    cosc_int32 offset = cosc_serial_accepts(serial, type);
    if (offset < 0)
        return offset;
    if (size > cosc_serial_get_available(serial))
        return COSC_EOVERRUN;
    return offset;
}

static cosc_int32 cosc_serial_end_scalar(
    struct cosc_serial *serial,
    cosc_int32 size
)
{
    serial->levels[serial->level].size += size;
    cosc_serial_next_msgtype(serial);
    return size;
}

static cosc_int32 cosc_serial_repeat(
    struct cosc_serial *serial
)
{
#ifndef COSC_NOARRAY
    cosc_int32 type = cosc_serial_get_msgtype(serial);
    if (type <= 0)
        return type;
    if (type != ']')
        return COSC_EMSGTYPE;
    const char *buffer = (const char *)(COSC_SERIAL_ISREADER(serial) ? serial->rbuffer : serial->wbuffer);
    cosc_int32 offset = serial->levels[serial->level].ttstart;
    offset += serial->levels[serial->level].ttindex;
    while (offset > serial->levels[serial->level].ttstart + 1 && buffer[offset] != '[')
        offset--;
    if (buffer[offset] != '[')
        return COSC_EMSGTYPE;
    serial->levels[serial->level].ttindex = offset - serial->levels[serial->level].ttstart;
#endif
    return 0;
}

cosc_int32 cosc_serial_get_buffer_size(
    const struct cosc_serial *serial
)
{
    return serial->buffer_size;
}

cosc_int32 cosc_serial_get_size(
    const struct cosc_serial *serial
)
{
    if (serial->level < 0)
        return serial->size;
    return serial->levels[serial->level].start + serial->levels[serial->level].size;
}

cosc_int32 cosc_serial_get_msgtype(
    const struct cosc_serial *serial
)
{
    if (serial->level < 0
        || serial->levels[serial->level].type != COSC_LEVEL_TYPE_MESSAGE)
        return COSC_ELEVELTYPE;
    cosc_int32 tt = serial->levels[serial->level].ttstart;
    tt += serial->levels[serial->level].ttindex;
    if (tt >= serial->levels[serial->level].ttend)
        return 0;
    if (COSC_SERIAL_ISREADER(serial))
        return serial->rbuffer[tt];
    return serial->wbuffer[tt];
}

void cosc_serial_reset(
    struct cosc_serial *serial
)
{
    serial->size = 0;
    serial->level = -1;
}

#endif /* !COSC_NOWRITER && !COSC_NOREADER */

#ifndef COSC_NOWRITER

void cosc_writer_setup(
    struct cosc_serial *serial,
    void *buffer,
    cosc_int32 buffer_size,
    struct cosc_level *levels,
    cosc_int32 level_max,
    cosc_uint32 flags
)
{
    cosc_serial_setup(serial, buffer, 0, buffer_size, levels, level_max, flags);
}

cosc_int32 cosc_writer_start_bundle(
    struct cosc_serial *serial,
    cosc_uint64 timetag
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 use_psize = COSC_SERIAL_DOPSIZE(serial);
    if (!use_psize && serial->size > 0)
        return COSC_EPSIZEFLAG;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 offset = cosc_serial_get_offset(serial);
    cosc_int32 sz = cosc_write_bundle(serial->wbuffer + offset, available, timetag, use_psize ? -1 : 0);
    if (sz < 0)
        return sz;
    cosc_int32 level = cosc_serial_start_level(serial, COSC_LEVEL_TYPE_BUNDLE);
    if (level < 0)
        return level;
    serial->levels[level].size += sz;
    return sz;
}

cosc_int32 cosc_writer_end_bundle(
    struct cosc_serial *serial
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    if (serial->level < 0 || serial->levels[serial->level].type != COSC_LEVEL_TYPE_BUNDLE)
        return COSC_ELEVELTYPE;
    if (serial->level > 0 || (serial->flags & COSC_SERIAL_PSIZE))
        cosc_store_int32(serial->wbuffer + serial->levels[serial->level].start, serial->levels[serial->level].size - 4);
    cosc_serial_end_level(serial);
    return 0;
}

cosc_int32 cosc_writer_start_message(
    struct cosc_serial *serial,
    const char *address,
    cosc_int32 address_n,
    const char *typetag,
    cosc_int32 typetag_n
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    if (!cosc_typetag_validate(typetag, typetag_n, 0))
        return COSC_ETYPE;
    cosc_int32 use_psize = COSC_SERIAL_DOPSIZE(serial);
    if (!use_psize && serial->size > 0)
        return COSC_EPSIZEFLAG;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 offset = cosc_serial_get_offset(serial);
    cosc_int32 req = 0;
    if (available < 8)
        return COSC_EOVERRUN;
    if (use_psize)
        req += 4;
    cosc_int32 address_size = cosc_write_string(serial->wbuffer + offset + req, available - req, address, address_n, 0);
    if (address_size < 0)
        return address_size;
    req += address_size;
    cosc_int32 typetag_size = cosc_write_string(serial->wbuffer + offset + req, available - req, typetag, typetag_n, 0);
    if (typetag_size < 0)
        return typetag_size;
    req += typetag_size;
    if (use_psize)
        cosc_store_int32(serial->wbuffer + offset, req);
    cosc_int32 level = cosc_serial_start_level(serial, COSC_LEVEL_TYPE_MESSAGE);
    if (level < 0)
        return level;
    serial->levels[level].size += req;
    serial->levels[level].ttstart = serial->levels[level].start + address_size;
    if (use_psize)
        serial->levels[level].ttstart += 4;
    serial->levels[level].ttend = serial->levels[level].ttstart + typetag_size;
    cosc_serial_next_msgtype(serial);
    return req;
}

cosc_int32 cosc_writer_end_message(
    struct cosc_serial *serial
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    if (serial->level < 0 || serial->levels[serial->level].type != COSC_LEVEL_TYPE_MESSAGE)
        return COSC_ELEVELTYPE;
    cosc_int32 add = 0;
    cosc_int32 t;
    while ((t = cosc_serial_get_msgtype(serial)) != 0)
    {
        if (t == '[' || t == ']')
            break;
        cosc_int32 sz = cosc_writer_skip(serial);
        if (sz < 0)
            return sz;
        add += sz;
    }
    if (serial->level > 0 || (serial->flags & COSC_SERIAL_PSIZE))
        cosc_store_int32(serial->wbuffer + serial->levels[serial->level].start, serial->levels[serial->level].size - 4);
    cosc_serial_end_level(serial);
    return add;
}

cosc_int32 cosc_writer_start_blob(
    struct cosc_serial *serial
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 offset = cosc_serial_get_offset(serial);
    cosc_int32 sz = cosc_write_int32(serial->wbuffer + offset, available, 0);
    if (sz < 0)
        return sz;
    cosc_int32 level = cosc_serial_start_level(serial, COSC_LEVEL_TYPE_BLOB);
    if (level < 0)
        return level;
    serial->levels[level].size += sz;
    return sz;
}

cosc_int32 cosc_writer_end_blob(
    struct cosc_serial *serial
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    if (serial->level < 0 || serial->levels[serial->level].type != COSC_LEVEL_TYPE_BLOB)
        return COSC_ELEVELTYPE;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 pad = COSC_PAD(serial->levels[serial->level].size);
    if (serial->levels[serial->level].size > available - pad)
        return COSC_EOVERRUN;
    if (serial->level > 0 || (serial->flags & COSC_SERIAL_PSIZE))
        cosc_store_int32(serial->wbuffer + serial->levels[serial->level].start, serial->levels[serial->level].size - 4);
    cosc_memset(serial->wbuffer + serial->levels[serial->level].start + serial->levels[serial->level].size, 0, pad);
    serial->levels[serial->level].size += pad;
    cosc_serial_end_level(serial);
    cosc_serial_next_msgtype(serial);
    return pad;
}

cosc_int32 cosc_writer_uint32(
    struct cosc_serial *serial,
    cosc_uint32 value
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'r', 4);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_write_uint32(serial->wbuffer + offset, 4, value));
}

cosc_int32 cosc_writer_int32(
    struct cosc_serial *serial,
    cosc_int32 value
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'i', 4);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_write_int32(serial->wbuffer + offset, 4, value));
}

cosc_int32 cosc_writer_float32(
    struct cosc_serial *serial,
    cosc_float32 value
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'f', 4);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_write_float32(serial->wbuffer + offset, 4, value));
}

cosc_int32 cosc_writer_uint64(
    struct cosc_serial *serial,
    cosc_uint64 value
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 't', 8);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_write_uint64(serial->wbuffer + offset, 8, value));
}

cosc_int32 cosc_writer_int64(
    struct cosc_serial *serial,
    cosc_int64 value
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'h', 8);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_write_int64(serial->wbuffer + offset, 8, value));
}

cosc_int32 cosc_writer_float64(
    struct cosc_serial *serial,
    cosc_float64 value
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'd', 8);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_write_float64(serial->wbuffer + offset, 8, value));
}

cosc_int32 cosc_writer_string(
    struct cosc_serial *serial,
    const char *value,
    cosc_int32 value_n,
    cosc_int32 *length
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_accepts(serial, 's');
    if (offset < 0)
        return offset;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 sz = cosc_write_string(serial->wbuffer + offset, available, value, value_n, length);
    if (sz < 0)
        return sz;
    serial->levels[serial->level].size += sz;
    cosc_serial_next_msgtype(serial);
    return sz;
}

cosc_int32 cosc_writer_blob(
    struct cosc_serial *serial,
    const void *value,
    cosc_int32 value_n
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_accepts(serial, 'b');
    if (offset < 0)
        return offset;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 sz = cosc_write_blob(serial->wbuffer + offset, available, value, value_n);
    if (sz < 0)
        return sz;
    serial->levels[serial->level].size += sz;
    cosc_serial_next_msgtype(serial);
    return sz;
}

cosc_int32 cosc_writer_char(
    struct cosc_serial *serial,
    cosc_int32 value
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'c', 4);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_write_int32(serial->wbuffer + offset, 4, value));
}

cosc_int32 cosc_writer_midi(
    struct cosc_serial *serial,
    const unsigned char value[4]
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'm', 4);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_write_midi(serial->wbuffer + offset, 4, value));
}

cosc_int32 cosc_writer_value(
    struct cosc_serial *serial,
    cosc_int32 type,
    const union cosc_value *value
)
{
#if defined(COSC_NOINT64) || defined(COSC_NOFLOAT64)
#ifndef __cplusplus
    static const struct cosc_64bits zero = {0};
#else
    static const struct cosc_64bits zero = {};
#endif
#endif
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    switch (type)
    {
    case 'i': return cosc_writer_int32(serial, value ? value->i : 0);
    case 'f': return cosc_writer_float32(serial, value ? value->f : 0);
    case 's':
    case 'S': return cosc_writer_string(serial, value ? value->s.s : 0, value ? value->s.length : 0, 0);
    case 'b': return cosc_writer_blob(serial, value ? value->b.b : 0, value ? value->b.size : 0);
#ifndef COSC_NOINT64
    case 'h': return cosc_writer_int64(serial, value ? value->h : 0);
    case 't': return cosc_writer_uint64(serial, value ? value->t : 0);
#else
    case 'h': return cosc_writer_int64(serial, value ? value->h : zero);
    case 't': return cosc_writer_uint64(serial, value ? value->t : zero);
#endif
#ifndef COSC_NOFLOAT64
    case 'd': return cosc_writer_float64(serial, value ? value->d : 0);
#else
    case 'd': return cosc_writer_float64(serial, value ? value->d : zero);
#endif
    case 'c': return cosc_writer_char(serial, value ? value->c : 0);
    case 'r': return cosc_writer_uint32(serial, value ? value->r : 0);
    case 'm': return cosc_writer_midi(serial, value ? value->m : 0);
    case 'T':
    case 'F':
    case 'N':
    case 'I':
        cosc_serial_next_msgtype(serial);
        return 0;
    }
    return COSC_ETYPE;
}

cosc_int32 cosc_writer_message(
    struct cosc_serial *serial,
    const struct cosc_message *message,
    cosc_int32 *value_count
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
    {
        if (value_count) *value_count = 0;
        return COSC_EINVAL;
    }
    cosc_int32 use_psize = COSC_SERIAL_DOPSIZE(serial);
    if (serial->level < 0 && serial->size > 0 && !use_psize)
    {
        if (value_count) *value_count = 0;
        return COSC_EPSIZEFLAG;
    }
    if (serial->level >= 0
        && serial->levels[serial->level].type != COSC_LEVEL_TYPE_BUNDLE
        && serial->levels[serial->level].type != COSC_LEVEL_TYPE_BLOB)
    {
        if (value_count) *value_count = 0;
        return COSC_ELEVELTYPE;
    }
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 offset = cosc_serial_get_offset(serial);
    cosc_int32 sz = cosc_write_message(serial->wbuffer + offset, available, message, use_psize ? -1 : 0, value_count);
    if (sz < 0)
        return sz;
    if (serial->level >= 0)
        serial->levels[serial->level].size += sz;
    else
        serial->size += sz;
    return sz;
}

cosc_int32 cosc_writer_bytes(
    struct cosc_serial *serial,
    const void *value,
    cosc_int32 value_n
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    if (serial->level < 0 || serial->levels[serial->level].type != COSC_LEVEL_TYPE_BLOB)
        return COSC_ELEVELTYPE;
    cosc_int32 available = cosc_serial_get_available(serial);
    if (value_n > available)
        return COSC_EOVERRUN;
    else if (value_n < 0)
        value_n = 0;
    cosc_int32 offset = cosc_serial_get_offset(serial);
    if (value)
        cosc_memcpy(serial->wbuffer + offset, value, value_n);
    else
        cosc_memset(serial->wbuffer + offset, 0, value_n);
    serial->levels[serial->level].size += value_n;
    return value_n;
}

cosc_int32 cosc_writer_skip(
    struct cosc_serial *serial
)
{
    if (!COSC_SERIAL_ISWRITER(serial))
        return COSC_EINVAL;
    cosc_int32 type = cosc_serial_get_msgtype(serial);
    if (type <= 0)
        return type;
#ifndef COSC_NOARRAY
    if (type == '[' || type == ']')
    {
        cosc_serial_next_msgtype(serial);
        return 0;
    }
#endif
    return cosc_writer_value(serial, type, 0);
}

cosc_int32 cosc_writer_repeat(
    struct cosc_serial *serial
)
{
    return cosc_serial_repeat(serial);
}

#endif /* !COSC_NOWRITER */

#ifndef COSC_NOREADER

void cosc_reader_setup(
    struct cosc_serial *serial,
    const void *buffer,
    cosc_int32 buffer_size,
    struct cosc_level *levels,
    cosc_int32 level_max,
    cosc_uint32 flags
)
{
    cosc_serial_setup(serial, 0, buffer, buffer_size, levels, level_max, flags);
}

cosc_int32 cosc_reader_peek_bundle(
    struct cosc_serial *serial,
    cosc_uint64 *timetag,
    cosc_int32 *psize
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 offset = cosc_serial_get_offset(serial);
    cosc_int32 use_psize = COSC_SERIAL_DOPSIZE(serial);
    cosc_int32 tmp_psize = 0;
    cosc_int32 sz = cosc_read_bundle(serial->rbuffer + offset, available, timetag, use_psize ? &tmp_psize : 0);
    if (sz < 0)
        return sz;
    if (use_psize && tmp_psize > available - 4)
        return COSC_EPSIZE;
    if (psize)
        *psize = tmp_psize;
    return sz;
}

cosc_int32 cosc_reader_start_bundle(
    struct cosc_serial *serial,
    cosc_uint64 *timetag
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 use_psize = COSC_SERIAL_DOPSIZE(serial);
    if (!use_psize && serial->size > 0)
        return COSC_EPSIZEFLAG;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 offset = cosc_serial_get_offset(serial);
    cosc_int32 psize = 0;
    cosc_int32 sz = cosc_read_bundle(serial->rbuffer + offset, available, timetag, use_psize ? &psize : 0);
    if (sz < 0)
        return sz;
    if (use_psize && psize > available - 4)
        return COSC_EPSIZE;
    cosc_int32 level = cosc_serial_start_level(serial, COSC_LEVEL_TYPE_BUNDLE);
    if (level < 0)
        return level;
    serial->levels[level].size += sz;
    if (use_psize)
        serial->levels[level].size_max = psize + 4;
    return sz;
}

cosc_int32 cosc_reader_end_bundle(
    struct cosc_serial *serial
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    if (serial->level < 0 || serial->levels[serial->level].type != COSC_LEVEL_TYPE_BUNDLE)
        return COSC_ELEVELTYPE;
    cosc_int32 sz = serial->levels[serial->level].size_max - serial->levels[serial->level].size;
    serial->levels[serial->level].size = serial->levels[serial->level].size_max;
    cosc_serial_end_level(serial);
    return sz;
}

cosc_int32 cosc_reader_start_message(
    struct cosc_serial *serial,
    const char **address,
    cosc_int32 *address_n,
    const char **typetag,
    cosc_int32 *typetag_n
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 use_psize = (serial->level >= 0 || (serial->flags & COSC_SERIAL_PSIZE));
    if (!use_psize && serial->size > 0)
        return COSC_EPSIZEFLAG;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 offset = cosc_serial_get_offset(serial);
    cosc_int32 req = 0, psize = 0;
    if (available < 8)
        return COSC_EOVERRUN;
    if (use_psize)
    {
        psize = cosc_load_int32(serial->rbuffer + offset);
        if (psize > available - 4 || psize < 8 || COSC_PAD(psize))
            return COSC_EPSIZE;
        req += 4;
    }
    cosc_int32 address_size = cosc_read_string(serial->rbuffer + offset + req, available - req, 0, 0, address_n);
    if (address_size < 0)
        return address_size;
    if (address)
        *address = (const char *)serial->rbuffer + offset + req;
    req += address_size;
    cosc_int32 typetag_size = cosc_read_string(serial->rbuffer + offset + req, available - req, 0, 0, typetag_n);
    if (typetag_size < 0)
        return typetag_size;
    if (typetag)
        *typetag = (const char *)serial->rbuffer + offset + req;
    req += typetag_size;
    cosc_int32 level = cosc_serial_start_level(serial, COSC_LEVEL_TYPE_MESSAGE);
    if (level < 0)
        return level;
    serial->levels[level].size += req;
    serial->levels[level].ttstart = serial->levels[level].start + address_size;
    if (use_psize)
    {
        serial->levels[level].size_max = psize + 4;
        serial->levels[level].ttstart += 4;
    }
    serial->levels[level].ttend = serial->levels[level].ttstart + typetag_size;
    cosc_serial_next_msgtype(serial);
    return req;
}

cosc_int32 cosc_reader_end_message(
    struct cosc_serial *serial,
    cosc_int32 exit_early
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    if (serial->level < 0 || serial->levels[serial->level].type != COSC_LEVEL_TYPE_MESSAGE)
        return COSC_ELEVELTYPE;
    cosc_int32 add = 0;
    cosc_int32 t, counter = 0;
    while (serial->levels[serial->level].size < serial->levels[serial->level].size_max
           && (t = cosc_serial_get_msgtype(serial)) != 0)
    {
        if (!exit_early)
        {
            if (t == ']')
            {
                cosc_reader_repeat(serial);
                counter++;
            }
        }
        else if (t == '[' || t == ']')
            break;
        cosc_int32 sz = cosc_reader_skip(serial);
        if (sz < 0)
            return sz;
        add += sz;
    }
    cosc_serial_end_level(serial);
    return add;
}

cosc_int32 cosc_reader_start_blob(
    struct cosc_serial *serial,
    cosc_int32 *value_size
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 offset = cosc_serial_get_offset(serial);
    cosc_int32 psize;
    cosc_int32 sz = cosc_read_int32(serial->rbuffer + offset, available, &psize);
    if (sz < 0)
        return sz;
    cosc_int32 pad = COSC_PAD(psize);
    if (psize > available - pad)
        return COSC_EOVERRUN;
    cosc_int32 level = cosc_serial_start_level(serial, COSC_LEVEL_TYPE_BLOB);
    if (level < 0)
        return level;
    if (value_size)
        *value_size = psize;
    serial->levels[level].size += sz;
    serial->levels[level].size_max = psize + pad + 4;
    return sz;
}

cosc_int32 cosc_reader_end_blob(
    struct cosc_serial *serial
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    if (serial->level < 0 || serial->levels[serial->level].type != COSC_LEVEL_TYPE_BLOB)
        return COSC_ELEVELTYPE;
    cosc_int32 pad = COSC_PAD(serial->levels[serial->level].size_max);
    if (serial->levels[serial->level].size_max > COSC_SIZE_MAX - pad)
        return COSC_ESIZEMAX;
    serial->levels[serial->level].size_max += pad;
    cosc_int32 sz = serial->levels[serial->level].size_max - serial->levels[serial->level].size;
    serial->levels[serial->level].size = serial->levels[serial->level].size_max;
    cosc_serial_end_level(serial);
    cosc_serial_next_msgtype(serial);
    return sz;
}

cosc_int32 cosc_reader_uint32(
    struct cosc_serial *serial,
    cosc_uint32 *value
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'r', 4);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_read_uint32(serial->rbuffer + offset, 4, value));
}

cosc_int32 cosc_reader_int32(
    struct cosc_serial *serial,
    cosc_int32 *value
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'i', 4);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_read_int32(serial->rbuffer + offset, 4, value));
}

cosc_int32 cosc_reader_float32(
    struct cosc_serial *serial,
    cosc_float32 *value
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'f', 4);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_read_float32(serial->rbuffer + offset, 4, value));
}

cosc_int32 cosc_reader_uint64(
    struct cosc_serial *serial,
    cosc_uint64 *value
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 't', 8);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_read_uint64(serial->rbuffer + offset, 8, value));
}

cosc_int32 cosc_reader_int64(
    struct cosc_serial *serial,
    cosc_int64 *value
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'h', 8);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_read_int64(serial->rbuffer + offset, 8, value));
}

cosc_int32 cosc_reader_float64(
    struct cosc_serial *serial,
    cosc_float64 *value
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'd', 8);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_read_float64(serial->rbuffer + offset, 8, value));
}

cosc_int32 cosc_reader_string(
    struct cosc_serial *serial,
    char *value,
    cosc_int32 value_n,
    cosc_int32 *length
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_accepts(serial, 's');
    if (offset < 0)
        return offset;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 sz = cosc_read_string(serial->rbuffer + offset, available, value, value_n, length);
    if (sz < 0)
        return sz;
    serial->levels[serial->level].size += sz;
    cosc_serial_next_msgtype(serial);
    return sz;
}

cosc_int32 cosc_reader_blob(
    struct cosc_serial *serial,
    void *value,
    cosc_int32 value_n,
    const void **data,
    cosc_int32 *data_size
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_accepts(serial, 'b');
    if (offset < 0)
        return offset;
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 sz = cosc_read_blob(serial->rbuffer + offset, available, value, value_n, data, data_size);
    if (sz < 0)
        return sz;
    serial->levels[serial->level].size += sz;
    cosc_serial_next_msgtype(serial);
    return sz;
}

cosc_int32 cosc_reader_char(
    struct cosc_serial *serial,
    cosc_int32 *value
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'c', 4);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_read_char(serial->rbuffer + offset, 4, value));
}

cosc_int32 cosc_reader_midi(
    struct cosc_serial *serial,
    unsigned char value[4]
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 offset = cosc_serial_start_scalar(serial, 'm', 4);
    if (offset < 0)
        return offset;
    return cosc_serial_end_scalar(serial, cosc_read_midi(serial->rbuffer + offset, 4, value));
}

cosc_int32 cosc_reader_value(
    struct cosc_serial *serial,
    cosc_int32 type,
    union cosc_value *value
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    union cosc_value discard;
    if (!value)
        value = &discard;
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    switch (type)
    {
    case 'i': return cosc_reader_int32(serial, &value->i);
    case 'f': return cosc_reader_float32(serial, &value->f);
    case 's':
    case 'S':
        value->s.s = (const char *)serial->rbuffer + cosc_serial_get_offset(serial);
        return cosc_reader_string(serial, 0, 0, &value->s.length);
    case 'b': return cosc_reader_blob(serial, 0, 0, value ? &value->b.b : 0, value ? &value->b.size : 0);
    case 'h': return cosc_reader_int64(serial, &value->h);
    case 't': return cosc_reader_uint64(serial, &value->t);
    case 'd': return cosc_reader_float64(serial, &value->d);
    case 'c': return cosc_reader_char(serial, &value->c);
    case 'r': return cosc_reader_uint32(serial, &value->r);
    case 'm': return cosc_reader_midi(serial, value->m);
    case '[':
    case 'T':
    case 'F':
    case 'N':
    case 'I':
        cosc_serial_next_msgtype(serial);
        return 0;
    }
    return COSC_ETYPE;
}

cosc_int32 cosc_reader_message(
    struct cosc_serial *serial,
    struct cosc_message *message,
    cosc_int32 *value_count,
    cosc_int32 exit_early
)
{
    if (!COSC_SERIAL_ISREADER(serial))
    {
        if (value_count) *value_count = 0;
        return COSC_EINVAL;
    }
    if (serial->level >= 0
        && serial->levels[serial->level].type != COSC_LEVEL_TYPE_BUNDLE
        && serial->levels[serial->level].type != COSC_LEVEL_TYPE_BLOB)
    {
        if (value_count) *value_count = 0;
        return COSC_ELEVELTYPE;
    }
    cosc_int32 use_psize = COSC_SERIAL_DOPSIZE(serial);
    if (serial->level < 0 && serial->size > 0 && !use_psize)
    {
        if (value_count) *value_count = 0;
        return COSC_EPSIZEFLAG;
    }
    cosc_int32 available = cosc_serial_get_available(serial);
    cosc_int32 offset = cosc_serial_get_offset(serial);
    cosc_int32 tmp_psize = 0;
    cosc_int32 sz = cosc_read_message(serial->rbuffer + offset, available, message, use_psize ? &tmp_psize : 0, value_count, exit_early);
    if (sz < 0)
        return sz;
    if (use_psize && tmp_psize > available - 4)
        return COSC_EPSIZE;
    if (serial->level >= 0)
        serial->levels[serial->level].size += sz;
    else
        serial->size += sz;
    return sz;
}

cosc_int32 cosc_reader_bytes(
    struct cosc_serial *serial,
    void *value,
    cosc_int32 value_n
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    if (serial->level < 0 || serial->levels[serial->level].type != COSC_LEVEL_TYPE_BLOB)
        return COSC_ELEVELTYPE;
    cosc_int32 available = cosc_serial_get_available(serial);
    if (value_n > available)
        return COSC_EOVERRUN;
    else if (value_n < 0)
        value_n = 0;
    cosc_int32 offset = cosc_serial_get_offset(serial);
    if (value)
        cosc_memcpy(value, serial->rbuffer + offset, value_n);
    serial->levels[serial->level].size += value_n;
    return value_n;
}

cosc_int32 cosc_reader_skip(
    struct cosc_serial *serial
)
{
    if (!COSC_SERIAL_ISREADER(serial))
        return COSC_EINVAL;
    cosc_int32 type = cosc_serial_get_msgtype(serial);
    if (type <= 0)
        return type;
#ifndef COSC_NOARRAY
    if (type == '[' || type == ']')
    {
        cosc_serial_next_msgtype(serial);
        return 0;
    }
#endif
    return cosc_reader_value(serial, type, 0);
}

cosc_int32 cosc_reader_repeat(
    struct cosc_serial *serial
)
{
    return cosc_serial_repeat(serial);
}

#endif /* !COSC_NOREADER */
