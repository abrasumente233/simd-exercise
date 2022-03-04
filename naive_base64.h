#ifndef NAIVE_BASE64
#define NAIVE_BASE64

#include <stdint.h>

namespace naive_base64 {

static const char *lookup =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void encode(const uint8_t *input, size_t bytes, uint8_t *output) {
    size_t lim = bytes - bytes % 3;

    size_t i, j;
    for (i = 0, j = 0; i < lim; i += 3, j += 4) {

        // load three bytes into an uint32_t
        uint32_t t = (input[i] << 16) | (input[i + 1] << 8) | (input[i + 2]);

        uint8_t i3 = t & 0x3f, i2 = (t >> 6) & 0x3f, i1 = (t >> 12) & 0x3f,
                i0 = (t >> 18) & 0x3f;

        output[j]     = lookup[i0];
        output[j + 1] = lookup[i1];
        output[j + 2] = lookup[i2];
        output[j + 3] = lookup[i3];
    }

    int remain = bytes - i;

    // @FIXME: This looks really stupid, performance-wise.
    if (remain == 1) {

        // load one byte into an uint32_t
        uint32_t t = (input[i] << 4);

        uint8_t i0 = (t >> 6);
        uint8_t i1 = t & 0x3f;

        output[j]     = lookup[i0];
        output[j + 1] = lookup[i1];
        output[j + 2] = '=';
        output[j + 3] = '=';

        j += 4;
    } else if (remain == 2) {

        // load two bytes into an uint32_t
        uint32_t t = (input[i] << 10) | (input[i + 1] << 2);

        uint8_t i0 = (t >> 12);
        uint8_t i1 = (t >> 6) & 0x3f;
        uint8_t i2 = t & 0x3f;

        output[j]     = lookup[i0];
        output[j + 1] = lookup[i1];
        output[j + 2] = lookup[i2];
        output[j + 3] = '=';

        j += 4;
    }

    output[j] = '\0';
}

void decode(const uint8_t *input, size_t bytes, uint8_t *output) {}

} // namespace naive_base64

#endif
