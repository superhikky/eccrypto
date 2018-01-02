#include "ecctest.h"

namespace ecc {
    TEST_GROUP(hash) {};

    TEST(hash, sha1) {
        {
            string str = "The quick brown fox jumps over the lazy dog";
            vector<uint32_t> vals;
            concatinate(
                make_shared<InputProcess<int>>
                    (input_from(make_getter_from_range<int>(str.begin(), str.end()))),
                make_shared<InputOutputProcess<int, int>>
                    (pad_message(64)),
                make_shared<InputOutputProcess<int, uint32_t>>
                    (bytes_to_numbers<uint32_t>()),
                make_shared<InputOutputProcess<uint32_t, uint32_t>>
                    (sha1()),
                make_shared<OutputProcess<uint32_t>>
                    (output_to(make_putter_from_iterator<uint32_t>(back_inserter(vals))))
            )->run();
            CHECK_EQUAL(5, vals.size());
            CHECK_EQUAL(0x2fd4e1c6ul, vals.at(0));
            CHECK_EQUAL(0x7a2d28fcul, vals.at(1));
            CHECK_EQUAL(0xed849ee1ul, vals.at(2));
            CHECK_EQUAL(0xbb76e739ul, vals.at(3));
            CHECK_EQUAL(0x1b93eb12ul, vals.at(4));
        }
        {
            string str = "The quick brown fox jumps over the lazy cog";
            vector<uint32_t> vals;
            concatinate(
                make_shared<InputProcess<int>>
                    (input_from(make_getter_from_range<int>(str.begin(), str.end()))),
                make_shared<InputOutputProcess<int, int>>
                    (pad_message(64)),
                make_shared<InputOutputProcess<int, uint32_t>>
                    (bytes_to_numbers<uint32_t>()),
                make_shared<InputOutputProcess<uint32_t, uint32_t>>
                    (sha1()),
                make_shared<OutputProcess<uint32_t>>
                    (output_to(make_putter_from_iterator<uint32_t>(back_inserter(vals))))
            )->run();
            CHECK_EQUAL(5, vals.size());
            CHECK_EQUAL(0xde9f2c7ful, vals.at(0));
            CHECK_EQUAL(0xd25e1b3aul, vals.at(1));
            CHECK_EQUAL(0xfad3e85aul, vals.at(2));
            CHECK_EQUAL(0x0bd17d9bul, vals.at(3));
            CHECK_EQUAL(0x100db4b3ul, vals.at(4));
        }
    }

    TEST(hash, sha256) {
        {
            string str = "The quick brown fox jumps over the lazy dog";
            vector<uint32_t> vals;
            concatinate(
                make_shared<InputProcess<int>>
                    (input_from(make_getter_from_range<int>(str.begin(), str.end()))),
                make_shared<InputOutputProcess<int, int>>
                    (pad_message(64)),
                make_shared<InputOutputProcess<int, uint32_t>>
                    (bytes_to_numbers<uint32_t>()),
                make_shared<InputOutputProcess<uint32_t, uint32_t>>
                    (sha256()),
                make_shared<OutputProcess<uint32_t>>
                    (output_to(make_putter_from_iterator<uint32_t>(back_inserter(vals))))
            )->run();
            CHECK_EQUAL(8, vals.size());
            CHECK_EQUAL(0XD7A8FBB3UL, vals.at(0));
            CHECK_EQUAL(0X07D78094UL, vals.at(1));
            CHECK_EQUAL(0X69CA9ABCUL, vals.at(2));
            CHECK_EQUAL(0XB0082E4FUL, vals.at(3));
            CHECK_EQUAL(0X8D5651E4UL, vals.at(4));
            CHECK_EQUAL(0X6D3CDB76UL, vals.at(5));
            CHECK_EQUAL(0X2D02D0BFUL, vals.at(6));
            CHECK_EQUAL(0X37C9E592UL, vals.at(7));
        }
        {
            string str = "The quick brown fox jumps over the lazy cog";
            vector<uint32_t> vals;
            concatinate(
                make_shared<InputProcess<int>>
                    (input_from(make_getter_from_range<int>(str.begin(), str.end()))),
                make_shared<InputOutputProcess<int, int>>
                    (pad_message(64)),
                make_shared<InputOutputProcess<int, uint32_t>>
                    (bytes_to_numbers<uint32_t>()),
                make_shared<InputOutputProcess<uint32_t, uint32_t>>
                    (sha256()),
                make_shared<OutputProcess<uint32_t>>
                    (output_to(make_putter_from_iterator<uint32_t>(back_inserter(vals))))
            )->run();
            CHECK_EQUAL(8, vals.size());
            CHECK_EQUAL(0XE4C4D8F3UL, vals.at(0));
            CHECK_EQUAL(0XBF76B692UL, vals.at(1));
            CHECK_EQUAL(0XDE791A17UL, vals.at(2));
            CHECK_EQUAL(0X3E053211UL, vals.at(3));
            CHECK_EQUAL(0X50F7A345UL, vals.at(4));
            CHECK_EQUAL(0XB46484FEUL, vals.at(5));
            CHECK_EQUAL(0X427F6ACCUL, vals.at(6));
            CHECK_EQUAL(0X7ECC81BEUL, vals.at(7));
        }
    }
}
