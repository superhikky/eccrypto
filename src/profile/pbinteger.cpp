#include "eccprofile.h"

namespace ecc {
    constexpr size_t BI_NUM_LOOS = 1000000;
    const big_int<20> lhs("425826231723888350446541592701409065913635568770");
    const big_int<20> rhs("2035201141629041078739914579573");

    TEST_GROUP(binteger) {};

    TEST(binteger, add) {
        for (size_t i = 0; i < BI_NUM_LOOS; ++i)
            volatile big_int<20> res = lhs + rhs;
    }

    TEST(binteger, subtract) {
        for (size_t i = 0; i < BI_NUM_LOOS; ++i)
            volatile big_int<20> res = lhs - rhs;
    }

    TEST(binteger, multiply) {
        for (size_t i = 0; i < BI_NUM_LOOS; ++i)
            volatile big_int<20> res = lhs * rhs;
    }

    TEST(binteger, divide) {
        for (size_t i = 0; i < BI_NUM_LOOS; ++i)
            volatile big_int<20> res = lhs / rhs;
    }

    TEST(binteger, modulo) {
        for (size_t i = 0; i < BI_NUM_LOOS; ++i)
            volatile big_int<20> res = lhs % rhs;
    }
}
