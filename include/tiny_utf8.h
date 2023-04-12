#ifndef _TINY_UTF8_H_
#define _TINY_UTF8_H_

/**
 * Tiny UTF8
 * ========= 
 * 
 * A tiny, header-only UTF-8 encoding and decoding library for C and C++.
 * 
 * Usage:
 * 
 *   #define _TINY_UTF8_IMPL_
 *   #include "tiny_utf8.h"
 * 
 * Note: Only define _TINY_UTF8_IMPL_ once in your project.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Indicates that the decoded code point was invalid.
#define TINY_UTF8_ERR_INVALID_CODE_POINT 8

// Indicates that there were not enough bytes provided to decode.
#define TINY_UTF8_ERR_NOT_ENOUGH_BYTES 9

// tiny_utf8_rune represents a Unicode code point.
typedef int32_t tiny_utf8_rune;

// tiny_utf8_encode encodes rune into the start of buff of size n.  If buff is
// large enough, it returns the number bytes written to buff.  If buff is too
// small, then it returns the number bytes needed to encode rune.  If the rune
// cannot be encoded, it returns TINY_UTF8_ERR_* value.
extern size_t tiny_utf8_encode(tiny_utf8_rune rune, char* buff, size_t n);

// tiny_utf8_decode decodes a single Unicode rune into out_rune from buff where
// buff is of size n.  It decodes from the start of buff.  If successful, it
// returns the number of bytes read from the buffer.  Otherwise, it returns a
// TINY_UTF8_ERR_* value.
extern size_t tiny_utf8_decode(tiny_utf8_rune* out_rune, const char* buff, size_t n);

#ifdef __cplusplus
}
#endif

#ifdef _TINY_UTF8_IMPL_

size_t tiny_utf8_encode(tiny_utf8_rune rune, char* buff, size_t n) {
    if (rune < 0x80) {
        if (n < 1)
            return TINY_UTF8_ERR_NOT_ENOUGH_BYTES;

        buff[0] = rune & 0x7f;

        return 1;
    } else if (rune < 0x0800) {
        if (n < 2)
            return TINY_UTF8_ERR_NOT_ENOUGH_BYTES;

        buff[1] = 0x80 | (rune & 0x3f);
        buff[0] = 0xc0 | ((rune >> 6) & 0x1f);

        return 2;
    } else if (rune < 0x10000) {
        if (n < 3)
            return TINY_UTF8_ERR_NOT_ENOUGH_BYTES;

        buff[2] = 0x80 | (rune & 0x3f);
        buff[1] = 0x80 | ((rune >> 6) & 0x3f);
        buff[0] = 0xe0 | ((rune >> 12) & 0x0f);

        return 3;
    } else if (rune < 0x10ffff) {
        if (n < 4)
            return TINY_UTF8_ERR_NOT_ENOUGH_BYTES;

        buff[3] = 0x80 | (rune & 0x3f);
        buff[2] = 0x80 | ((rune >> 6) & 0x3f);
        buff[1] = 0x80 | ((rune >> 12) & 0x3f);
        buff[0] = 0xf0 | ((rune >> 18) & 0x07);

        return 4;
    } else {
        return TINY_UTF8_ERR_INVALID_CODE_POINT;
    }
}

size_t tiny_utf8_decode(tiny_utf8_rune* out_rune, const char* buff, size_t n) {
    size_t size = 0;

    char first = buff[0];
    char firstMask = 0;
    if ((first & 0x80) == 0) {
        size = 1;
        firstMask = 0x7f;
    } else if ((first & 0xe0) == 0xc0) {
        size = 2;
        firstMask = 0x1f;
    } else if ((first & 0xf0) == 0xe0) {
        size = 3;
        firstMask = 0x0f;
    } else if ((first & 0xf8) == 0xf0) {
        size = 4;
        firstMask = 0x07;
    } else
        return TINY_UTF8_ERR_INVALID_CODE_POINT;

    if (n < size)
        return TINY_UTF8_ERR_NOT_ENOUGH_BYTES;

    tiny_utf8_rune r = first & firstMask;
    for (size_t i = 1; i < size; i++) {
        r <<= 6;
        r |= buff[i] & 0x3f;
    }

    *out_rune = r;
    return size;
}

#endif

#endif