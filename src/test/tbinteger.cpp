#include "ecctest.h"

namespace ecc {
    TEST_GROUP(binteger) {};

    TEST(binteger, big_int_const) {
        {
            CHECK_EQUAL(20, big_int<20>::LOGICAL_SIZE);
            CHECK_EQUAL(4, big_int<20>::SIZE_OF_FIELD);
            CHECK_EQUAL(32, big_int<20>::LENGTH_OF_BITS_ON_FIELD);
            CHECK_EQUAL(0xffffffffull, big_int<20>::FIELD_MASK);
            CHECK_EQUAL(2, big_int<1>::NUMBER_OF_FIELDS);
            CHECK_EQUAL(11, big_int<20>::NUMBER_OF_FIELDS);
            CHECK_EQUAL(10, (big_int<20, true>::NUMBER_OF_FIELDS));
        }
    }

    TEST(binteger, big_int_new) {
        {
            auto a = big_int<20>();
        }
        {
            auto a = big_int<20>("1461501637330902918203684832716283019653785059327");
            auto b = big_int<20>(a);
            CHECK_EQUAL("1461501637330902918203684832716283019653785059327", describe(b));
        }
        {
            auto a = big_int<20>(-1000000000);
            CHECK_EQUAL("-1000000000", describe(a));
        }
        {
            auto a = big_int<20>(1000000000u);
            CHECK_EQUAL("1000000000", describe(a));
        }
        {
            auto a = big_int<20>(-10000000000000ll);
            CHECK_EQUAL("-10000000000000", describe(a));
        }
        {
            auto a = big_int<20>(10000000000000ull);
            CHECK_EQUAL("10000000000000", describe(a));
        }
        {
            auto a = big_int<20>("1461501637330902918203684832716283019653785059327");
            CHECK_EQUAL("1461501637330902918203684832716283019653785059327", describe(a));
        }
        {
            auto a = big_int<20>("ffffffffffffffffffffffffffffffff7fffffff", ios::hex);
            CHECK_EQUAL("ffffffffffffffffffffffffffffffff7fffffff", describe(hex, a));
        }
        {
            auto a = big_int<20>(string("1461501637330902918203684832716283019653785059327"));
            CHECK_EQUAL("1461501637330902918203684832716283019653785059327", describe(a));
        }
        {
            auto a = big_int<20>(string("ffffffffffffffffffffffffffffffff7fffffff"), ios::hex);
            CHECK_EQUAL("ffffffffffffffffffffffffffffffff7fffffff", describe(hex, a));
        }
        {
            auto str = string("1461501637330902918203684832716283019653785059327");
            auto a = big_int<20>(str.begin(), str.end());
            CHECK_EQUAL("1461501637330902918203684832716283019653785059327", describe(a));
        }
        {
            auto str = string("ffffffffffffffffffffffffffffffff7fffffff");
            auto a = big_int<20>(str.begin(), str.end(), ios::hex);
            CHECK_EQUAL("ffffffffffffffffffffffffffffffff7fffffff", describe(hex, a));
        }
    }

    TEST(binteger, big_int_bitwise_and) {
        {
            auto a = big_int<20>("aaaaaaaaaaaaaaaaaaaa", ios::hex);
            auto b = big_int<20>("33333333333333333333", ios::hex);
            auto c = a & b;
            CHECK_EQUAL("22222222222222222222", describe(hex, c));
        }
        {
            auto a = big_int<20>("aaaaaaaaaaaaaaaaaaaa", ios::hex);
            auto b = big_int<20>("33333333333333333333", ios::hex);
            a &= b;
            CHECK_EQUAL("22222222222222222222", describe(hex, a));
        }
    }

    TEST(binteger, big_int_to_bool) {
        {
            auto a = big_int<20>(0);
            CHECK_FALSE(bool(a));
        }
        {
            auto a = big_int<20>(1);
            CHECK(bool(a));
        }
    }

    TEST(binteger, big_int_to_int) {
        {
            auto a = big_int<20>(-1000000000);
            CHECK_EQUAL(-1000000000, int(a));
        }
    }

