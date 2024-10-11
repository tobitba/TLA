#ifndef HASH_UTILS_H
#define HASH_UTILS_H

#include <stdint.h>

/**
 * Get 32-bit Murmur3 hash.
 *
 * @param `count`     number of properties to hash.
 * @param `...`       pairs of arguments of the form `void* data, size_t nbytes` where:
 *                      `data`      pointer to source data.
 *                      `nbytes`    size in bytes of data.
 *
 * @return 32-bit unsigned hash value.
 *
 * @code
 * Modified from https://github.com/wolkykim/qlibc/blob/main/src/utilities/qhash.c#L263
 * to accept variable arguments.
 * @endcode
 */
uint32_t murmurHash3(int count, ...);

#endif
