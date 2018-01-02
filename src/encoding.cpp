#include "eccrypto.h"
#include <cstddef>
#include <cstdint>
#include <deque>

namespace ecc {
    byte_to_bit base64_decode() {
        static const int POTS[128] = {
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x40, 0x40, 0x3e, 0x40, 0x40, 0x40, 0x3f,
            0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
            0x3c, 0x3d, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
            0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
            0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
            0x17, 0x18, 0x19, 0x40, 0x40, 0x40, 0x40, 0x40,
            0x40, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
            0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
            0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
            0x31, 0x32, 0x33, 0x40, 0x40, 0x40, 0x40, 0x40,
        };
        return [] (const get_byte& getLet, const put_bit& putBit) {
            for (;;) {
                const int let = getLet();
                if (let < 128 && POTS[let] < 0x40) {
                    for (size_t i = 0; i < 6; ++i)
                        putBit((POTS[let] >> (6 - i - 1)) & 1);
                }
            }
        };
    }

    bit_to_byte base64_encode() {
        static const int LETS[64] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
            'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
            'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '0', '1', '2', '3',
            '4', '5', '6', '7', '8', '9', '+', '/',
        };
        return [] (const get_bit& getBit, const put_byte& putLet) {
            for (;;) {
                for (size_t i = 0; i < 4; ++i) {
                    int pot = 0;
                    size_t j;
                    for (j = 0; j < 6; ++j) {
                        bool bit = false;
                        try {
                            bit = getBit();
                        } catch (const eof_exception& eofExc) {
                            if (j == 0) {
                                if (i == 0) throw eofExc;
                                putLet('=');
                                break;
                            }
                        }
                        pot = (pot << 1) | bit;
                    }
                    if (j > 0) putLet(LETS[pot]);
                }
            }
        };
    }

    byte_to_byte pad_message(const size_t& sizBlo) {
        return [sizBlo] (const get_byte& getByt, const put_byte& putByt) {
            uint64_t siz;
            for (siz = 0;; ++siz) {
                try {
                    putByt(getByt());
                } catch (const eof_exception& eofExc) {
                    break;
                }
            }
            const size_t rem = size_t(siz % (uint64_t)sizBlo);
            size_t sizPad = sizBlo - rem;
            if (sizPad <= 8) sizPad += sizBlo;
            const size_t sizFil = sizPad - (1 + 8);
            putByt(0x80);
            for (size_t i = 0; i < sizFil; ++i) putByt(0x00);
            const uint64_t len = siz * 8;
            for (size_t i = 0; i < 8; ++i)
                putByt(int(len >> (8 * (8 - i - 1))) & 0xff);
        };
    }

    byte_to_byte strip_message(const size_t& sizBlo) {
        return [sizBlo] (const get_byte& getByt, const put_byte& putByt) {
            std::deque<int> buf;
            uint64_t siz = 0;
            for (;;) {
                try {
                    buf.push_back(getByt());
                    if (buf.size() > sizBlo + 8) {
                        putByt(buf.front());
                        buf.pop_front();
                        ++siz;
                    }
                } catch (const eof_exception& eofExc) {
                    break;
                }
            }
            if (buf.size() > 8) {
                auto ite = buf.rbegin();
                uint64_t len = 0;
                for (size_t i = 0; i < 8; ++i)
                    len |= (uint64_t)(*ite++ << (8 * i));
                const uint64_t sizMes = len / 8;
                if (sizMes > siz) {
                    size_t rem = size_t(sizMes - siz);
                    for (size_t i = 0; i < rem; ++i) {
                        if (buf.empty()) break;
                        putByt(buf.front());
                        buf.pop_front();
                    }
                }
            }
        };
    }
}
