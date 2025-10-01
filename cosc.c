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
#else
#include <string.h>
#include <stdlib.h>
#define cosc_memcpy memcpy
#define cosc_memset memset
#define cosc_memmove memmove
#define cosc_memcmp memcmp
#endif

inline static void cosc_store_uint32(
    void *buffer,
    cosc_uint32 value
)
{
#ifdef COSC_NOSWAP
    ((unsigned char *)buffer)[0] = ((const unsigned char *)(&value))[0];
    ((unsigned char *)buffer)[1] = ((const unsigned char *)(&value))[1];
    ((unsigned char *)buffer)[2] = ((const unsigned char *)(&value))[2];
    ((unsigned char *)buffer)[3] = ((const unsigned char *)(&value))[3];
#else
    ((unsigned char *)buffer)[0] = (value & 0xff000000) >> 24;
    ((unsigned char *)buffer)[1] = (value & 0xff0000) >> 16;
    ((unsigned char *)buffer)[2] = (value & 0xff00) >> 8;
    ((unsigned char *)buffer)[3] = value & 0xff;
#endif
}

inline static cosc_uint32 cosc_load_uint32(
    const void *buffer
)
{
#ifdef COSC_NOSWAP
    cosc_uint32 tmp;
    ((unsigned char *)(&tmp))[0] = ((const unsigned char *)buffer)[0];
    ((unsigned char *)(&tmp))[1] = ((const unsigned char *)buffer)[1];
    ((unsigned char *)(&tmp))[2] = ((const unsigned char *)buffer)[2];
    ((unsigned char *)(&tmp))[3] = ((const unsigned char *)buffer)[3];
    return tmp;
#else
    return (
        ((cosc_uint32)((const unsigned char *)buffer)[0] << 24)
        | ((cosc_uint32)((const unsigned char *)buffer)[1] << 16)
        | ((cosc_uint32)((const unsigned char *)buffer)[2] << 8)
        | ((cosc_uint32)((const unsigned char *)buffer)[3])
    );
#endif
}

inline static void cosc_store_int32(
    void *buffer,
    cosc_int32 value
)
{
    union { cosc_int32 in; cosc_uint32 out; } tmp = {value};
    cosc_store_uint32(buffer, tmp.out);
}

inline static cosc_int32 cosc_load_int32(
    const void *buffer
)
{
    union { cosc_uint32 in; cosc_int32 out; } tmp = {cosc_load_uint32(buffer)};
    return tmp.out;
}

inline static void cosc_store_float32(
    void *buffer,
    cosc_float32 value
)
{
#ifdef COSC_NOFLOAT32
    cosc_store_uint32(buffer, value);
#else
    union { cosc_float32 in; cosc_uint32 out; } tmp = {value};
    cosc_store_uint32(buffer, tmp.out);
#endif
}

inline static cosc_float32 cosc_load_float32(
    const void *buffer
)
{
    union { cosc_uint32 in; cosc_float32 out; } tmp = {cosc_load_uint32(buffer)};
    return tmp.out;
}

