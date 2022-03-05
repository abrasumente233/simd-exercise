#ifndef NEON_BASE64
#define NEON_BASE64

#include <stdint.h>
#include <arm_neon.h> // ifdef __ARM_NEON

#include "naive_base64.h"

namespace neon_base64 {

static void dump_vector(uint8x16_t v) {
    printf("[");
#define L(_i) printf("0x%02x, ", vgetq_lane_u8(v, _i))
#define LL(_i) printf("0x%02x", vgetq_lane_u8(v, _i))
    L(0); L(1); L(2); L(3); L(4); L(5); L(6); L(7); L(8);
    L(9); L(10); L(11); L(12); L(13); L(14); LL(15);
    printf("]");
}

static inline uint8x16_t packed_mask(uint8_t x) {
    return vdupq_n_u8(x);
}

static const unsigned char *lut = (const unsigned char *)
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void encode(const uint8_t *input, size_t bytes, uint8_t *output) {
    
    size_t lim = bytes - bytes % (16 * 3);

    // @TODO: We should really do this only when lim >= 48,
    // if the overhead were too huge.
    uint8x16x4_t vlut = vld1q_u8_x4(lut);

    size_t i;
    for (i = 0; i < lim; i += 16 * 3) {

        uint8x16x3_t v = vld3q_u8(input + i);

        uint8x16_t v0 = vshrq_n_u8(v.val[0], 2);
        // @TODO: WHY DO WE NEED MASK 0x3f here???
        uint8x16_t v1 =
            vandq_u8(vorrq_u8(vshlq_n_u8(v.val[0], 4), vshrq_n_u8(v.val[1], 4)),
                     packed_mask(0x3f));
        uint8x16_t v2 =
            vandq_u8(vorrq_u8(vshlq_n_u8(v.val[1], 2), vshrq_n_u8(v.val[2], 6)),
                     packed_mask(0x3f));
        uint8x16_t v3 = vandq_u8(v.val[2], packed_mask(0x3f));

        uint8x16x4_t res;

        res.val[0] = vqtbl4q_u8(vlut, v0);
        res.val[1] = vqtbl4q_u8(vlut, v1);
        res.val[2] = vqtbl4q_u8(vlut, v2);
        res.val[3] = vqtbl4q_u8(vlut, v3);

        vst4q_u8(output, res);

        output += 64;
    }

    naive_base64::encode(input + i, bytes - i, output);
}

void decode(const uint8_t *input, size_t bytes, uint8_t *output) {}

} // namespace neon_base64

#endif
