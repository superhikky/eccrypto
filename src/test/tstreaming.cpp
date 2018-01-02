#include "ecctest.h"

namespace ecc {
    TEST_GROUP(streaming) {};

    TEST(streaming, streaming) {
        {
            vector<uint32_t> nums32({0x10203040ul, 0x50607080ul});
            vector<uint16_t> nums16;
            concatinate(
                make_shared<InputProcess<uint32_t>>
                    (input_from(make_getter_from_range<uint32_t>(nums32.begin(), nums32.end()))),
                make_shared<InputOutputProcess<uint32_t, int>>
                    (numbers_to_bytes<uint32_t>()),
                make_shared<InputOutputProcess<int, bool>>
                    (bytes_to_bits()),
                make_shared<InputOutputProcess<bool, int>>
                    (bits_to_bytes()),
                make_shared<InputOutputProcess<int, uint16_t>>
                    (bytes_to_numbers<uint16_t>()),
                make_shared<OutputProcess<uint16_t>>
                    (output_to(make_putter_from_iterator<uint16_t>(back_inserter(nums16))))
            )->run();
            CHECK_EQUAL(4, nums16.size());
            CHECK_EQUAL(0x1020, nums16.at(0));
            CHECK_EQUAL(0x3040, nums16.at(1));
            CHECK_EQUAL(0x5060, nums16.at(2));
            CHECK_EQUAL(0x7080, nums16.at(3));
        }
    }
}
