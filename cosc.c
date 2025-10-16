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
    cosc_int32 req = psize >= 0 ? 20 : 16;
    if (psize > COSC_SIZE_MAX - 16)
        return COSC_ESIZEMAX;
    if (buffer)
    {
        if (size < req)
            return COSC_EOVERRUN;
        if (psize >= 0)
        {
            cosc_store_int32(buffer, psize);
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
        if (*psize < 16 || *psize > COSC_SIZE_MAX - 16 || COSC_PAD(*psize))
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
    if (psize >= 0)
    {
        if (psize > COSC_SIZE_MAX - 12)
            return COSC_ESIZEMAX;
        if (COSC_PAD(psize) != 0)
            return COSC_EPSIZE;
    }
    if (buffer)
    {
        if (size < 12)
            return COSC_EOVERRUN;
        if (psize >= 0)
        {
            cosc_store_int32(buffer, psize);
            req += 4;
        }
        sz = cosc_write_string((unsigned char *)buffer + req, size - req, address, address_n, 0);
        if (sz < 0)
            return sz;
        if (sz > COSC_SIZE_MAX - req)
            return COSC_ESIZEMAX;
        req += sz;
        sz = cosc_write_string((unsigned char *)buffer + req, size - req, typetag, typetag_n, 0);
        if (sz < 0)
            return sz;
        if (sz > COSC_SIZE_MAX - req)
            return COSC_ESIZEMAX;
        req += sz;
        if (psize >= 0)
            cosc_store_int32(buffer, psize);
    }
    else
    {
        if (psize >= 0)
            req += 4;
        sz = cosc_write_string(0, 0, address, address_n, 0);
        if (sz < 0)
            return sz;
        if (sz > COSC_SIZE_MAX - req)
            return COSC_ESIZEMAX;
        req += sz;
        sz = cosc_write_string(0, 0, typetag, typetag_n, 0);
        if (sz < 0)
            return sz;
        if (sz > COSC_SIZE_MAX - req)
            return COSC_ESIZEMAX;
        req += sz;
    }
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
    const char *typetag,
    cosc_int32 typetag_n,
    const union cosc_value *values,
    cosc_int32 values_n,
    cosc_int32 *value_count
)
{
    cosc_int32 tlen = 0, vlen = 0, req = 0;
#ifndef COSC_NOARRAY
    cosc_int32 array_start = 0;
#endif
    if (typetag_n <= 0 || *typetag != ',')
    {
        if (value_count) *value_count = vlen;
        return COSC_ETYPE;
    }
    tlen++;
    while (tlen < typetag_n && typetag[tlen] != 0)
    {
        cosc_int32 sz;
#ifndef COSC_NOARRAY
        if (typetag[tlen] == '[')
        {
            if (array_start)
            {
                if (value_count) *value_count = vlen;
                return COSC_ETYPE;
            }
            tlen++;
            array_start = tlen;
            continue;
        }
        if (typetag[tlen] == ']')
        {
            if (!array_start)
            {
                if (value_count) *value_count = vlen;
                return COSC_ETYPE;
            }
            if (vlen >= values_n)
                break;
            tlen = array_start;
            continue;
        }
#endif
        if (values && vlen < values_n)
            sz = cosc_write_value(buffer, size - req, typetag[tlen], values + vlen);
        else
            sz = cosc_write_value(buffer, size - req, typetag[tlen], 0);
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
            vlen++;
    }
    if (value_count) *value_count = vlen;
#ifndef COSC_NOARRAY
    if (array_start > 0 && (tlen >= typetag_n || typetag[tlen] != ']'))
        return COSC_ETYPE;
#endif
    return req;
}

cosc_int32 cosc_read_values(
    const void *buffer,
    cosc_int32 size,
    const char *typetag,
    cosc_int32 typetag_n,
    union cosc_value *values,
    cosc_int32 values_n,
    cosc_int32 *value_count
)
{
    cosc_int32 tlen = 0, vlen = 0, req = 0;
#ifndef COSC_NOARRAY
    cosc_int32 array_start = 0;
#endif
    if (typetag_n <= 0 || *typetag != ',')
    {
        if (value_count) *value_count = vlen;
        return COSC_ETYPE;
    }
    tlen++;
    while (tlen < typetag_n && typetag[tlen] != 0)
    {
        cosc_int32 sz;
#ifndef COSC_NOARRAY
        if (typetag[tlen] == '[')
        {
            if (array_start)
            {
                if (value_count) *value_count = vlen;
                return COSC_ETYPE;
            }
            tlen++;
            array_start = tlen;
            continue;
        }
        if (typetag[tlen] == ']')
        {
            if (!array_start)
            {
                if (value_count) *value_count = vlen;
                return COSC_ETYPE;
            }
            if (vlen >= values_n)
                break;
            tlen = array_start;
            continue;
        }
#endif
        if (values && vlen < values_n)
            sz = cosc_read_value(buffer, size - req, typetag[tlen], values + vlen);
        else
            sz = cosc_read_value(buffer, size - req, typetag[tlen], 0);
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
            vlen++;
    }
    if (value_count) *value_count = vlen;
#ifndef COSC_NOARRAY
    if (array_start && (tlen >= typetag_n || typetag[tlen] != ']'))
        return COSC_ETYPE;
#endif
    return req;
}

cosc_int32 cosc_write_message(
    void *buffer,
    cosc_int32 size,
    const struct cosc_message *message,
    cosc_int32 use_psize,
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
        use_psize ? 8 : -1
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
    if (use_psize)
        cosc_write_int32(buffer, 4, req - 4);
    return req;
}

cosc_int32 cosc_read_message(
    const void *buffer,
    cosc_int32 size,
    struct cosc_message *message,
    cosc_int32 *psize,
    cosc_int32 *value_count
)
{
    cosc_int32 req = 0, sz, tmp_psize = 0;
    struct cosc_message tmp_message;
    if (!message)
        message = &tmp_message;
    sz = cosc_read_signature(
        buffer, size - req,
        &message->address, &message->address_n,
        &message->typetag, &message->typetag_n,
        psize ? &tmp_psize : 0
    );
    if (psize)
        *psize = tmp_psize;
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
    if (tmp_psize > size - 4)
        return COSC_EPSIZE;
    req += sz;
    buffer = (char *)buffer + sz;
    sz = cosc_read_values(
        buffer, size - req,
        message->typetag, message->typetag_n,
        message->values.read, message->values_n,
        value_count
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

#if !defined(COSC_NOWRITER) && !defined(COSC_NOREADER)

static void cosc_serial_setup(
    struct cosc_serial *serial,
    cosc_int32 buffer_size,
    struct cosc_level *levels,
    cosc_int32 level_max,
    cosc_uint32 flags
)
{
    serial->buffer_size = buffer_size > 0 ? (buffer_size < COSC_SIZE_MAX ? buffer_size : COSC_SIZE_MAX) : 0;
    serial->size = 0;
    serial->flags = flags;
    serial->levels = levels;
    serial->level_max = levels && level_max > 1 ? level_max : 0;
    if ((cosc_uint32)serial->level_max > COSC_SIZE_MAX / sizeof(struct cosc_level))
        serial->level_max = (COSC_SIZE_MAX / sizeof(struct cosc_level)) * sizeof(struct cosc_level);
    serial->level = -1;
}

static void cosc_serial_reset(
    struct cosc_serial *serial
)
{
    serial->size = 0;
    serial->level = -1;
}

static cosc_int32 cosc_serial_get_buffer_size(
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
    return (
        serial->levels[serial->level].start
        + serial->levels[serial->level].size
    );
}

static cosc_int32 cosc_serial_get_level_max(
    const struct cosc_serial *serial
)
{
    return serial->level_max;
}

static cosc_int32 cosc_serial_get_level(
    const struct cosc_serial *serial
)
{
    return serial->level + 1;
}

static cosc_int32 cosc_serial_get_level_size(
    const struct cosc_serial *serial
)
{
    if (serial->level < 0)
        return 0;
    return serial->levels[serial->level].size;
}

static cosc_int32 cosc_serial_get_level_type(
    const struct cosc_serial *serial
)
{
    if (serial->level < 0)
        return 0;
    return serial->levels[serial->level].type;
}

static cosc_int32 cosc_serial_get_level_msgtype(
    const struct cosc_serial *serial,
    const unsigned char *buffer
)
{
    if (serial->level < 0 || serial->levels[serial->level].type != 'M')
        return 0;
    cosc_int32 offset = serial->levels[serial->level].ttstart;
    offset += serial->levels[serial->level].ttindex;
    if (offset >= serial->levels[serial->level].ttend
        || offset >= serial->levels[serial->level].start + serial->levels[serial->level].size)
        return 0;
    return buffer[offset];
}

static cosc_int32 cosc_serial_next_msgtype(
    const struct cosc_serial *serial,
    const unsigned char *buffer
)
{
    if (serial->level < 0 || serial->levels[serial->level].type != 'M')
        return COSC_ELEVELTYPE;
    cosc_int32 index = 0;
    const char *buf = (const char *)buffer + serial->levels[serial->level].ttstart;
    cosc_int32 len = serial->levels[serial->level].ttend - serial->levels[serial->level].ttstart;
    index = serial->levels[serial->level].ttindex;
    if (index < len)
        index++;
    cosc_int32 avoid_infinite_loop = 0;
    while (index < len)
    {
        if (buf[index] == ',')
            index++;
        else if (buf[index] == '[')
        {
            index++;
            avoid_infinite_loop = 1;
        }
        else if (buf[index] == ']')
        {
            if (avoid_infinite_loop)
            {
                index++;
                break;
            }
            while (index > 0 && buf[index] != '[')
                index--;
        }
        else
            break;
    }
    return index;
}

static cosc_int32 cosc_serial_open_level(
    struct cosc_serial *serial,
    cosc_int32 type,
    cosc_int32 initial_size,
    const unsigned char *buffer
)
{
    if (serial->level >= serial->level_max - 1)
        return COSC_ELEVELMAX;
    if (serial->level < 0 && serial->size > 0 && !(serial->flags & COSC_SERIAL_PSIZE))
        return COSC_EPSIZEFLAG;
    switch (type)
    {
    case COSC_LEVEL_TYPE_BUNDLE:
        if (serial->level < 0
            || serial->levels[serial->level].type == COSC_LEVEL_TYPE_BLOB)
            break;
        return COSC_ELEVELTYPE;
    case COSC_LEVEL_TYPE_MESSAGE:
        if (serial->level < 0
            || serial->levels[serial->level].type == COSC_LEVEL_TYPE_BLOB
            || serial->levels[serial->level].type == COSC_LEVEL_TYPE_BUNDLE)
            break;
        return COSC_ELEVELTYPE;
    case COSC_LEVEL_TYPE_BLOB:
        if (cosc_serial_get_level_msgtype(serial, buffer) == 'b')
            break;
        return COSC_ELEVELTYPE;
    default:
        return COSC_ELEVELTYPE;
    }
    cosc_int32 start = cosc_serial_get_size(serial);
    serial->level++;
    serial->levels[serial->level].type = type;
    serial->levels[serial->level].start = start;
    serial->levels[serial->level].size = initial_size;
    if ((serial->flags & COSC_SERIAL_PSIZE) || serial->level > 0)
        serial->levels[serial->level].psize = (serial->flags & COSC_SERIAL_READ) ? cosc_load_int32(buffer + start) : 0;
    else
        serial->levels[serial->level].psize = -1;
    if (type == COSC_LEVEL_TYPE_MESSAGE)
    {
        serial->levels[serial->level].ttstart = serial->levels[serial->level].start;
        if (serial->levels[serial->level].psize >= 0)
        {
            initial_size -= 4;
            serial->levels[serial->level].ttstart += 4;
        }
        serial->levels[serial->level].ttstart += cosc_read_string(buffer + serial->levels[serial->level].ttstart, initial_size, 0, 0, 0);
        serial->levels[serial->level].ttend = serial->levels[serial->level].ttstart;
        while (serial->levels[serial->level].ttend < serial->levels[serial->level].ttstart + initial_size
               && buffer[serial->levels[serial->level].ttend] != 0)
            serial->levels[serial->level].ttend++;
        serial->levels[serial->level].ttend += COSC_PADMUST(serial->levels[serial->level].ttend);
        serial->levels[serial->level].ttindex = 0;
        serial->levels[serial->level].ttindex = cosc_serial_next_msgtype(serial, buffer);
    }
    return serial->level;
}

static void cosc_serial_close_level(
    struct cosc_serial *serial
)
{
    if (serial->level < 0)
        return;
    if (serial->level > 0)
        serial->levels[serial->level - 1].size += serial->levels[serial->level].size;
    else
        serial->size += serial->levels[serial->level].size;
    serial->level--;
}

static cosc_int32 cosc_serial_prepare(
    const struct cosc_serial *serial,
    cosc_int32 type,
    const unsigned char *buffer
)
{
    if (serial->level >= 0 && serial->levels[serial->level].type == COSC_LEVEL_TYPE_BLOB)
        return cosc_serial_get_size(serial);
    cosc_int32 cur_type = cosc_serial_get_level_msgtype(serial, buffer);
    if (cur_type == 'S')
        cur_type = 's';
    if (type == 'S')
        type = 's';
    if (cur_type != type)
        return COSC_EMSGTYPE;
    return cosc_serial_get_size(serial);
}

static cosc_int32 cosc_serial_finish(
    struct cosc_serial *serial,
    cosc_int32 size,
    const unsigned char *buffer
)
{
    if (size < 0)
        return size;
    cosc_int32 index = cosc_serial_next_msgtype(serial, buffer);
    if (index < 0)
        return index;
    serial->levels[serial->level].ttindex = index;
    serial->levels[serial->level].size += size;
    return size;
}

#endif /* !COSC_NOWRITER && !COSC_NOREADER */

#ifndef COSC_NOWRITER

void cosc_writer_setup(
    struct cosc_writer *writer,
    void *buffer,
    cosc_int32 buffer_size,
    struct cosc_level *levels,
    cosc_int32 level_max,
    cosc_uint32 flags
)
{
    flags &= ~COSC_SERIAL_READ;
    cosc_serial_setup(&writer->serial, buffer_size, levels, level_max, flags);
    writer->buffer = (unsigned char *)buffer;
}

void cosc_writer_reset(
    struct cosc_writer *writer
)
{
    cosc_serial_reset(&writer->serial);
}

cosc_int32 cosc_writer_get_buffer_size(
    const struct cosc_writer *writer
)
{
    return cosc_serial_get_buffer_size(&writer->serial);
}

cosc_int32 cosc_writer_get_size(
    const struct cosc_writer *writer
)
{
    return cosc_serial_get_size(&writer->serial);
}

cosc_int32 cosc_writer_get_level_max(
    const struct cosc_writer *writer
)
{
    return cosc_serial_get_level_max(&writer->serial);
}

cosc_int32 cosc_writer_get_level(
    const struct cosc_writer *writer
)
{
    return cosc_serial_get_level(&writer->serial);
}

cosc_int32 cosc_writer_get_level_size(
    const struct cosc_writer *writer
)
{
    return cosc_serial_get_level_size(&writer->serial);
}

cosc_int32 cosc_writer_get_level_type(
    const struct cosc_writer *writer
)
{
    return cosc_serial_get_level_type(&writer->serial);
}

cosc_int32 cosc_writer_get_level_msgtype(
    const struct cosc_writer *writer
)
{
    return cosc_serial_get_level_msgtype(&writer->serial, writer->buffer);
}

void *cosc_writer_get_buffer(
    const struct cosc_writer *writer
)
{
    return writer->buffer;
}

cosc_int32 cosc_writer_set_buffer(
    struct cosc_writer *writer,
    void *buffer,
    cosc_int32 size
)
{
    if (size < writer->serial.size)
        return COSC_EINVAL;
    writer->buffer = (unsigned char *)buffer;
    writer->serial.size = size;
    return 0;
}

cosc_int32 cosc_writer_open_bundle(
    struct cosc_writer *writer,
    cosc_uint64 timetag
)
{
    if (writer->serial.level >= writer->serial.level_max - 1)
        return COSC_ELEVELMAX;
    if (writer->serial.level < 0 && writer->serial.size > 0 && !(writer->serial.flags & COSC_SERIAL_PSIZE))
        return COSC_EPSIZEFLAG;
    cosc_int32 size = cosc_serial_get_size(&writer->serial);
    cosc_int32 has_psize = ((writer->serial.flags & COSC_SERIAL_PSIZE) || writer->serial.level >= 0);
    cosc_int32 ret = cosc_write_bundle(writer->buffer + size, writer->serial.buffer_size - size, timetag, has_psize ? 0 : -1);
    if (ret < 0)
        return ret;
    cosc_int32 level = cosc_serial_open_level(&writer->serial, COSC_LEVEL_TYPE_BUNDLE, ret, writer->buffer);
    if (level < 0)
        return level;
    return ret;
}

cosc_int32 cosc_writer_open_message(
    struct cosc_writer *writer,
    const char *address,
    cosc_int32 address_n,
    const char *typetag,
    cosc_int32 typetag_n
)
{
    if (!cosc_typetag_validate(typetag, typetag_n, 0))
        return COSC_ETYPE;
    if (writer->serial.level >= writer->serial.level_max - 1)
        return COSC_ELEVELMAX;
    if (writer->serial.level < 0 && writer->serial.size > 0 && !(writer->serial.flags & COSC_SERIAL_PSIZE))
        return COSC_EPSIZEFLAG;
    cosc_int32 size = cosc_serial_get_size(&writer->serial);
    cosc_int32 has_psize = ((writer->serial.flags & COSC_SERIAL_PSIZE) || writer->serial.level >= 0);
    cosc_int32 ret = cosc_write_signature(
        writer->buffer + size, writer->serial.buffer_size - size,
        address, address_n, typetag, typetag_n, has_psize ? 0 : -1
    );
    if (ret < 0)
        return ret;
    cosc_int32 level = cosc_serial_open_level(&writer->serial, COSC_LEVEL_TYPE_MESSAGE, ret, writer->buffer);
    if (level < 0)
        return level;
    return ret;
}

cosc_int32 cosc_writer_open_blob(
    struct cosc_writer *writer
)
{
    if (writer->serial.level >= writer->serial.level_max - 1)
        return COSC_ELEVELMAX;
    if (writer->serial.level < 0 && writer->serial.size > 0 && !(writer->serial.flags & COSC_SERIAL_PSIZE))
        return COSC_EPSIZEFLAG;
    cosc_int32 size = cosc_serial_get_size(&writer->serial);
    cosc_int32 ret = cosc_write_int32(writer->buffer + size, writer->serial.buffer_size - size, 0);
    if (ret < 0)
        return ret;
    cosc_int32 level = cosc_serial_open_level(&writer->serial, COSC_LEVEL_TYPE_BLOB, ret, writer->buffer);
    if (level < 0)
        return level;
    return ret;
}

cosc_int32 cosc_writer_close(
    struct cosc_writer *writer,
    cosc_int32 levels
)
{
    cosc_int32 size = 0;
    if (levels < 0)
        levels = writer->serial.level + 1;
    else if (levels > writer->serial.level + 1)
        return COSC_EINVAL;
    while (levels > 0)
    {
        cosc_int32 level_type = writer->serial.levels[writer->serial.level].type;
        switch (level_type)
        {
        case COSC_LEVEL_TYPE_BUNDLE:
            if ((writer->serial.flags & COSC_SERIAL_PSIZE) || writer->serial.level > 0)
                cosc_store_int32(
                    writer->buffer + writer->serial.levels[writer->serial.level].start,
                    writer->serial.levels[writer->serial.level].size - 4
                );
            break;
        case COSC_LEVEL_TYPE_MESSAGE:
            if (writer->buffer[writer->serial.levels[writer->serial.level].ttstart
                               + writer->serial.levels[writer->serial.level].ttindex - 1] != '[')
            {
                while (cosc_serial_get_level_msgtype(&writer->serial, writer->buffer))
                {
                    cosc_int32 old_index = writer->serial.levels[writer->serial.level].ttindex;
                    cosc_int32 ret = cosc_writer_skip(writer);
                    if (ret < 0)
                        return ret;
                    size += ret;
                    if (writer->serial.levels[writer->serial.level].ttindex < old_index)
                        break;
                }
            }
            if ((writer->serial.flags & COSC_SERIAL_PSIZE) || writer->serial.level > 0)
                cosc_store_int32(
                    writer->buffer + writer->serial.levels[writer->serial.level].start,
                    writer->serial.levels[writer->serial.level].size - 4
                );
            break;
        case COSC_LEVEL_TYPE_BLOB:
            size = COSC_PAD(writer->serial.levels[writer->serial.level].size);
            if (size > writer->serial.buffer_size - writer->serial.levels[writer->serial.level].size)
                return COSC_EOVERRUN;
            cosc_memset(
                writer->buffer
                + writer->serial.levels[writer->serial.level].start
                + writer->serial.levels[writer->serial.level].size,
                0, size
            );
            writer->serial.levels[writer->serial.level].size += size;
            if ((writer->serial.flags & COSC_SERIAL_PSIZE) || writer->serial.level > 0)
                cosc_store_int32(
                    writer->buffer + writer->serial.levels[writer->serial.level].start,
                    writer->serial.levels[writer->serial.level].size - 4
                );
            if (writer->serial.levels[writer->serial.level - 1].type == COSC_LEVEL_TYPE_MESSAGE)
                writer->serial.levels[writer->serial.level - 1].ttindex++;
            break;
        }
        cosc_serial_close_level(&writer->serial);
        levels--;
    }
    return size;
}

cosc_int32 cosc_writer_uint32(
    struct cosc_writer *writer,
    cosc_uint32 value
)
{
    cosc_int32 size = cosc_serial_prepare(&writer->serial, 'r', writer->buffer);
    if (size < 0)
        return size;
    cosc_int32 ret = cosc_write_uint32(writer->buffer + size, writer->serial.buffer_size - size, value);
    return cosc_serial_finish(&writer->serial, ret, writer->buffer);
}

cosc_int32 cosc_writer_int32(
    struct cosc_writer *writer,
    cosc_int32 value
)
{
    cosc_int32 size = cosc_serial_prepare(&writer->serial, 'i', writer->buffer);
    if (size < 0)
        return size;
    cosc_int32 ret = cosc_write_int32(writer->buffer + size, writer->serial.buffer_size - size, value);
    return cosc_serial_finish(&writer->serial, ret, writer->buffer);
}

cosc_int32 cosc_writer_float32(
    struct cosc_writer *writer,
    cosc_float32 value
)
{
    cosc_int32 size = cosc_serial_prepare(&writer->serial, 'f', writer->buffer);
    if (size < 0)
        return size;
    cosc_int32 ret = cosc_write_float32(writer->buffer + size, writer->serial.buffer_size - size, value);
    return cosc_serial_finish(&writer->serial, ret, writer->buffer);
}

cosc_int32 cosc_writer_uint64(
    struct cosc_writer *writer,
    cosc_uint64 value
)
{
    cosc_int32 size = cosc_serial_prepare(&writer->serial, 't', writer->buffer);
    if (size < 0)
        return size;
    cosc_int32 ret = cosc_write_uint64(writer->buffer + size, writer->serial.buffer_size - size, value);
    return cosc_serial_finish(&writer->serial, ret, writer->buffer);
}

cosc_int32 cosc_writer_int64(
    struct cosc_writer *writer,
    cosc_int64 value
)
{
    cosc_int32 size = cosc_serial_prepare(&writer->serial, 'h', writer->buffer);
    if (size < 0)
        return size;
    cosc_int32 ret = cosc_write_int64(writer->buffer + size, writer->serial.buffer_size - size, value);
    return cosc_serial_finish(&writer->serial, ret, writer->buffer);
}

cosc_int32 cosc_writer_float64(
    struct cosc_writer *writer,
    cosc_float64 value
)
{
    cosc_int32 size = cosc_serial_prepare(&writer->serial, 'd', writer->buffer);
    if (size < 0)
        return size;
    cosc_int32 ret = cosc_write_float64(writer->buffer + size, writer->serial.buffer_size - size, value);
    return cosc_serial_finish(&writer->serial, ret, writer->buffer);
}

cosc_int32 cosc_writer_string(
    struct cosc_writer *writer,
    const char *value,
    cosc_int32 value_n,
    cosc_int32 *length
)
{
    cosc_int32 size = cosc_serial_prepare(&writer->serial, 's', writer->buffer);
    if (size < 0)
        return size;
    cosc_int32 ret = cosc_write_string(writer->buffer + size, writer->serial.buffer_size - size, value, value_n, length);
    return cosc_serial_finish(&writer->serial, ret, writer->buffer);
}

cosc_int32 cosc_writer_blob(
    struct cosc_writer *writer,
    const void *value,
    cosc_int32 value_size
)
{
    cosc_int32 size = cosc_serial_prepare(&writer->serial, 'b', writer->buffer);
    if (size < 0)
        return size;
    cosc_int32 ret = cosc_write_blob(writer->buffer + size, writer->serial.buffer_size - size, value, value_size);
    return cosc_serial_finish(&writer->serial, ret, writer->buffer);
}

cosc_int32 cosc_writer_char(
    struct cosc_writer *writer,
    cosc_int32 value
)
{
    cosc_int32 size = cosc_serial_prepare(&writer->serial, 'c', writer->buffer);
    if (size < 0)
        return size;
    cosc_int32 ret = cosc_write_char(writer->buffer + size, writer->serial.buffer_size - size, value);
    return cosc_serial_finish(&writer->serial, ret, writer->buffer);
}

cosc_int32 cosc_writer_midi(
    struct cosc_writer *writer,
    const unsigned char value[4]
)
{
    cosc_int32 size = cosc_serial_prepare(&writer->serial, 'm', writer->buffer);
    if (size < 0)
        return size;
    cosc_int32 ret = cosc_write_midi(writer->buffer + size, writer->serial.buffer_size - size, value);
    return cosc_serial_finish(&writer->serial, ret, writer->buffer);
}

cosc_int32 cosc_writer_bytes(
    struct cosc_writer *writer,
    const void *value,
    cosc_int32 value_n
)
{
    if (writer->serial.level < 0 || writer->serial.levels[writer->serial.level].type != COSC_LEVEL_TYPE_BLOB)
        return COSC_ELEVELTYPE;
    cosc_int32 size = cosc_serial_get_size(&writer->serial);
    if (value_n > COSC_SIZE_MAX - size)
        return COSC_ESIZEMAX;
    if (value_n > writer->serial.buffer_size - size)
        return COSC_EOVERRUN;
    if (value)
        cosc_memcpy(writer->buffer + writer->serial.levels[writer->serial.level].start + writer->serial.levels[writer->serial.level].size, value, value_n);
    else
        cosc_memset(writer->buffer + writer->serial.levels[writer->serial.level].start + writer->serial.levels[writer->serial.level].size, 0, value_n);
    writer->serial.levels[writer->serial.level].size += value_n;
    return value_n;
}

cosc_int32 cosc_writer_value(
    struct cosc_writer *writer,
    cosc_int32 type,
    const union cosc_value *value
)
{
    static const union cosc_value zero = {0};
    if (!value)
        value = &zero;
    cosc_int32 size = cosc_serial_prepare(&writer->serial, type, writer->buffer);
    if (size < 0)
        return size;
    cosc_int32 ret = cosc_write_value(writer->buffer + size, writer->serial.buffer_size - size, type, value);
    return cosc_serial_finish(&writer->serial, ret, writer->buffer);
}

cosc_int32 cosc_writer_message(
    struct cosc_writer *writer,
    const struct cosc_message *message,
    cosc_int32 *value_count
)
{
#ifdef __cplusplus
    static const struct cosc_message zero = {};
#else
    static const struct cosc_message zero = {0};
#endif
    if (!message)
        message = &zero;
    if (writer->serial.level >= 0
        && writer->serial.levels[writer->serial.level].type != COSC_LEVEL_TYPE_BUNDLE
        && writer->serial.levels[writer->serial.level].type != COSC_LEVEL_TYPE_BLOB)
        return COSC_ELEVELTYPE;
    if (writer->serial.level < 0 && writer->serial.size > 0 && !(writer->serial.flags & COSC_SERIAL_PSIZE))
        return COSC_EPSIZEFLAG;
    cosc_int32 size = cosc_serial_get_size(&writer->serial);
    cosc_int32 has_psize = ((writer->serial.flags & COSC_SERIAL_PSIZE) || writer->serial.level >= 0);
    cosc_int32 ret = cosc_write_message(
        writer->buffer + size, writer->serial.buffer_size - size,
        message, has_psize, value_count
    );
    if (ret < 0)
        return ret;
    if (writer->serial.level >= 0)
        writer->serial.levels[writer->serial.level].size += ret;
    return ret;
}

cosc_int32 cosc_writer_skip(
    struct cosc_writer *writer
)
{
    cosc_int32 cur_type = cosc_serial_get_level_msgtype(&writer->serial, writer->buffer);
    return cosc_writer_value(writer, cur_type, 0);
}

#endif /* !COSC_NOWRITER */

#ifndef COSC_NOREADER

void cosc_reader_setup(
    struct cosc_reader *reader,
    const void *buffer,
    cosc_int32 size,
    struct cosc_level *levels,
    cosc_int32 level_max,
    cosc_uint32 flags
)
{
    flags |= COSC_SERIAL_READ;
    cosc_serial_setup(&reader->serial, size, levels, level_max, flags);
    reader->buffer = (const unsigned char *)buffer;
}

void cosc_reader_reset(
    struct cosc_reader *reader
)
{
    cosc_serial_reset(&reader->serial);
}

cosc_int32 cosc_reader_get_buffer_size(
    const struct cosc_reader *reader
)
{
    return cosc_serial_get_buffer_size(&reader->serial);
}

cosc_int32 cosc_reader_get_size(
    const struct cosc_reader *reader
)
{
    return cosc_serial_get_size(&reader->serial);
}

cosc_int32 cosc_reader_get_psize(
    const struct cosc_reader *reader
)
{
    if (reader->serial.level < 0 || reader->serial.levels[reader->serial.level].psize < 0)
        return reader->serial.buffer_size;
    return reader->serial.levels[reader->serial.level].start + reader->serial.levels[reader->serial.level].psize;
}

cosc_int32 cosc_reader_get_level_max(
    const struct cosc_reader *reader
)
{
    return cosc_serial_get_level_max(&reader->serial);
}

cosc_int32 cosc_reader_get_level(
    const struct cosc_reader *reader
)
{
    return cosc_serial_get_level(&reader->serial);
}

cosc_int32 cosc_reader_get_level_size(
    const struct cosc_reader *reader
)
{
    return cosc_serial_get_level_size(&reader->serial);
}

cosc_int32 cosc_reader_get_level_psize(
    const struct cosc_reader *reader
)
{
    if (reader->serial.level < 0 || reader->serial.levels[reader->serial.level].psize < 0)
        return -1;
    return reader->serial.levels[reader->serial.level].psize;
}

cosc_int32 cosc_reader_get_level_type(
    const struct cosc_reader *reader
)
{
    return cosc_serial_get_level_type(&reader->serial);
}

cosc_int32 cosc_reader_get_level_msgtype(
    const struct cosc_reader *reader
)
{
    return cosc_serial_get_level_msgtype(&reader->serial, reader->buffer);
}

const void *cosc_reader_get_buffer(
    const struct cosc_reader *reader
)
{
    return reader->buffer;
}

cosc_int32 cosc_reader_peek_bundle(
    struct cosc_reader *reader,
    cosc_uint64 *timetag,
    cosc_int32 *psize
)
{
    if (reader->serial.level >= reader->serial.level_max - 1)
        return COSC_ELEVELMAX;
    if (reader->serial.level < 0 && reader->serial.size > 0 && !(reader->serial.flags & COSC_SERIAL_PSIZE))
        return COSC_EPSIZEFLAG;
    cosc_int32 size = cosc_serial_get_size(&reader->serial);
    cosc_int32 has_psize = ((reader->serial.flags & COSC_SERIAL_PSIZE) || reader->serial.level >= 0);
    cosc_int32 tmp_psize = -1;
    cosc_int32 size_max = cosc_reader_get_psize(reader);
    cosc_int32 ret = cosc_read_bundle(reader->buffer + size, size_max - size, timetag, has_psize ? &tmp_psize : 0);
    if (ret < 0)
        return ret;
    if (psize)
        *psize = tmp_psize;
    return ret;
}

cosc_int32 cosc_reader_open_bundle(
    struct cosc_reader *reader,
    cosc_uint64 *timetag,
    cosc_int32 *psize
)
{
    if (reader->serial.level >= reader->serial.level_max - 1)
        return COSC_ELEVELMAX;
    if (reader->serial.level < 0 && reader->serial.size > 0 && !(reader->serial.flags & COSC_SERIAL_PSIZE))
        return COSC_EPSIZEFLAG;
    cosc_int32 size = cosc_serial_get_size(&reader->serial);
    cosc_int32 has_psize = ((reader->serial.flags & COSC_SERIAL_PSIZE) || reader->serial.level >= 0);
    cosc_int32 tmp_psize = -1;
    cosc_int32 size_max = cosc_reader_get_psize(reader);
    cosc_int32 ret = cosc_read_bundle(reader->buffer + size, size_max - size, timetag, has_psize ? &tmp_psize : 0);
    if (ret < 0)
        return ret;
    cosc_int32 level = cosc_serial_open_level(&reader->serial, COSC_LEVEL_TYPE_BUNDLE, ret, reader->buffer);
    if (level < 0)
        return level;
    if (psize)
        *psize = tmp_psize;
    return ret;
}

cosc_int32 cosc_reader_open_message(
    struct cosc_reader *reader,
    const char **address,
    cosc_int32 *address_n,
    const char **typetag,
    cosc_int32 *typetag_n,
    cosc_int32 *psize
)
{
    if (reader->serial.level >= reader->serial.level_max - 1)
        return COSC_ELEVELMAX;
    if (reader->serial.level < 0 && reader->serial.size > 0 && !(reader->serial.flags & COSC_SERIAL_PSIZE))
        return COSC_EPSIZEFLAG;
    cosc_int32 size = cosc_serial_get_size(&reader->serial);
    cosc_int32 has_psize = ((reader->serial.flags & COSC_SERIAL_PSIZE) || reader->serial.level >= 0);
    cosc_int32 tmp_psize = -1;
    cosc_int32 size_max = cosc_reader_get_psize(reader);
    const char *tmp_typetag;
    cosc_int32 tmp_typetag_n;
    cosc_int32 ret = cosc_read_signature(
        reader->buffer + size, size_max - size,
        address, address_n, &tmp_typetag, &tmp_typetag_n, has_psize ? &tmp_psize : 0
    );
    if (ret < 0)
        return ret;
    if (!cosc_typetag_validate(tmp_typetag, tmp_typetag_n, 0))
        return COSC_ETYPE;
    cosc_int32 level = cosc_serial_open_level(&reader->serial, COSC_LEVEL_TYPE_MESSAGE, ret, reader->buffer);
    if (level < 0)
        return level;
    if (typetag)
        *typetag = tmp_typetag;
    if (typetag_n)
        *typetag_n = tmp_typetag_n;
    if (psize)
        *psize = tmp_psize;
    return ret;
}

cosc_int32 cosc_reader_open_blob(
    struct cosc_reader *reader,
    cosc_int32 *psize
)
{
    if (reader->serial.level >= reader->serial.level_max - 1)
        return COSC_ELEVELMAX;
    if (reader->serial.level < 0 && reader->serial.size > 0 && !(reader->serial.flags & COSC_SERIAL_PSIZE))
        return COSC_EPSIZEFLAG;
    cosc_int32 size = cosc_serial_get_size(&reader->serial);
    cosc_int32 size_max = cosc_reader_get_psize(reader);
    cosc_int32 ret = cosc_read_int32(reader->buffer + size, size_max - size, psize);
    if (ret < 0)
        return ret;
    cosc_int32 level = cosc_serial_open_level(&reader->serial, COSC_LEVEL_TYPE_BLOB, ret, reader->buffer);
    if (level < 0)
        return level;
    return ret;
}

cosc_int32 cosc_reader_close(
    struct cosc_reader *reader,
    cosc_int32 levels
)
{
    cosc_int32 size = 0;
    if (levels < 0)
        levels = reader->serial.level + 1;
    else if (levels > reader->serial.level + 1)
        return COSC_EINVAL;
    while (levels > 0)
    {
        cosc_int32 level_type = reader->serial.levels[reader->serial.level].type;
        switch (level_type)
        {
        case COSC_LEVEL_TYPE_BUNDLE:
            break;
        case COSC_LEVEL_TYPE_MESSAGE:
            if (reader->buffer[reader->serial.levels[reader->serial.level].ttstart
                               + reader->serial.levels[reader->serial.level].ttindex - 1] != '[')
            {
                while (cosc_serial_get_level_msgtype(&reader->serial, reader->buffer))
                {
                    cosc_int32 old_index = reader->serial.levels[reader->serial.level].ttindex;
                    cosc_int32 ret = cosc_reader_skip(reader);
                    if (ret < 0)
                        return ret;
                    size += ret;
                    if (reader->serial.levels[reader->serial.level].ttindex < old_index)
                        break;
                }
            }
            break;
        case COSC_LEVEL_TYPE_BLOB:
            size = COSC_PAD(reader->serial.levels[reader->serial.level].size);
            if (size > reader->serial.buffer_size - reader->serial.levels[reader->serial.level].size)
                return COSC_EOVERRUN;
            reader->serial.levels[reader->serial.level].size += size;
            if (reader->serial.levels[reader->serial.level - 1].type == COSC_LEVEL_TYPE_MESSAGE)
                reader->serial.levels[reader->serial.level - 1].ttindex++;
            break;
        }
        cosc_serial_close_level(&reader->serial);
        levels--;
    }
    return size;
}

cosc_int32 cosc_reader_uint32(
    struct cosc_reader *reader,
    cosc_uint32 *value
)
{
    cosc_int32 size = cosc_serial_prepare(&reader->serial, 'r', reader->buffer);
    if (size < 0)
        return size;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_uint32(reader->buffer + size, size_max - size, value);
    return cosc_serial_finish(&reader->serial, ret, reader->buffer);
}

cosc_int32 cosc_reader_int32(
    struct cosc_reader *reader,
    cosc_int32 *value
)
{
    cosc_int32 size = cosc_serial_prepare(&reader->serial, 'i', reader->buffer);
    if (size < 0)
        return size;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_int32(reader->buffer + size, size_max - size, value);
    return cosc_serial_finish(&reader->serial, ret, reader->buffer);
}

cosc_int32 cosc_reader_float32(
    struct cosc_reader *reader,
    cosc_float32 *value
)
{
    cosc_int32 size = cosc_serial_prepare(&reader->serial, 'f', reader->buffer);
    if (size < 0)
        return size;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_float32(reader->buffer + size, size_max - size, value);
    return cosc_serial_finish(&reader->serial, ret, reader->buffer);
}

cosc_int32 cosc_reader_uint64(
    struct cosc_reader *reader,
    cosc_uint64 *value
)
{
    cosc_int32 size = cosc_serial_prepare(&reader->serial, 't', reader->buffer);
    if (size < 0)
        return size;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_uint64(reader->buffer + size, size_max - size, value);
    return cosc_serial_finish(&reader->serial, ret, reader->buffer);
}

cosc_int32 cosc_reader_int64(
    struct cosc_reader *reader,
    cosc_int64 *value
)
{
    cosc_int32 size = cosc_serial_prepare(&reader->serial, 'h', reader->buffer);
    if (size < 0)
        return size;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_int64(reader->buffer + size, size_max - size, value);
    return cosc_serial_finish(&reader->serial, ret, reader->buffer);
}

cosc_int32 cosc_reader_float64(
    struct cosc_reader *reader,
    cosc_float64 *value
)
{
    cosc_int32 size = cosc_serial_prepare(&reader->serial, 'd', reader->buffer);
    if (size < 0)
        return size;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_float64(reader->buffer + size, size_max - size, value);
    return cosc_serial_finish(&reader->serial, ret, reader->buffer);
}

cosc_int32 cosc_reader_string(
    struct cosc_reader *reader,
    char *value,
    cosc_int32 value_n,
    const char **ptr,
    cosc_int32 *length
)
{
    cosc_int32 size = cosc_serial_prepare(&reader->serial, 's', reader->buffer);
    if (size < 0)
        return size;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_string(reader->buffer + size, size_max - size, value, value_n, length);
    if (ptr)
        *ptr = (const char *)reader->buffer + size;
    return cosc_serial_finish(&reader->serial, ret, reader->buffer);
}

cosc_int32 cosc_reader_blob(
    struct cosc_reader *reader,
    void *value,
    cosc_int32 value_n,
    const void **data,
    cosc_int32 *data_size
)
{
    cosc_int32 size = cosc_serial_prepare(&reader->serial, 'b', reader->buffer);
    if (size < 0)
        return size;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_blob(reader->buffer + size, size_max - size, value, value_n, data, data_size);
    return cosc_serial_finish(&reader->serial, ret, reader->buffer);
}

cosc_int32 cosc_reader_char(
    struct cosc_reader *reader,
    cosc_int32 *value
)
{
    cosc_int32 size = cosc_serial_prepare(&reader->serial, 'c', reader->buffer);
    if (size < 0)
        return size;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_char(reader->buffer + size, size_max - size, value);
    return cosc_serial_finish(&reader->serial, ret, reader->buffer);
}

cosc_int32 cosc_reader_midi(
    struct cosc_reader *reader,
    unsigned char value[4]
)
{
    cosc_int32 size = cosc_serial_prepare(&reader->serial, 'm', reader->buffer);
    if (size < 0)
        return size;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_midi(reader->buffer + size, size_max - size, value);
    return cosc_serial_finish(&reader->serial, ret, reader->buffer);
}

cosc_int32 cosc_reader_bytes(
    struct cosc_reader *reader,
    void *value,
    cosc_int32 value_n
)
{
    if (reader->serial.level < 0 || reader->serial.levels[reader->serial.level].type != COSC_LEVEL_TYPE_BLOB)
        return COSC_ELEVELTYPE;
    cosc_int32 size = cosc_serial_get_size(&reader->serial);
    if (value_n > COSC_SIZE_MAX - size)
        return COSC_ESIZEMAX;
    cosc_int32 size_max = cosc_reader_get_psize(reader);
    if (value_n > size_max - size)
        value_n = size_max - size;
    if (value)
        cosc_memcpy(value, reader->buffer + reader->serial.levels[reader->serial.level].start + reader->serial.levels[reader->serial.level].size, value_n);
    reader->serial.levels[reader->serial.level].size += value_n;
    return value_n;
}

cosc_int32 cosc_reader_value(
    struct cosc_reader *reader,
    cosc_int32 *type,
    union cosc_value *value
)
{
    cosc_int32 tmp_type = cosc_serial_get_level_msgtype(&reader->serial, reader->buffer);
    if (type)
        *type = tmp_type;
    cosc_int32 size = cosc_serial_prepare(&reader->serial, tmp_type, reader->buffer);
    if (size < 0)
        return size;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_value(reader->buffer + size, size_max - size, tmp_type, value);
    return cosc_serial_finish(&reader->serial, ret, reader->buffer);
}

COSC_API cosc_int32 cosc_reader_message(
    struct cosc_reader *reader,
    struct cosc_message *message,
    cosc_int32 *value_count
)
{
#ifdef __cplusplus
    static struct cosc_message discard = {};
#else
    static struct cosc_message discard = {0};
#endif
    if (!message)
        message = &discard;
    if (reader->serial.level >= 0
        && reader->serial.levels[reader->serial.level].type != COSC_LEVEL_TYPE_BUNDLE
        && reader->serial.levels[reader->serial.level].type != COSC_LEVEL_TYPE_BLOB)
        return COSC_ELEVELTYPE;
    if (reader->serial.level < 0 && reader->serial.size > 0 && !(reader->serial.flags & COSC_SERIAL_PSIZE))
        return COSC_EPSIZEFLAG;
    cosc_int32 size = cosc_serial_get_size(&reader->serial);
    cosc_int32 has_psize = ((reader->serial.flags & COSC_SERIAL_PSIZE) || reader->serial.level >= 0);
    cosc_int32 tmp_psize;
    cosc_int32 level_psize = cosc_reader_get_level_psize(reader);
    cosc_int32 size_max = level_psize >= 0 && reader->serial.buffer_size < level_psize ? level_psize : reader->serial.buffer_size;
    cosc_int32 ret = cosc_read_message(
        reader->buffer + size, size_max - size,
        message, has_psize ? &tmp_psize : 0, value_count
    );
    if (ret < 0)
        return ret;
    if (reader->serial.level >= 0)
        reader->serial.levels[reader->serial.level].size += ret;
    return ret;
}

cosc_int32 cosc_reader_skip(
    struct cosc_reader *reader
)
{
    return cosc_reader_value(reader, 0, 0);
}

#endif /* !COSC_NOREADER */
