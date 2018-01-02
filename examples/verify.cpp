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
        cerr << "usage: verify publicKey signature messageFile" << endl;
        return 1;
    }
    const auto pubKey = string(argv[1]);
    const auto signature = string(argv[2]);
    const auto mesFil = string(argv[3]);

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
        cerr << "error: invalid publicKey" << endl;
        return 1;
    }
    const auto P = rp(fe(pubKey_nums.at(0)), fe(pubKey_nums.at(1)));
    const auto signature_nums = ascTexToNums(signature);
    if (signature_nums.size() != 3) {
        cerr << "error: invalid signature" << endl;
        return 1;
    }
    const auto U = rp(fe(signature_nums.at(0)), fe(signature_nums.at(1)));
    const auto u = ge(signature_nums.at(2));

    ifstream mesIS(mesFil, ios::in | ios::binary);
    if (!mesIS) {
        cerr << "error: failed to open messageFile" << endl;
        return 1;
    }

    auto h = ge(V(0));
    concatinate(
        make_shared<InputProcess<int>>
            (input_from(make_getter_from_istream(mesIS))),
        make_shared<InputOutputProcess<int, int>>
            (pad_message(64)),
        make_shared<InputOutputProcess<int, uint32_t>>
            (bytes_to_numbers<uint32_t>()),
        make_shared<InputOutputProcess<uint32_t, uint32_t>>
            (sha256()),
        make_shared<InputOutputProcess<uint32_t, int>>
            (numbers_to_bytes<uint32_t>()),
        make_shared<InputOutputProcess<int, V>>
            (bytes_to_numbers<V, 16>()),
        make_shared<InputOutputProcess<V, element<V>>>
            (transform<V, element<V>>(ge)),
        make_shared<OutputProcess<element<V>>>
            (output_to(make_putter_from_sum<element<V>>(&h)))
    )->run();

    const auto w = ge(V(1)) / u;
    const auto W = (h * w * B + ge(U.x()) * w * P).project();
    if (U == W) cout << "OK" << endl;
    else cout << "NG" << endl;

    return 0;
}
