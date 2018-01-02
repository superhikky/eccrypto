// ECDH鍵共有のサンプルプログラム

#include <eccrypto.h>
#include <iostream>
#include <memory>

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

    // 乱数の生成
    auto rng = std::make_shared<ecc::RandomNumberGenerator<int>>();
    auto r_a = ge(rng->getNumber(1, G->order() - 1));
    auto r_b = ge(rng->getNumber(1, G->order() - 1));

    // 乱数によるスカラー倍点の計算
    auto R_a = r_a * B;
    auto R_b = r_b * B;

    // 鍵の計算
    auto K_a = r_a * R_b;
    auto K_b = r_b * R_a;

    // 結果の表示
    std::cout << "F->order(): " << F->order() << std::endl;
    std::cout << "E->coefficientA(): " << E->coefficientA() << std::endl;
    std::cout << "E->coefficientB(): " << E->coefficientB() << std::endl;
    std::cout << "B: " << B << std::endl;
    std::cout << "G->order(): " << G->order() << std::endl;
    std::cout << "r_a: " << r_a << std::endl;
    std::cout << "r_b: " << r_b << std::endl;
    std::cout << "R_a: " << R_a.project() << std::endl;
    std::cout << "R_b: " << R_b.project() << std::endl;
    std::cout << "K_a: " << K_a.project() << std::endl;
    std::cout << "K_b: " << K_b.project() << std::endl;

    return 0;
}