inline static void cosc_store_uint64(
    void *buffer,
    cosc_uint64 value
)
{
#ifdef COSC_NOSWAP
    ((unsigned char *)buffer)[0] = ((const unsigned char *)(&value))[0];
    ((unsigned char *)buffer)[1] = ((const unsigned char *)(&value))[1];
    ((unsigned char *)buffer)[2] = ((const unsigned char *)(&value))[2];
    ((unsigned char *)buffer)[3] = ((const unsigned char *)(&value))[3];
    ((unsigned char *)buffer)[4] = ((const unsigned char *)(&value))[4];
    ((unsigned char *)buffer)[5] = ((const unsigned char *)(&value))[5];
    ((unsigned char *)buffer)[6] = ((const unsigned char *)(&value))[6];
    ((unsigned char *)buffer)[7] = ((const unsigned char *)(&value))[7];
#else
#ifdef COSC_NO64
    ((unsigned char *)buffer)[0] = (value.hi & 0xff000000) >> 24;
    ((unsigned char *)buffer)[1] = (value.hi & 0xff0000) >> 16;
    ((unsigned char *)buffer)[2] = (value.hi & 0xff00) >> 8;
    ((unsigned char *)buffer)[3] = value.hi & 0xff;
    ((unsigned char *)buffer)[4] = (value.lo & 0xff000000) >> 24;
    ((unsigned char *)buffer)[5] = (value.lo & 0xff0000) >> 16;
    ((unsigned char *)buffer)[6] = (value.lo & 0xff00) >> 8;
    ((unsigned char *)buffer)[7] = value.lo & 0xff;
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
    ((unsigned char *)(&tmp))[0] = ((const unsigned char *)buffer)[0];
    ((unsigned char *)(&tmp))[1] = ((const unsigned char *)buffer)[1];
    ((unsigned char *)(&tmp))[2] = ((const unsigned char *)buffer)[2];
    ((unsigned char *)(&tmp))[3] = ((const unsigned char *)buffer)[3];
    ((unsigned char *)(&tmp))[4] = ((const unsigned char *)buffer)[4];
    ((unsigned char *)(&tmp))[5] = ((const unsigned char *)buffer)[5];
    ((unsigned char *)(&tmp))[6] = ((const unsigned char *)buffer)[6];
    ((unsigned char *)(&tmp))[7] = ((const unsigned char *)buffer)[7];
    return tmp;
#else
#ifdef COSC_NO64
    struct cosc_64bits tmp = {cosc_load_uint32(buffer), cosc_load_uint32((const char *)buffer + 4)};
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

inline static void cosc_store_int64(
    void *buffer,
    cosc_int64 value
)
{
#ifdef COSC_NO64
    cosc_store_uint64(buffer, value);
#else
    union { cosc_int64 in; cosc_uint64 out; } tmp = {value};
    cosc_store_uint64(buffer, tmp.out);
#endif
}

inline static cosc_int64 cosc_load_int64(
    const void *buffer
)
{
#ifdef COSC_NO64
    return cosc_load_uint64(buffer);
#else
    union { cosc_uint64 in; cosc_int64 out; } tmp = {cosc_load_uint64(buffer)};
    return tmp.out;
#endif
}

inline static void cosc_store_float64(
    void *buffer,
    cosc_float64 value
)
{
#ifdef COSC_NO64
    cosc_store_uint64(buffer, value);
#else
    union { cosc_float64 in; cosc_uint64 out; } tmp = {value};
    cosc_store_uint64(buffer, tmp.out);
#endif
}

inline static cosc_float64 cosc_load_float64(
    const void *buffer
)
{
#ifdef COSC_NO64
    return cosc_load_uint64(buffer);
#else
    union { cosc_uint64 in; cosc_float64 out; } tmp = {cosc_load_uint64(buffer)};
    return tmp.out;
#endif
}

#if defined(COSC_NO64)

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

    struct cosc_64bits res =
    {
        (product[3] << 16) | product[2],
        (product[1] << 16) | product[0]
    };
    return res;
}

static void cosc_div64(struct cosc_64bits *dividend, cosc_uint32 divisor)
{
    struct cosc_64bits q = {0, 0};
    struct cosc_64bits r = {0, 0};
    if (divisor > 0)
    {
        for (cosc_int32 i = 63; i >= 0; i--)
        {
            r.hi <<= 1;
            r.hi |= (r.lo & 0x80000000) >> 31;
            r.lo <<= 1;
            if (i < 32)
                r.lo |= (dividend->lo & (1U << i)) >> i;
            else
                r.lo |= (dividend->hi & (1U << (i - 32))) >> (i - 32);
            if (r.lo >= divisor || r.hi > 0)
            {
                if (r.lo < divisor)
                    r.hi--;
                r.lo -= divisor;
                if (i < 32)
                    q.lo |= 1U << i;
                else
                    q.hi |= 1U << (i - 32);
            }
        }
    }
    *dividend = q;
}

