#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

// NOLINTBEGIN(readability-identifier-length, hicpp-signed-bitwise, readability-magic-numbers)
uint32_t murmurHash3(int count, ...) {
  va_list arguments;
  va_start(arguments, count);

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;
  uint32_t h = 0;

  for (int i = 0; i < count; ++i) {
    const void* data = va_arg(arguments, const void*);
    const size_t nbytes = va_arg(arguments, size_t);

    if (data == NULL || nbytes == 0) continue;

    const size_t nblocks = nbytes / 4;
    const uint32_t* blocks = (const uint32_t*)(data);
    const uint8_t* tail = (const uint8_t*)data + (nblocks * 4);

    uint32_t k;
    for (int i = 0; i < nblocks; i++) {
      k = blocks[i];

      k *= c1;
      k = (k << 15) | (k >> (32 - 15));
      k *= c2;

      h ^= k;
      h = (h << 13) | (h >> (32 - 13));
      h = (h * 5) + 0xe6546b64;
    }

    k = 0;
    switch (nbytes & 3) {
    case 3:
      k ^= tail[2] << 16;
    case 2:
      k ^= tail[1] << 8;
    case 1:
      k ^= tail[0];
      k *= c1;
      k = (k << 15) | (k >> (32 - 15));
      k *= c2;
      h ^= k;
    default:
      break;
    };

    h ^= nbytes;

    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
  }

  va_end(arguments);

  return h;
}
// NOLINTEND(readability-identifier-length, hicpp-signed-bitwise, readability-magic-numbers)
