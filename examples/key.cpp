#include <cstddef>
#include <eccrypto.h>
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

    if (argc != 1) {
        cerr << "usage: key" << endl;
        return 1;
    }

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

    const auto rng = make_shared<RandomNumberGenerator<V>>();
    const auto d = ge(rng->getNumber(V(1), G->order() - V(1)));
    const auto P = (d * B).project();

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
    cout << numsToAscTex(vector<V>({d.value()})) << endl;
    cout << numsToAscTex(vector<V>({P.x().value(), P.y().value()})) << endl;

    return 0;
}