static void cosc_add64(struct cosc_64bits *augend, cosc_uint32 addend)
{
    if (addend > 0xffffffff - augend->lo)
        augend->hi++;
    augend->lo += addend;
}

#endif /* COSC_NO64 */

#ifndef COSC_NOEXTRAS

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

//
// Public below.
//

cosc_int32 cosc_address_char_validate(
    char c
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
    char c
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
        while (len < typetag_n)
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

cosc_int32 cosc_pattern_char_validate(
    char c
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
    if (pattern_n > 0 && *pattern == ',')
    {
        is_typetag = 1;
        p_offset++;
    }
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
    sz = cosc_read_string(buffer, size, &len);
    if (sz < 0)
        return sz;
    if (cosc_pattern_match((const char *)buffer, sz, apattern, apattern_n)
        && cosc_pattern_match((const char *)buffer + sz, size - sz, tpattern, tpattern_n))
        return 1;
    return 0;
}

#endif /* !COSC_NOEXTRAS */

cosc_uint32 cosc_timetag_to_time(
    cosc_uint64 timetag,
    cosc_uint32 *nanos
)
{
#ifdef COSC_NO64
    if (nanos)
    {
        struct cosc_64bits tmp = cosc_mul64(timetag.lo, 1000000000);
        cosc_add64(&tmp, 500000000);
        *nanos = tmp.hi; // Same as >> 32.
    }
    return timetag.hi;
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

cosc_uint64 cosc_timetag_from_time(
    cosc_uint32 seconds,
    cosc_uint32 nanos
)
{
    seconds += nanos / 1000000000;
    nanos %= 1000000000;
#ifdef COSC_NO64
    struct cosc_64bits res = {nanos, 0};
    cosc_add64(&res, 0x20000000);
    cosc_div64(&res, 1000000000);
    res.hi = seconds;
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
        cosc_store_int32(buffer, value);
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
    if (value) *value = cosc_load_int32(buffer);
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
        cosc_store_float32(buffer, value);
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
    if (value) *value = cosc_load_float32(buffer);
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
        cosc_store_int64(buffer, value);
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
    if (value) *value = cosc_load_int64(buffer);
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
        if (size < 8) return COSC_EOVERRUN;
        cosc_store_float64(buffer, value);
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
    if (value) *value = cosc_load_float64(buffer);
    return 8;
}

cosc_int32 cosc_write_string(
    void *buffer,
    cosc_int32 size,
    const char *s,
    cosc_int32 n
)
{
    cosc_int32 len = 0, pad;
    if (buffer)
    {
        if (s)
            while (len < n && s[len] != 0)
            {
                if (len >= size)
                    return COSC_EOVERRUN;
                ((char *)buffer)[len] = s[len];
                len++;
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
        if (s)
            while (len < n && s[len] != 0)
                len++;
        pad = COSC_PADMUST(len);
        if (len > COSC_SIZE_MAX - pad)
            return COSC_ESIZEMAX;
    }
    return len + pad;
}

cosc_int32 cosc_read_string(
    const void *buffer,
    cosc_int32 size,
    cosc_int32 *length
)
{
    cosc_int32 len = 0, pad;
    if (size < 4)
        return COSC_EOVERRUN;
    while (len < size && ((const char *)buffer)[len] != 0)
        len++;
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
    const void *data,
    cosc_int32 data_size
)
{
    if (data_size < 0)
        data_size = 0;
    else if (data_size > COSC_SIZE_MAX - 4)
        return COSC_ESIZEMAX;
    cosc_int32 pad = COSC_PAD(data_size);
    if (data_size + 4 > COSC_SIZE_MAX - pad)
        return COSC_ESIZEMAX;
    if (buffer)
    {
        if (data_size > size - pad - 4)
            return COSC_EOVERRUN;
        cosc_store_int32(buffer, data_size);
        if (data)
        {
            cosc_memcpy((char *)buffer + 4, data, data_size);
            cosc_memset((char *)buffer + 4 + data_size, 0, pad);
        }
        else
            cosc_memset((char *)buffer + 4, 0, data_size + pad);
    }
    return data_size + pad + 4;
}

cosc_int32 cosc_read_blob(
    const void *buffer,
    cosc_int32 size,
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
    if (data)
        *data = psize > 0 ? (const char *)buffer + 4 : 0;
    if (data_size)
        *data_size = psize;
    return psize + 4 + pad;
}

cosc_int32 cosc_write_char(
    void *buffer,
    cosc_int32 size,
    char value
)
{
    if (buffer)
    {
        if (size < 4)
            return COSC_EOVERRUN;
        ((char *)buffer)[0] = value;
        ((char *)buffer)[1] = 0;
        ((char *)buffer)[2] = 0;
        ((char *)buffer)[3] = 0;
    }
    return 4;
}

cosc_int32 cosc_read_char(
    const void *buffer,
    cosc_int32 size,
    char *value
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
        if (cosc_memcmp(buffer, "#bundle", 8) != 0)
            return COSC_ETYPE;
        if (timetag)
            *timetag = cosc_load_uint64((const unsigned char *)buffer + 8);
    }
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
        sz = cosc_write_string((unsigned char *)buffer + req, size - req, address, address_n);
        if (sz < 0)
            return sz;
        if (sz > COSC_SIZE_MAX - req)
            return COSC_ESIZEMAX;
        req += sz;
        sz = cosc_write_string((unsigned char *)buffer + req, size - req, typetag, typetag_n);
        if (sz < 0)
            return sz;
        if (sz > COSC_SIZE_MAX - req)
            return COSC_ESIZEMAX;
        req += sz;
        if (psize >= 0)
            cosc_store_int32(buffer, req - 4);
    }
    else
    {
        if (psize >= 0)
            req += 4;
        sz = cosc_write_string(0, 0, address, address_n);
        if (sz < 0)
            return sz;
        if (sz > COSC_SIZE_MAX - req)
            return COSC_ESIZEMAX;
        req += sz;
        sz = cosc_write_string(0, 0, typetag, typetag_n);
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
    if (size < 12)
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
    sz = cosc_read_string((const char *)buffer + req, size - req, address_n);
    if (sz < 0)
        return sz;
    req += sz;
    if (typetag)
        *typetag = (const char *)buffer + req;
    sz = cosc_read_string((const char *)buffer + req, size - req, typetag_n);
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
#ifdef COSC_NO64
    static const struct cosc_64bits zero64 = {0, 0};
#endif
    if (buffer)
    {
        switch (type)
        {
        case 'i': return cosc_write_int32(buffer, size, value ? value->i : 0);
        case 'r': return cosc_write_uint32(buffer, size, value ? value->r : 0);
        case 'f': return cosc_write_float32(buffer, size, value ? value->f : 0);
#ifdef COSC_NO64
        case 'h': return cosc_write_int64(buffer, size, value ? value->h : zero64);
        case 't': return cosc_write_uint64(buffer, size, value ? value->t : zero64);
        case 'd': return cosc_write_float64(buffer, size, value ? value->d : zero64);
#else
        case 'h': return cosc_write_int64(buffer, size, value ? value->h : 0);
        case 't': return cosc_write_uint64(buffer, size, value ? value->t : 0);
        case 'd': return cosc_write_float64(buffer, size, value ? value->d : 0);
#endif
        case 'c': return cosc_write_char(buffer, size, value ? value->c : 0);
        case 'm': return cosc_write_midi(buffer, size, value ? value->m : 0);
        case 's':
        case 'S': return cosc_write_string(buffer, size, value ? value->s.s : 0, value ? value->s.length : 0);
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
        case 'S': return cosc_write_string(0, 0, value ? value->s.s : 0, value ? value->s.length : 0);
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
            return cosc_read_string(buffer, size, &value->s.length);
        }
        return cosc_read_string(buffer, size, 0);
    case 'b':
        if (value)
            return cosc_read_blob(buffer, size, &value->b.b, &value->b.size);
        return cosc_read_blob(buffer, size, 0, 0);
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
    cosc_int32 *value_count,
    cosc_int32 *value_offsets
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
        if (value_offsets && vlen < values_n)
            value_offsets[vlen] = req;
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
    cosc_int32 *value_count,
    cosc_int32 *value_offsets
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
        if (value_offsets && vlen < values_n)
            value_offsets[vlen] = req;
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
    cosc_int32 prefix,
    cosc_int32 *value_count,
    cosc_int32 *value_offsets
)
{
    cosc_int32 req = 0, sz;
    struct cosc_message tmp_message = {0, 0, 0, 0, {0}, 0};

    if (!message)
        message = &tmp_message;
    if (buffer)
    {
        sz = cosc_write_signature(
            buffer, size - req,
            message->address, message->address_n,
            message->typetag, message->typetag_n,
            prefix ? 8 : -1
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
            value_count, value_offsets
        );
        if (sz < 0)
            return sz;
        if (sz > COSC_SIZE_MAX - req)
            return COSC_SIZE_MAX;
        req += sz;
        cosc_write_int32(buffer, 4, req - 4);
    }
    else
    {
        sz = cosc_write_signature(
            0, 0,
            message->address, message->address_n,
            message->typetag, message->typetag_n,
            prefix ? 8 : -1
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
            0, 0,
            message->typetag, message->typetag_n,
            message->values.write, message->values_n,
            value_count, value_offsets
        );
        if (sz < 0)
            return sz;
        if (sz > COSC_SIZE_MAX - req)
            return COSC_SIZE_MAX;
        req += sz;
    }
    return req;
}

cosc_int32 cosc_read_message(
    const void *buffer,
    cosc_int32 size,
    struct cosc_message *message,
    cosc_int32 prefix,
    cosc_int32 *value_count,
    cosc_int32 *value_offsets
)
{
    cosc_int32 req = 0, sz, psize = 0;
    struct cosc_message tmp_message;
    if (!message)
        message = &tmp_message;
    sz = cosc_read_signature(
        buffer, size - req,
        &message->address, &message->address_n,
        &message->typetag, &message->typetag_n,
        prefix ? &psize : 0
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
    buffer = (char *)buffer + sz;
    sz = cosc_read_values(
        buffer, size - req,
        message->typetag, message->typetag_n,
        message->values.read, message->values_n,
        value_count, value_offsets
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

#if !defined(COSC_NOSTDLIB) && !defined(COSC_NODUMP) && !defined(COSC_NOEXTRAS)

#include <stdio.h>
#include <inttypes.h>

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
#ifdef COSC_NO64
    case 'h': return snprintf(s, n, "0x%08x %08x", value->h.hi, value->h.lo);
    case 't': return snprintf(s, n, "0x%08x %08x", value->t.hi, value->t.lo);
    case 'd': return snprintf(s, n, "0x%08x %08x", value->d.hi, value->d.lo);
#else
    case 'h': return snprintf(s, n, "%" PRId64, value->h);
    case 't': return snprintf(s, n, "%" PRIu64, value->t);
#ifdef COSC_NOFLOAT64
    case 'd': return snprintf(s, n, "0x%" PRIx64, value->d);
#else
    case 'd': return snprintf(s, n, "%f", value->d);
#endif
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
    cosc_int32 value_count = cosc_typetag_payload(0, 0, message->typetag, message->typetag_n, NULL);
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

#endif /* !COSC_NODUMP && !COSC_NOSTDLIB && !COSC_NOEXTRAS */
