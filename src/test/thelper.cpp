#include "ecctest.h"

namespace ecc {
    TEST_GROUP(helper) {};

    TEST(helper, factory) {
        {
            auto F = make_shared<PrimeField<int>>(7);
            auto fe = factory<element<int>, PrimeField<int>>(F.get());
            auto e = fe(4);
            CHECK_EQUAL(4, e.value());
        }
    }

    TEST(helper, LOGICAL_SIZE_OF) {
        {
            CHECK_EQUAL(4, LOGICAL_SIZE_OF<int32_t>());
            CHECK_EQUAL(20, LOGICAL_SIZE_OF<big_int<20>>());
        }
    }

    TEST(helper, bytes_to_numbers) {
        {
            vector<int> byts({0x10, 0x20, 0x30, 0x40});
            vector<uint16_t> nums;
            try {
                bytes_to_numbers<uint16_t>()(
                    make_getter_from_range<int>(byts.begin(), byts.end()),
                    make_putter_from_iterator<uint16_t>(back_inserter(nums))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(2, nums.size());
            CHECK_EQUAL(0x1020, nums.at(0));
            CHECK_EQUAL(0x3040, nums.at(1));
        }
        {
            vector<int> byts({0x10, 0x20, 0x30, 0x40});
            vector<uint32_t> nums;
            try {
                bytes_to_numbers<uint32_t>()(
                    make_getter_from_range<int>(byts.begin(), byts.end()),
                    make_putter_from_iterator<uint32_t>(back_inserter(nums))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(1, nums.size());
            CHECK_EQUAL(0x10203040ul, nums.at(0));
        }
        {
            vector<int> byts({0x10, 0x20, 0x30, 0x40});
            vector<uint16_t> nums;
            try {
                bytes_to_numbers<uint16_t, 1>()(
                    make_getter_from_range<int>(byts.begin(), byts.end()),
                    make_putter_from_iterator<uint16_t>(back_inserter(nums))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(4, nums.size());
            CHECK_EQUAL(0x10, nums.at(0));
            CHECK_EQUAL(0x20, nums.at(1));
            CHECK_EQUAL(0x30, nums.at(2));
            CHECK_EQUAL(0x40, nums.at(3));
        }
        {
            vector<int> byts({0x10, 0x20, 0x30, 0x40});
            vector<uint16_t> nums;
            try {
                bytes_to_numbers<uint16_t, 2, false>()(
                    make_getter_from_range<int>(byts.begin(), byts.end()),
                    make_putter_from_iterator<uint16_t>(back_inserter(nums))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(2, nums.size());
            CHECK_EQUAL(0x2010, nums.at(0));
            CHECK_EQUAL(0x4030, nums.at(1));
        }
    }

    TEST(helper, make_factory) {
        {
            auto F = make_shared<PrimeField<int>>(7);
            auto fe = make_factory<element<int>>(F.get());
            auto e = fe(4);
            CHECK_EQUAL(4, e.value());
        }
    }

    TEST(helper, make_getter_from_range) {
        {
            vector<int> byts({0x10, 0x20, 0x30, 0x40});
            vector<uint16_t> nums;
            try {
                bytes_to_numbers<uint16_t>()(
                    make_getter_from_range<int>(byts.begin(), byts.end()),
                    make_putter_from_iterator<uint16_t>(back_inserter(nums))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(2, nums.size());
            CHECK_EQUAL(0x1020, nums.at(0));
            CHECK_EQUAL(0x3040, nums.at(1));
        }
        {
            vector<int> byts({0x10, 0x20, 0x30, 0x40});
            vector<uint16_t> nums;
            try {
                bytes_to_numbers<uint16_t>()(
                    make_getter_from_range<int>(byts.begin() + 1, byts.begin() + 3),
                    make_putter_from_iterator<uint16_t>(back_inserter(nums))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(1, nums.size());
            CHECK_EQUAL(0x2030, nums.at(0));
        }
    }

    TEST(helper, make_putter_from_iterator) {
        {
            vector<int> byts({0x10, 0x20, 0x30, 0x40});
            vector<uint16_t> nums;
            try {
                bytes_to_numbers<uint16_t>()(
                    make_getter_from_range<int>(byts.begin(), byts.end()),
                    make_putter_from_iterator<uint16_t>(back_inserter(nums))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(2, nums.size());
            CHECK_EQUAL(0x1020, nums.at(0));
            CHECK_EQUAL(0x3040, nums.at(1));
        }
    }

    TEST(helper, make_putter_from_sum) {
        {
            vector<int> byts({0x10, 0x20, 0x30, 0x40});
            int sum = 0;
            try {
                bytes_to_numbers<uint16_t>()(
                    make_getter_from_range<int>(byts.begin(), byts.end()),
                    make_putter_from_sum<uint16_t>(&sum)
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(0x4060, sum);
        }
    }

    TEST(helper, numbers_to_bytes) {
        {
            vector<uint16_t> nums({0x1020, 0x3040});
            vector<int> byts;
            try {
                numbers_to_bytes<uint16_t>()(
                    make_getter_from_range<uint16_t>(nums.begin(), nums.end()),
                    make_putter_from_iterator<int>(back_inserter(byts))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(4, byts.size());
            CHECK_EQUAL(0x10, byts.at(0));
            CHECK_EQUAL(0x20, byts.at(1));
            CHECK_EQUAL(0x30, byts.at(2));
            CHECK_EQUAL(0x40, byts.at(3));
        }
        {
            vector<uint32_t> nums({0x10203040});
            vector<int> byts;
            try {
                numbers_to_bytes<uint32_t>()(
                    make_getter_from_range<uint32_t>(nums.begin(), nums.end()),
                    make_putter_from_iterator<int>(back_inserter(byts))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(4, byts.size());
            CHECK_EQUAL(0x10, byts.at(0));
            CHECK_EQUAL(0x20, byts.at(1));
            CHECK_EQUAL(0x30, byts.at(2));
            CHECK_EQUAL(0x40, byts.at(3));
        }
        {
            vector<uint16_t> nums({0x1020, 0x3040});
            vector<int> byts;
            try {
                numbers_to_bytes<uint16_t, 1>()(
                    make_getter_from_range<uint16_t>(nums.begin(), nums.end()),
                    make_putter_from_iterator<int>(back_inserter(byts))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(2, byts.size());
            CHECK_EQUAL(0x20, byts.at(0));
            CHECK_EQUAL(0x40, byts.at(1));
        }
        {
            vector<uint16_t> nums({0x1020, 0x3040});
            vector<int> byts;
            try {
                numbers_to_bytes<uint16_t, 2, false>()(
                    make_getter_from_range<uint16_t>(nums.begin(), nums.end()),
                    make_putter_from_iterator<int>(back_inserter(byts))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(4, byts.size());
            CHECK_EQUAL(0x20, byts.at(0));
            CHECK_EQUAL(0x10, byts.at(1));
            CHECK_EQUAL(0x40, byts.at(2));
            CHECK_EQUAL(0x30, byts.at(3));
        }
    }

    TEST(helper, bits_to_bytes) {
        {
            vector<bool> bits({
                0, 1, 0, 1, 0, 1, 0, 1, 
                1, 0, 0, 1, 1, 0, 0, 1, 
            });
            vector<int> byts;
            try {
                bits_to_bytes()(
                    make_getter_from_range<bool>(bits.begin(), bits.end()),
                    make_putter_from_iterator<int>(back_inserter(byts))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(2, byts.size());
            CHECK_EQUAL(0x55, byts.at(0));
            CHECK_EQUAL(0x99, byts.at(1));
        }
    }

    TEST(helper, bytes_to_bits) {
        {
            vector<int> byts({0x55, 0x99});
            vector<bool> bits;
            try {
                bytes_to_bits()(
                    make_getter_from_range<int>(byts.begin(), byts.end()),
                    make_putter_from_iterator<bool>(back_inserter(bits))
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL(16, bits.size());
            CHECK_FALSE(bits.at(0));
            CHECK(bits.at(1));
            CHECK_FALSE(bits.at(2));
            CHECK(bits.at(3));
            CHECK_FALSE(bits.at(4));
            CHECK(bits.at(5));
            CHECK_FALSE(bits.at(6));
            CHECK(bits.at(7));
            CHECK(bits.at(8));
            CHECK_FALSE(bits.at(9));
            CHECK_FALSE(bits.at(10));
            CHECK(bits.at(11));
            CHECK(bits.at(12));
            CHECK_FALSE(bits.at(13));
            CHECK_FALSE(bits.at(14));
            CHECK(bits.at(15));
        }
    }

    TEST(helper, make_getter_from_istream) {
        {
            istringstream iss("abc");
            ostringstream oss;
            try {
                through<int>()(
                    make_getter_from_istream(iss),
                    make_putter_from_ostream(oss)
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL("abc", oss.str());
        }
    }

    TEST(helper, make_putter_from_ostream) {
        {
            istringstream iss("abc");
            ostringstream oss;
            try {
                through<int>()(
                    make_getter_from_istream(iss),
                    make_putter_from_ostream(oss)
                );
            } catch (eof_exception eofExc) {}
            CHECK_EQUAL("abc", oss.str());
        }
    }
}
