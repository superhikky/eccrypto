#include "ecctest.h"

namespace ecc {
    TEST_GROUP(random) {};

    TEST(random, RandomNumberGenerator) {
        {
            struct MocRBG {
                bool bit;

                MocRBG() : bit(false) {}

                bool getBit() {
                    bit = !bit;
                    return bit;
                }
            };
            auto rng = make_shared<RandomNumberGenerator<uint16_t, MocRBG>>();
            CHECK_EQUAL(0xaaaa, rng->getNumber(0, 0xffff));
            CHECK_EQUAL(0x00aa, rng->getNumber(0, 0x00ff));
        }
    }
}
