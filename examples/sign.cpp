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

    if (argc != 3) {
        cerr << "usage: sign privateKey messageFile" << endl;
        return 1;
    }
    const auto priKey = string(argv[1]);
    const auto mesFil = string(argv[2]);

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
    const auto priKey_nums = ascTexToNums(priKey);
    if (priKey_nums.size() != 1) {
        cerr << "error: invalid privateKey" << endl;
        return 1;
    }
    const auto d = ge(priKey_nums.at(0));

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

    const auto rng = make_shared<RandomNumberGenerator<V>>();
    element<V> r, u;
    rational_point<V> U;
    do {
        r = ge(rng->getNumber(V(1), G->order() - V(1)));
        U = (r * B).project();
        u = (h + ge(U.x()) * d) / r;
    } while (!u);

    const auto numsToAscTex = [] (const vector<V>& nums) -> string {
        string ascTex;
        concatinate(
            make_shared<InputProcess<V>>
                (input_from(make_getter_from_range<V>(nums.begin(), nums.end()))),
            make_shared<InputOutputProcess<V, int>>
                (numbers_to_bytes<V>()),
            make_shared<InputOutputProcess<int, bool>>
                (bytes_to_bits()),
            make_shared<InputOutputProcess<bool, int>>
                (base64_encode()),
            make_shared<OutputProcess<int>>
                (output_to(make_putter_from_iterator<int>(back_inserter(ascTex))))
        )->run();
        return ascTex;
    };
    cout << numsToAscTex(vector<V>({U.x().value(), U.y().value(), u.value()}));

    return 0;
}
