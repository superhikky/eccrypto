#include "ecctest.h"
#include <iostream>
#include <CppUTest/CommandLineTestRunner.h>

namespace ecc {
    void dump_bits_to
        (std::ostream& os, const std::function<int()>& getBit)
    {
        std::ostringstream oss;
        int bit = 0;
        for (;;) {
            bool out = false;
            for (size_t i = 0; i < 8; ++i) {
                for (size_t j = 0; j < 4; ++j) {
                    bit = getBit();
                    if (bit < 0) break;
                    oss << bit;
                    out = true;
                }
                if (bit < 0) break;
                oss << ' ';
            }
            if (out) oss << std::endl;
            if (bit < 0) break;
        }
        os << oss.str();
    }

    void dump_bytes_to
        (std::ostream& os, const std::function<int()>& getByt)
    {
        std::ostringstream oss;
        int byt = 0;
        for (;;) {
            bool out = false;
            for (size_t i = 0; i < 16; ++i) {
                byt = getByt();
                if (byt < 0) break;
                oss <<
                    std::hex <<
                    std::setw(2) <<
                    std::setfill('0') <<
                    byt <<
                    ' ';
                out = true;
            }
            if (out) oss << std::endl;
            if (byt < 0) break;
        }
        os << oss.str();
    }
}

int main(int argc, char** argv) {
    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
    return RUN_ALL_TESTS(argc, argv);
}
