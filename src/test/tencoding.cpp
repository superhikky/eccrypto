#include "ecctest.h"

namespace ecc {
    TEST_GROUP(encoding) {};

    TEST(encoding, base64_decode) {
        {
            string ascTex = "QUJDREVGRw==";
            string oriTex;
            concatinate(
                make_shared<InputProcess<int>>
                    (input_from(make_getter_from_range<int>(ascTex.begin(), ascTex.end()))),
                make_shared<InputOutputProcess<int, bool>>
                    (base64_decode()),
                make_shared<InputOutputProcess<bool, int>>
                    (bits_to_bytes()),
                make_shared<OutputProcess<int>>
                    (output_to(make_putter_from_iterator<int>(back_inserter(oriTex))))
            )->run();
            CHECK_EQUAL("ABCDEFG", oriTex);
        }
    }

    TEST(encoding, base64_encode) {
        {
            string oriTex = "ABCDEFG";
            string ascTex;
            concatinate(
                make_shared<InputProcess<int>>
                    (input_from(make_getter_from_range<int>(oriTex.begin(), oriTex.end()))),
                make_shared<InputOutputProcess<int, bool>>
                    (bytes_to_bits()),
                make_shared<InputOutputProcess<bool, int>>
                    (base64_encode()),
                make_shared<OutputProcess<int>>
                    (output_to(make_putter_from_iterator<int>(back_inserter(ascTex))))
            )->run();
            CHECK_EQUAL("QUJDREVGRw==", ascTex);
        }
    }

    TEST(encoding, pad_message) {
        {
            vector<int> mes({0x10, 0x20, 0x30, 0x40});
            vector<int> padMes;
            concatinate(
                make_shared<InputProcess<int>>
                    (input_from(make_getter_from_range<int>(mes.begin(), mes.end()))),
                make_shared<InputOutputProcess<int, int>>
                    (pad_message(16)),
                make_shared<OutputProcess<int>>
                    (output_to(make_putter_from_iterator<int>(back_inserter(padMes))))
            )->run();
            CHECK_EQUAL(16, padMes.size());
            CHECK_EQUAL(0x10, padMes.at(0));
            CHECK_EQUAL(0x20, padMes.at(1));
            CHECK_EQUAL(0x30, padMes.at(2));
            CHECK_EQUAL(0x40, padMes.at(3));
            CHECK_EQUAL(0x80, padMes.at(4));
            CHECK_EQUAL(0x00, padMes.at(5));
            CHECK_EQUAL(0x00, padMes.at(6));
            CHECK_EQUAL(0x00, padMes.at(7));
            CHECK_EQUAL(0x00, padMes.at(8));
            CHECK_EQUAL(0x00, padMes.at(9));
            CHECK_EQUAL(0x00, padMes.at(10));
            CHECK_EQUAL(0x00, padMes.at(11));
            CHECK_EQUAL(0x00, padMes.at(12));
            CHECK_EQUAL(0x00, padMes.at(13));
            CHECK_EQUAL(0x00, padMes.at(14));
            CHECK_EQUAL(0x20, padMes.at(15));
        }
    }

    TEST(encoding, strip_message) {
        {
            vector<int> padMes({
                0x10, 0x20, 0x30, 0x40,
                0x80, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x20,
            });
            vector<int> mes;
            concatinate(
                make_shared<InputProcess<int>>
                    (input_from(make_getter_from_range<int>(padMes.begin(), padMes.end()))),
                make_shared<InputOutputProcess<int, int>>
                    (strip_message(16)),
                make_shared<OutputProcess<int>>
                    (output_to(make_putter_from_iterator<int>(back_inserter(mes))))
            )->run();
            CHECK_EQUAL(4, mes.size());
            CHECK_EQUAL(0x10, mes.at(0));
            CHECK_EQUAL(0x20, mes.at(1));
            CHECK_EQUAL(0x30, mes.at(2));
            CHECK_EQUAL(0x40, mes.at(3));
        }
    }
}
