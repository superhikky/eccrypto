#include "eccrypto.h"
#include <cstddef>
#include <cstdint>

namespace ecc {
    uint32_t ch(const uint32_t& x, const uint32_t& y, const uint32_t& z) {
        return (x & y) ^ (~x & z);
    }

    uint32_t lcsig0(const uint32_t& x) {
        return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
    }

    uint32_t lcsig1(const uint32_t& x) {
        return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
    }

    uint32_t maj(const uint32_t& x, const uint32_t& y, const uint32_t& z) {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    uint32_t parity
        (const uint32_t& x, const uint32_t& y, const uint32_t& z)
    {
        return x ^ y ^ z;
    }

    uint32_t rotl(const uint32_t& x, const size_t& len) {
        return (x << len) | (x >> (32 - len));
    }

    uint32_t rotr(const uint32_t& x, const size_t& len) {
        return (x >> len) | (x << (32 - len));
    }

    number_to_number<uint32_t, uint32_t> sha1() {
        static uint32_t (*const FUNS[4])
            (const uint32_t& x, const uint32_t& y, const uint32_t& z) =
                {ch, parity, maj, parity};
        static const uint32_t K[4] = 
            {0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6};
        return [] (
            const get_number<uint32_t>& getMesVal,
            const put_number<uint32_t>& putHasVal
        ) {
            uint32_t h[5] = {
                0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0
            };
            for (;;) {
                uint32_t w[16];
                try {
                    for (size_t i = 0; i < 16; ++i) w[i] = getMesVal();
                } catch (const eof_exception& eofExc) {
                    break;
                }
                uint32_t
                    a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
                for (size_t i = 0; i < 80; ++i) {
                    const size_t s = i % 16;
                    if (i >= 16)
                        w[s] = rotl(
                            w[(s + 13) % 16] ^
                            w[(s + 8) % 16] ^
                            w[(s + 2) % 16] ^
                            w[s],
                            1
                        );
                    const uint32_t
                        t =
                            rotl(a, 5) +
                            FUNS[i / 20](b, c, d) +
                            e +
                            K[i / 20] +
                            w[s];
                    e = d;
                    d = c;
                    c = rotl(b, 30);
                    b = a;
                    a = t;
                }
                h[0] += a;
                h[1] += b;
                h[2] += c;
                h[3] += d;
                h[4] += e;
            }
            for (size_t i = 0; i < 5; ++i) putHasVal(h[i]);
        };
    }

    number_to_number<uint32_t, uint32_t> sha256() {
        static const uint32_t K[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
            0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
            0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
            0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
            0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
            0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
            0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
            0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
            0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
        };
        return [] (
            const get_number<uint32_t>& getMesVal,
            const put_number<uint32_t>& putHasVal
        ) {
            uint32_t h[8] = {
                0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
            };
            for (;;) {
                uint32_t w[16];
                try {
                    for (size_t i = 0; i < 16; ++i) w[i] = getMesVal();
                } catch (const eof_exception& eofExc) {
                    break;
                }
                uint32_t
                    a = h[0], b = h[1], c = h[2], d = h[3],
                    e = h[4], f = h[5], g = h[6], H = h[7];
                for (size_t i = 0; i < 64; ++i) {
                    const size_t s = i % 16;
                    if (i >= 16)
                        w[s] =
                            ucsig1(w[(s + 14) % 16]) +
                            w[(s + 9) % 16] +
                            ucsig0(w[(s + 1) % 16]) +
                            w[s];
                    const uint32_t
                        t1 =
                            H +
                            lcsig1(e) +
                            ch(e, f, g) +
                            K[i] +
                            w[s],
                        t2 = lcsig0(a) + maj(a, b, c);
                    H = g;
                    g = f;
                    f = e;
                    e = d + t1;
                    d = c;
                    c = b;
                    b = a;
                    a = t1 + t2;
                }
                h[0] += a;
                h[1] += b;
                h[2] += c;
                h[3] += d;
                h[4] += e;
                h[5] += f;
                h[6] += g;
                h[7] += H;
            }
            for (size_t i = 0; i < 8; ++i) putHasVal(h[i]);
        };
    }

    uint32_t ucsig0(const uint32_t& x) {
        return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
    }

    uint32_t ucsig1(const uint32_t& x) {
        return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
    }
}
