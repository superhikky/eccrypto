// ECDSA署名のサンプルプログラム

#include <cstdint>
#include <eccrypto.h>
#include <iostream>
#include <memory>
#include <vector>

int main(int argc, char** argv) {
    // 楕円曲線の定義
    auto F = std::make_shared<ecc::PrimeField<int>>(7);
    auto fe = ecc::make_factory<ecc::element<int>>(F.get());
    auto E = std::make_shared<ecc::EllipticCurve<int>>(F.get(), fe(2), fe(6));

    // 基準点の定義
    auto rp = ecc::make_factory<ecc::rational_point<int>>(E.get());
    auto B = rp(fe(1), fe(3));
    auto G = std::make_shared<ecc::PrimeField<int>>(11);
    auto ge = ecc::make_factory<ecc::element<int>>(G.get());

    // 鍵の生成
    auto rng = std::make_shared<ecc::RandomNumberGenerator<int>>();
    auto d_a = ge(rng->getNumber(1, G->order() - 1));
    auto P_a = d_a * B;

    // 平文の作成
    std::vector<ecc::element<int>> M;
    for (size_t i = 0; i < 3; ++i)
        M.push_back(fe(rng->getNumber(0, F->order() - 1)));

    // ハッシュ
    auto h = ge(0);
    ecc::concatinate(
        std::make_shared<ecc::InputProcess<int>>
            (ecc::input_from(ecc::make_getter_from_range<int>(M.begin(), M.end()))),
        std::make_shared<ecc::InputOutputProcess<int, int>>
            (ecc::numbers_to_bytes<int>()),
        std::make_shared<ecc::InputOutputProcess<int, int>>
            (ecc::pad_message(64)),
        std::make_shared<ecc::InputOutputProcess<int, uint32_t>>
            (ecc::bytes_to_numbers<uint32_t>()),
        std::make_shared<ecc::InputOutputProcess<uint32_t, uint32_t>>
            (ecc::sha1()),
        std::make_shared<ecc::InputOutputProcess<uint32_t, int>>
            (ecc::numbers_to_bytes<uint32_t>()),
        std::make_shared<ecc::InputOutputProcess<int, ecc::element<int>>>
            (ecc::transform<int, ecc::element<int>>(ge)),
        std::make_shared<ecc::OutputProcess<ecc::element<int>>>
            (ecc::output_to(ecc::make_putter_from_sum<ecc::element<int>>(&h)))
    )->run();

    // 署名
    ecc::element<int> r_a, u_a;
    ecc::rational_point<int> U_a;
    do {
        r_a = ge(rng->getNumber(1, G->order() - 1));
        U_a = (r_a * B).project();
        u_a = (h + ge(U_a.x()) * d_a) / r_a;
    } while (!u_a);

    // 検証
    auto u_b = ge(1) / u_a;
    auto U_b = (h * u_b * B + ge(U_a.x()) * u_b * P_a).project();

    // 結果の表示
    std::cout << "F->order(): " << F->order() << std::endl;
    std::cout << "E->coefficientA(): " << E->coefficientA() << std::endl;
    std::cout << "E->coefficientB(): " << E->coefficientB() << std::endl;
    std::cout << "B: " << B << std::endl;
    std::cout << "G->order(): " << G->order() << std::endl;
    std::cout << "d_a: " << d_a << std::endl;
    std::cout << "P_a: " << P_a.project() << std::endl;
    for (size_t i = 0; i < M.size(); ++i)
        std::cout << "M[" << i << "]: " << M[i] << std::endl;
    std::cout << "h: " << h << std::endl;
    std::cout << "r_a: " << r_a << std::endl;
    std::cout << "U_a: " << U_a << std::endl;
    std::cout << "u_a: " << u_a << std::endl;
    std::cout << "u_b: " << u_b << std::endl;
    std::cout << "U_b: " << U_b << std::endl;

    return 0;
}
