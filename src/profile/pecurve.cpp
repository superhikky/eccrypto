#include "eccprofile.h"

namespace ecc {
    const auto p = big_int<20>("1461501637330902918203684832716283019653785059327");
    const auto F = make_shared<PrimeField<big_int<20>>>(p);
    const auto fe = make_factory<element<big_int<20>>>(F.get());
    const auto a = fe(big_int<20>("-3"));
    const auto b = fe(big_int<20>("163235791306168110546604919403271579530548345413"));
    const auto E = make_shared<EllipticCurve<big_int<20>>>(F.get(), a, b);
    const auto rp = make_factory<rational_point<big_int<20>>>(E.get());
    const auto B = rp(
        fe(big_int<20>("425826231723888350446541592701409065913635568770")),
        fe(big_int<20>("203520114162904107873991457957346892027982641970"))
    );
    const auto l = big_int<20>("1461501637330902918203687197606826779884643492439");
    const auto G = make_shared<PrimeField<big_int<20>>>(l);
    const auto ge = make_factory<element<big_int<20>>>(G.get());

    TEST_GROUP(ecurve) {};

    TEST(ecurve, scalar_multiply) {
        constexpr size_t NUMBER_OF_LOOPS = 100;
        const auto d = ge(big_int<20>("100000000000000000000000000000000000000000000000"));
        for (size_t i = 0; i < NUMBER_OF_LOOPS; ++i)
            volatile const auto P = d * B;
    }
}
