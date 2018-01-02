#include "eccrypto.h"
#include <cstddef>
#include <ios>

namespace ecc {
    int base_to_radix(const std::ios::fmtflags& bas) {
        int rad;
        if (bas & std::ios::dec) rad = 10;
        else if (bas & std::ios::hex) rad = 16;
        else if (bas & std::ios::oct) rad = 8;
        return rad;
    }

    int bits_to_byte(const get_bit& getBit) {
        int byt = 0;
        for (size_t i = 0; i < 8; ++i) byt = (byt << 1) | getBit();
        return byt;
    }

    bit_to_byte bits_to_bytes() {
        return [] (const get_bit& getBit, const put_byte& putByt) {
            for (;;) putByt(bits_to_byte(getBit));
        };
    }

    void byte_to_bits(const int& byt, const put_bit& putBit) {
        for (size_t i = 0; i < 8; ++i) putBit((byt >> (8 - i - 1)) & 1);
    }

    byte_to_bit bytes_to_bits() {
        return [] (const get_byte& getByt, const put_bit& putBit) {
            for (;;) byte_to_bits(getByt(), putBit);
        };
    }

    int letter_to_digit(const int& let) {
        int dig = 0;
        if (let >= 'A' && let <= 'F') dig = 10 + let - 'A';
        else if(let >= 'a' && let <= 'f') dig = 10 + let - 'a';
        else if(let >= '0' && let <= '9') dig = let - '0';
        return dig;
    }

    get_byte make_getter_from_istream(std::istream& is) {
        return [&is] () -> int {
            const int byt = is.get();
            if (byt < 0) throw eof_exception();
            return byt;
        };
    }

    put_byte make_putter_from_ostream(std::ostream& os) {
        return [&os] (const int& byt) {
            os.put(byt);
        };
    }
}
