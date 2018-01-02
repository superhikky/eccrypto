// ECElGamal暗号のサンプルプログラム

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
    auto d_b = ge(rng->getNumber(1, G->order() - 1));
    auto P_b = d_b * B;

    // 暗号化
    auto r_a = ge(rng->getNumber(1, G->order() - 1));
    auto R_a = r_a * B;
    auto K_a = (P_b * r_a).project();
    std::vector<ecc::element<int>> M_a;
    for (size_t i = 0; i < 3; ++i)
        M_a.push_back(fe(rng->getNumber(0, F->order() - 1)));
    std::vector<ecc::element<int>> C_a;
    for (auto m : M_a) C_a.push_back(m + K_a.x());

    // 復号
    auto K_b = (d_b * R_a).project();
    std::vector<ecc::element<int>> M_b;
    for (auto c : C_a) M_b.push_back(c - K_b.x());

    // 結果の表示
    std::cout << "F->order(): " << F->order() << std::endl;
    std::cout << "E->coefficientA(): " << E->coefficientA() << std::endl;
    std::cout << "E->coefficientB(): " << E->coefficientB() << std::endl;
    std::cout << "B: " << B << std::endl;
    std::cout << "G->order(): " << G->order() << std::endl;
    std::cout << "d_b: " << d_b << std::endl;
    std::cout << "P_b: " << P_b.project() << std::endl;
    std::cout << "r_a: " << r_a << std::endl;
    std::cout << "R_a: " << R_a.project() << std::endl;
    std::cout << "K_a: " << K_a << std::endl;
    for (size_t i = 0; i < M_a.size(); ++i)
        std::cout << "M_a[" << i << "]: " << M_a[i] << std::endl;
    for (size_t i = 0; i < C_a.size(); ++i)
        std::cout << "C_a[" << i << "]: " << C_a[i] << std::endl;
    std::cout << "K_b: " << K_b << std::endl;
    for (size_t i = 0; i < M_b.size(); ++i)
        std::cout << "M_b[" << i << "]: " << M_b[i] << std::endl;

    return 0;
}
