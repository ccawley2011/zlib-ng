/* crc32_acle.c -- compute the CRC-32 of a data stream
 * Copyright (C) 1995-2006, 2010, 2011, 2012 Mark Adler
 * Copyright (C) 2016 Yang Zhang
 * For conditions of distribution and use, see copyright notice in zlib.h
 *
*/

#ifdef ARM_ACLE_CRC_HASH
#ifdef _MSC_VER
#  include <intrin.h>
#else
#  include <arm_acle.h>
#endif
#include "../../zbuild.h"

static uint32_t crc32_acle_z(uint32_t crc, const uint8_t *buf, size_t len) {
    Z_REGISTER uint32_t c;

    c = ~crc;
    if (len && ((ptrdiff_t)buf & 1)) {
        c = __crc32b(c, *buf++);
        len--;
    }

    if ((len >= sizeof(uint16_t)) && ((ptrdiff_t)buf & sizeof(uint16_t))) {
        c = __crc32h(c, *(const uint16_t *)buf);
        buf += sizeof(uint16_t);
        len -= sizeof(uint16_t);
    }

    if ((len >= sizeof(uint32_t)) && ((ptrdiff_t)buf & sizeof(uint32_t))) {
        c = __crc32w(c, *(const uint32_t *)buf);
        buf += sizeof(uint32_t);
        len -= sizeof(uint32_t);
    }

    if (len == 0) {
        c = ~c;
        return c;
    }

    while (len >= sizeof(uint64_t)) {
        c = __crc32d(c, *(const uint64_t *)buf);
        buf += sizeof(uint64_t);
        len -= sizeof(uint64_t);
    }

    if (len >= sizeof(uint32_t)) {
        c = __crc32w(c, *(const uint32_t *)buf);
        buf += sizeof(uint32_t);
        len -= sizeof(uint32_t);
    }

    if (len >= sizeof(uint16_t)) {
        c = __crc32h(c, *(const uint16_t *)buf);
        buf += sizeof(uint16_t);
        len -= sizeof(uint16_t);
    }

    if (len) {
        c = __crc32b(c, *buf);
    }

    c = ~c;
    return c;
}

uint32_t crc32_acle(uint32_t crc, const uint8_t * buf, uint64_t len) {
    while (len > SIZE_MAX) {
        crc = crc32_acle_z(crc, buf, SIZE_MAX);
        buf += SIZE_MAX;
        len -= SIZE_MAX;
    }
    if (len) {
        crc = crc32_acle_z(crc, buf, len);
    }
    return crc;
}
#endif