    TEST(binteger, big_int_to_int64_t) {
        {
            auto a = big_int<20>(-10000000000000ll);
            CHECK_EQUAL(-10000000000000ll, int64_t(a));
        }
    }

    TEST(binteger, big_int_to_uint64_t) {
        {
            auto a = big_int<20>(10000000000000ull);
            CHECK_EQUAL(10000000000000ull, uint64_t(a));
        }
    }

    TEST(binteger, big_int_to_unsigned) {
        {
            auto a = big_int<20>(1000000000u);
            CHECK_EQUAL(1000000000u, unsigned(a));
        }
    }

    TEST(binteger, big_int_modulo) {
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = big_int<20>("2035201141629041078739914579573");
            auto c = a % b;
            CHECK_EQUAL("624949790712844072763264461216", describe(c));
        }
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = big_int<20>("2035201141629041078739914579573");
            a %= b;
            CHECK_EQUAL("624949790712844072763264461216", describe(a));
        }
    }

    TEST(binteger, big_int_multiply) {
        {
            auto a = big_int<20>("425826231723888350446541592701");
            auto b = big_int<20>(1000);
            auto c = a * b;
            CHECK_EQUAL("425826231723888350446541592701000", describe(c));
        }
        {
            auto a = big_int<20>("425826231723888350446541592701");
            auto b = big_int<20>(1000);
            a *= b;
            CHECK_EQUAL("425826231723888350446541592701000", describe(a));
        }
    }

    TEST(binteger, big_int_add) {
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = big_int<20>("203520114162904107873991457957346892027982641970");
            auto c = a + b;
            CHECK_EQUAL("629346345886792458320533050658755957941618210740", describe(c));
        }
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = big_int<20>("203520114162904107873991457957346892027982641970");
            a += b;
            CHECK_EQUAL("629346345886792458320533050658755957941618210740", describe(a));
        }
    }

    TEST(binteger, big_int_negate) {
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = -a;
            CHECK_EQUAL("-425826231723888350446541592701409065913635568770", describe(b));
        }
    }

    TEST(binteger, big_int_subtract) {
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = big_int<20>("203520114162904107873991457957346892027982641970");
            auto c = a - b;
            CHECK_EQUAL("222306117560984242572550134744062173885652926800", describe(c));
        }
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = big_int<20>("203520114162904107873991457957346892027982641970");
            a -= b;
            CHECK_EQUAL("222306117560984242572550134744062173885652926800", describe(a));
        }
    }

    TEST(binteger, big_int_divide) {
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = big_int<20>("2035201141629041078739914579573");
            auto c = a / b;
            CHECK_EQUAL("209230538944687898", describe(c));
        }
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = big_int<20>("2035201141629041078739914579573");
            a /= b;
            CHECK_EQUAL("209230538944687898", describe(a));
        }
    }

    TEST(binteger, big_int_less) {
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = big_int<20>("203520114162904107873991457957346892027982641970");
            CHECK_FALSE(a < a);
            CHECK_FALSE(a < b);
            CHECK(b < a);
            CHECK(a <= a);
            CHECK_FALSE(a <= b);
            CHECK(b <= a);
        }
    }

    TEST(binteger, big_int_shift_left) {
        {
            auto a = big_int<20>("3333333333333333333333333333333", ios::hex);
            auto b = a << 10;
            CHECK_EQUAL("ccccccccccccccccccccccccccccccc00", describe(hex, b));
        }
        {
            auto a = big_int<20>("3333333333333333333333333333333", ios::hex);
            auto b = a << -10;
            CHECK_EQUAL("cccccccccccccccccccccccccccc", describe(hex, b));
        }
        {
            auto a = big_int<20>("3333333333333333333333333333333", ios::hex);
            a <<= 10;
            CHECK_EQUAL("ccccccccccccccccccccccccccccccc00", describe(hex, a));
        }
    }

    TEST(binteger, big_int_assign) {
        {
            auto a = big_int<20>("1461501637330902918203684832716283019653785059327");
            big_int<20> b;
            b = a;
            CHECK_EQUAL("1461501637330902918203684832716283019653785059327", describe(a));
        }
    }

    TEST(binteger, big_int_equal) {
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = big_int<20>("203520114162904107873991457957346892027982641970");
            CHECK(a == a);
            CHECK_FALSE(a == b);
            CHECK_FALSE(b == a);
            CHECK_FALSE(a != a);
            CHECK(a != b);
            CHECK(b != a);
        }
    }

    TEST(binteger, big_int_greater) {
        {
            auto a = big_int<20>("425826231723888350446541592701409065913635568770");
            auto b = big_int<20>("203520114162904107873991457957346892027982641970");
            CHECK_FALSE(a > a);
            CHECK(a > b);
            CHECK_FALSE(b > a);
            CHECK(a >= a);
            CHECK(a >= b);
            CHECK_FALSE(b >= a);
        }
    }

    TEST(binteger, big_int_shift_right) {
        {
            auto a = big_int<20>("3333333333333333333333333333333", ios::hex);
            auto b = a >> 10;
            CHECK_EQUAL("cccccccccccccccccccccccccccc", describe(hex, b));
        }
        {
            auto a = big_int<20>("3333333333333333333333333333333", ios::hex);
            auto b = a >> -10;
            CHECK_EQUAL("ccccccccccccccccccccccccccccccc00", describe(hex, b));
        }
        {
            auto a = big_int<20>("3333333333333333333333333333333", ios::hex);
            a >>= 10;
            CHECK_EQUAL("cccccccccccccccccccccccccccc", describe(hex, a));
        }
    }

    TEST(binteger, big_int_at) {
        {
            auto a = big_int<20>("123456789abcdef0", ios::hex);
            CHECK_EQUAL(0x9abcdef0ull, a[0]);
            CHECK_EQUAL(0x12345678ull, a[1]);
            a[0] = 0x0fedcba9ull;
            a[1] = 0x87654321ull;
            CHECK_EQUAL(0x0fedcba9ull, a[0]);
            CHECK_EQUAL(0x87654321ull, a[1]);
            const auto b = big_int<20>("123456789abcdef0", ios::hex);
            CHECK_EQUAL(0x9abcdef0ull, b[0]);
            CHECK_EQUAL(0x12345678ull, b[1]);
        }
    }

    TEST(binteger, big_int_bitwise_xor) {
        {
            auto a = big_int<20>("aaaaaaaaaaaaaaaaaaaa", ios::hex);
            auto b = big_int<20>("33333333333333333333", ios::hex);
            auto c = a ^ b;
            CHECK_EQUAL("99999999999999999999", describe(hex, c));
        }
        {
            auto a = big_int<20>("aaaaaaaaaaaaaaaaaaaa", ios::hex);
            auto b = big_int<20>("33333333333333333333", ios::hex);
            a ^= b;
            CHECK_EQUAL("99999999999999999999", describe(hex, a));
        }
    }

    TEST(binteger, big_int_bitwise_or) {
        {
            auto a = big_int<20>("aaaaaaaaaaaaaaaaaaaa", ios::hex);
            auto b = big_int<20>("33333333333333333333", ios::hex);
            auto c = a | b;
            CHECK_EQUAL("bbbbbbbbbbbbbbbbbbbb", describe(hex, c));
        }
        {
            auto a = big_int<20>("aaaaaaaaaaaaaaaaaaaa", ios::hex);
            auto b = big_int<20>("33333333333333333333", ios::hex);
            a |= b;
            CHECK_EQUAL("bbbbbbbbbbbbbbbbbbbb", describe(hex, a));
        }
    }

    TEST(binteger, big_int_bitwise_not) {
        {
            auto a = big_int<4, true>("aaaaaaaaaaaaaaaa", ios::hex);
            auto b = ~a;
            CHECK_EQUAL("5555555555555555", describe(hex, b));
        }
    }
}
