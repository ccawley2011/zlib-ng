/* chunkset.c -- inline functions to copy small data chunks.
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "zbuild.h"

typedef uint64_t chunk_t;

#define CHUNK_SIZE 8
#if !defined(UNALIGNED64_OK)
#  if !defined(UNALIGNED_OK)
#    define CHUNK_ALIGN 8
#  else
#    define CHUNK_ALIGN 4
#  endif
#else
#  define CHUNK_ALIGN 0
#endif

#define HAVE_CHUNKMEMSET_4

static inline void chunkmemset_4(uint8_t *from, chunk_t *chunk) {
    uint8_t *dest = (uint8_t *)chunk;
    memcpy(dest, from, sizeof(uint32_t));
    memcpy(dest+4, from, sizeof(uint32_t));
}

static inline void loadchunk(uint8_t const *s, chunk_t *chunk) {
    memcpy(chunk, (uint8_t *)s, sizeof(uint64_t));
}

static inline void storechunk(uint8_t *out, chunk_t *chunk) {
    memcpy(out, chunk, sizeof(uint64_t));
}

static inline void loadchunk_aligned(uint8_t const *s, chunk_t *chunk) {
    *chunk = *(chunk_t *)s;
}

static inline void storechunk_aligned(uint8_t *out, chunk_t *chunk) {
    *((chunk_t *)out) = *chunk;
}

#define CHUNKSIZE        chunksize_c
#define CHUNKCOPY        chunkcopy_c
#define CHUNKUNROLL      chunkunroll_c
#define CHUNKMEMSET      chunkmemset_c
#define CHUNKMEMSET_SAFE chunkmemset_safe_c

#include "chunkset_tpl.h"

#define INFLATE_FAST     inflate_fast_c

#include "inffast_tpl.h"
