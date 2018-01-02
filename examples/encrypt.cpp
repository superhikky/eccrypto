#include <cstddef>
#include <cstdint>
#include <eccrypto.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    static constexpr size_t S = 20;
    using namespace ecc;
    using namespace std;
    using V = big_int<S>;

    if (argc != 4) {
        cerr << "usage: encrypt publicKey messageFile cipherFile" << endl;
        return 1;
    }
    const auto pubKey = string(argv[1]);
    const auto mesFil = string(argv[2]);
    const auto cipFil = string(argv[3]);

    const auto F = make_shared<PrimeField<V>>
        (V("1461501637330902918203684832716283019653785059327"));
    const auto fe = make_factory<element<V>>(F.get());
    const auto E = make_shared<EllipticCurve<V>>(
        F.get(),
        fe(V(-3)),
        fe(V("163235791306168110546604919403271579530548345413"))
    );

    const auto rp = make_factory<rational_point<V>>(E.get());
    const auto B = rp(
        fe(V("425826231723888350446541592701409065913635568770")),
        fe(V("203520114162904107873991457957346892027982641970"))
    );
    const auto G = make_shared<PrimeField<V>>
        (V("1461501637330902918203687197606826779884643492439"));
    const auto ge = make_factory<element<V>>(G.get());

    const auto ascTexToNums = [] (const string& ascTex) -> vector<V> {
        vector<V> nums;
        concatinate(
            make_shared<InputProcess<int>>
                (input_from(make_getter_from_range<int>(ascTex.begin(), ascTex.end()))),
            make_shared<InputOutputProcess<int, bool>>
                (base64_decode()),
            make_shared<InputOutputProcess<bool, int>>
                (bits_to_bytes()),
            make_shared<InputOutputProcess<int, V>>
                (bytes_to_numbers<V>()),
            make_shared<OutputProcess<V>>
                (output_to(make_putter_from_iterator<V>(back_inserter(nums))))
        )->run();
        return nums;
    };
    const auto pubKey_nums = ascTexToNums(pubKey);
    if (pubKey_nums.size() != 2) {
        cerr << "error: invalid pubricKey" << endl;
        return 1;
    }
    const auto P = rp(fe(pubKey_nums.at(0)), fe(pubKey_nums.at(1)));

    const auto rng = make_shared<RandomNumberGenerator<V>>();
    const auto r = ge(rng->getNumber(V(1), G->order() - V(1)));
    const auto R = (r * B).project();
    const auto K = (r * P).project();

    ifstream mesIS(mesFil, ios::in | ios::binary);
    if (!mesIS) {
        cerr << "error: failed to open messageFile" << endl;
        return 1;
    }
    ofstream cipOS(cipFil, ios::out | ios::binary);
    if (!cipOS) {
        cerr << "error: failed to open cipherFile" << endl;
        return 1;
    }

    concatinate(
        make_shared<InputProcess<int>>
            (input_from(make_getter_from_istream(mesIS))),
        make_shared<InputOutputProcess<int, int>>
            (pad_message(S - 1)),
        make_shared<InputOutputProcess<int, V>>
            (bytes_to_numbers<V, S - 1>()),
        make_shared<InputOutputProcess<V, V>>(
            [&R, &K, &fe] (
                const get_number<V>& getNum,
                const put_number<V>& putNum
            ) {
                putNum(R.x().value());
                putNum(R.y().value());
                for (;;) putNum((fe(getNum()) + K.x()).value());
            }
        ),
        make_shared<InputOutputProcess<V, int>>
            (numbers_to_bytes<V>()),
        make_shared<OutputProcess<int>>
            (output_to(make_putter_from_ostream(cipOS)))
    )->run();

    return 0;
}
