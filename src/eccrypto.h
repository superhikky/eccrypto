/*!\mainpage
 * eccryptoライブラリは楕円曲線暗号を用いた暗号システムの構築を支援し<!--
 * -->ます。\n
 * \n
 * eccryptoライブラリには以下のような機能が含まれます。\n
 * - 楕円曲線上の有理点のスカラー倍算
 * - 乱数の生成
 * - ストリーミング
 * - 多倍長整数
 * .
 * このドキュメントではコードを例示しながら、eccryptoライブラリの使い<!--
 * -->方を説明します。\n
 * \n
 * <a href="pages.html"><b>諸情報へ</b></a>\n
 */
/*!\page pecurve 楕円曲線
 * はじめに楕円曲線暗号を扱うにあたって必要な定義について説明します。\n
 * \section pecspfield 素体
 * 楕円曲線は素体\f$ \mathbb{F}_p=\{0,1,...,p-1\} \f$上に定義されるの<!--
 * -->で、まず素体オブジェクトを作ります。\n
 * \code
 * auto F = std::make_shared<ecc::PrimeField<int>>(7);
 * \endcode
 * ここでは位数を`7`としています。\n
 * \section pecselement 元
 * 元は素体の要素です。\n
 * 元オブジェクトはファクトリーオブジェクトから作ります。\n
 * \code
 * auto fe = ecc::make_factory<ecc::element<int>>(F.get());
 * \endcode
 * テンプレート引数には元オブジェクトの型、引数には素体を渡します。\n
 * \n
 * ファクトリーオブジェクトを呼ぶと、指定した型のオブジェクトを作るこ<!--
 * -->とができます。\n
 * 元オブジェクトにはモジュラー算術による四則演算が定義されており、例<!--
 * -->えば以下のように書くことができます。\n
 * \code
 * auto e_1 = fe(1) + fe(2);
 * auto e_2 = fe(2) - fe(3);
 * auto e_3 = fe(3) * fe(4);
 * auto e_4 = fe(4) / fe(5);
 * \endcode
 * 計算内容は以下のようになります。\n
 * <table border="0"><tr><td>
 * \f{eqnarray*}{
 *     e_1 &\equiv& 1+2 \equiv 3 \bmod 7 \\
 *     e_2 &\equiv& 2-3 \equiv -1 \equiv 6 \bmod 7 \\
 *     e_3 &\equiv& 3 \times 4 \equiv 12 \equiv 5 \bmod 7 \\
 *     e_4 &\equiv& 4 \div 5 \equiv 4 \times 3 \equiv 12 \equiv 5 \bmod 7 \\
 * \f}
 * </td></tr></table>
 * 素体\f$ \mathbb{F}_7 \f$の元は必ず`0`から`6`までの値になります。\n
 * \section pecsecurve 楕円曲線
 * 素体\f$ \mathbb{F}_p \f$上にある楕円曲線\f$ E \f$は次の方程式で定義<!--
 * -->されます。\n
 * \n
 * \f$ E:y^2=x^3+ax+b(a,b \in \mathbb{F}_p) \f$\n
 * \n
 * 楕円曲線オブジェクトを作ります。\n
 * \code
 * auto E = std::make_shared<ecc::EllipticCurve<int>>(F.get(), fe(2), fe(6));
 * \endcode
 * 引数には素体と係数を渡します。\n
 * 係数は素体\f$ \mathbb{F}_p \f$の元です。\n
 * ここでは素体\f$ \mathbb{F}_7 \f$の元として、`a`を`2`、`b`を`6`とし<!--
 * -->ています。\n
 * \section pecsbpoint 基準点
 * 楕円曲線上の有理点の中から基準点を選びます。\n
 * 有理点オブジェクトを作るにはまずファクトリーオブジェクトから作りま<!--
 * -->す。\n
 * \code
 * auto rp = ecc::make_factory<ecc::rational_point<int>>(E.get());
 * \endcode
 * テンプレート引数には有理点オブジェクトの型、引数には楕円曲線を渡し<!--
 * -->ます。\n
 * \n
 * 基準点の有理点オブジェクトを作ります。\n
 * \code
 * auto B = rp(fe(1), fe(3));
 * \endcode
 * 引数には座標を渡します。\n
 * 座標の値は素体\f$ \mathbb{F}_p \f$の元です。\n
 * ここでは素体\f$ \mathbb{F}_7 \f$の元として、x座標を`1`、y座標を`3`<!--
 * -->としています。\n
 * \n
 * 有理点は座標をヤコビアン座標系で保持します。\n
 * 構築する際にz座標を省略すると`1`になります。\n
 * \code
 * auto B = rp(fe(1), fe(3)); // (1, 3, 1)
 * \endcode
 * z座標が`1`のときのx座標とy座標はアフィン座標系と等しくなります。\n
 * ヤコビアン座標系からアフィン座標系に射影したいときは<!--
 * -->`ecc::rational_point::project`メンバ関数を呼びます。\n
 * \code
 * auto J = rp(fe(8), fe(24), fe(2)); // (1, 3, 2)
 * auto A = J.project();              // (2, 3, 1)
 * \endcode
 * 点位数が`l`の基準点をスカラー倍算するためのスカラーは素体<!--
 * -->\f$ \mathbb{G}_l \f$の元なので、素体オブジェクトを作ります。\n
 * \code
 * auto G = std::make_shared<ecc::PrimeField<int>>(11);
 * \endcode
 * 引数には点位数を渡します。\n
 * \n
 * スカラーの元オブジェクトを作るにはまずファクトリーオブジェクトから<!--
 * -->作ります。\n
 * \code
 * auto ge = ecc::make_factory<ecc::element<int>>(G.get());
 * \endcode
 * これで楕円曲線暗号を扱う準備ができました。\n
 * \n
 * \ref pecdh "次のページへ"
 */
/*!\page pecdh ECDH鍵共有
 * アリスとボブは暗号文をやり取りする前に鍵を共有します。\n
 * \section pdhsgrnumber 乱数の生成
 * 乱数を生成するにはまず乱数生成器オブジェクトを作ります。\n
 * \code
 * auto rng = std::make_shared<RandomNumberGenerator<int>>();
 * \endcode
 * アリスは乱数を生成します。\n
 * \code
 * auto r_a = ge(rng->getNumber(1, G->order() - 1));
 * \endcode
 * 引数には最小値と最大値を渡します。\n
 * 乱数`r_a`は基準点のスカラー倍算に使うスカラーであり、素体<!--
 * -->\f$ \mathbb{G}_l \f$の元なので`1`から`G->order() - 1`までの値に<!--
 * -->しています。\n
 * \n
 * 同様にボブも乱数を生成します。\n
 * \code
 * auto r_b = ge(rng->getNumber(1, G->order() - 1));
 * \endcode
 * \section pdhssmultiplication スカラー倍算
 * アリスは基準点`B`を乱数`r_a`でスカラー倍算します。\n
 * \code
 * auto R_a = r_a * B;
 * \endcode
 * 同様にボブも基準点`B`を乱数`r_b`でスカラー倍算します。\n
 * \code
 * auto R_b = r_b * B;
 * \endcode
 * アリスは有理点`R_a`をボブに送り、ボブは有理点`R_b`をアリスに送りま<!--
 * -->す。\n
 * \section pdhsskeys 鍵の共有
 * アリスはボブから受け取った有理点`R_b`を乱数`r_a`でスカラー倍算しま<!--
 * -->す。\n
 * \code
 * auto K_a = r_a * R_b;
 * \endcode
 * 同様にボブもアリスから受け取った有理点`R_a`を乱数`r_b`でスカラー倍<!--
 * -->算します。\n
 * \code
 * auto K_b = r_b * R_a;
 * \endcode
 * ここまでの計算で、
 * <table border="0"><tr><td>
 * \f{eqnarray*}{
 *     K_a &=& r_a \times R_b\\
 *         &=& r_a \times r_b \times G\\
 *         &=& r_b \times r_a \times G\\
 *         &=& r_b \times R_a\\
 *         &=& K_b\\
 * \f} 
 * </td></tr></table>
 * となり、有理点`K_a`と有理点`K_b`は等しくなります。\n
 * この共通の有理点を鍵として共有することで暗号文をやり取りすることが<!--
 * -->できます。\n
 * \section pdhsimplimentation 実装例
 * ECDH鍵共有を実装したサンプルプログラムのソースコードを示します。\n
 * \code
 * // ECDH鍵共有のサンプルプログラム
 * 
 * #include <eccrypto.h>
 * #include <iostream>
 * #include <memory>
 * 
 * int main(int argc, char** argv) {
 *     // 楕円曲線の定義
 *     auto F = std::make_shared<ecc::PrimeField<int>>(7);
 *     auto fe = ecc::make_factory<ecc::element<int>>(F.get());
 *     auto E = std::make_shared<ecc::EllipticCurve<int>>(F.get(), fe(2), fe(6));
 * 
 *     // 基準点の定義
 *     auto rp = ecc::make_factory<ecc::rational_point<int>>(E.get());
 *     auto B = rp(fe(1), fe(3));
 *     auto G = std::make_shared<ecc::PrimeField<int>>(11);
 *     auto ge = ecc::make_factory<ecc::element<int>>(G.get());
 * 
 *     // 乱数の生成
 *     auto rng = std::make_shared<ecc::RandomNumberGenerator<int>>();
 *     auto r_a = ge(rng->getNumber(1, G->order() - 1));
 *     auto r_b = ge(rng->getNumber(1, G->order() - 1));
 * 
 *     // 乱数によるスカラー倍点の計算
 *     auto R_a = r_a * B;
 *     auto R_b = r_b * B;
 * 
 *     // 鍵の計算
 *     auto K_a = r_a * R_b;
 *     auto K_b = r_b * R_a;
 * 
 *     // 結果の表示
 *     std::cout << "F->order(): " << F->order() << std::endl;
 *     std::cout << "E->coefficientA(): " << E->coefficientA() << std::endl;
 *     std::cout << "E->coefficientB(): " << E->coefficientB() << std::endl;
 *     std::cout << "B: " << B << std::endl;
 *     std::cout << "G->order(): " << G->order() << std::endl;
 *     std::cout << "r_a: " << r_a << std::endl;
 *     std::cout << "r_b: " << r_b << std::endl;
 *     std::cout << "R_a: " << R_a.project() << std::endl;
 *     std::cout << "R_b: " << R_b.project() << std::endl;
 *     std::cout << "K_a: " << K_a.project() << std::endl;
 *     std::cout << "K_b: " << K_b.project() << std::endl;
 * 
 *     return 0;
 * }
 * \endcode
 * 実行した例を示します。\n
 * \code
 * F->order(): 7
 * E->coefficientA(): 2
 * E->coefficientB(): 6
 * B: {1, 3, 1}
 * G->order(): 11
 * r_a: 9
 * r_b: 7
 * R_a: {2, 5, 1}
 * R_b: {3, 2, 1}
 * K_a: {5, 6, 1}
 * K_b: {5, 6, 1}
 * \endcode
 * 有理点`K_a`と有理点`K_b`の座標が等しくなっていることがわかります。\n
 * \n
 * \ref pecelgamal "次のページへ"
 */
/*!\page pecelgamal ECElGamal暗号
 * アリスは平文を暗号化してボブに送ります、\n
 * ボブはアリスから受け取った暗号文を平文に復号します。\n
 * \section pegsgkeys 鍵の生成
 * ボブは乱数を生成します。\n
 * \code
 * auto d_b = ge(rng->getNumber(1, G->order() - 1));
 * \endcode
 * ボブは乱数`d_b`を秘密鍵として保管します。\n
 * \n
 * ボブは基準点`B`を秘密鍵`d_b`でスカラー倍算します。\n
 * \code
 * auto P_b = d_b * B;
 * \endcode
 * ボブは有理点`P_b`を公開鍵として公開します。\n
 * \section pegsencryption 暗号化
 * アリスは乱数を生成します。\n
 * \code
 * auto r_a = ge(rng->getNumber(1, G->order() - 1));
 * \endcode
 * アリスは基準点`B`を乱数`r_a`でスカラー倍算します。\n
 * \code
 * auto R_a = r_a * B;
 * \endcode
 * アリスはボブの公開鍵`P_b`を入手し、乱数`r_a`でスカラー倍算します。\n
 * \code
 * auto K_a = (P_b * r_a).project();
 * \endcode
 * 有理点`K_a`はアフィン座標系で扱います。\n
 * \n
 * アリスは平文を素体\f$ \mathbb{F}_7 \f$の元の配列として表します。\n
 * \code
 * std::vector<ecc::element<int>> M_a;
 * M_a.push_back(fe(3));
 * M_a.push_back(fe(2));
 * M_a.push_back(fe(6));
 * \endcode
 * アリスは平文の各要素と有理点`K_a`のx座標を加算し、暗号文を作ります。\n
 * \code
 * std::vector<ecc::element<int>> C_a;
 * for (auto m : M_a) C_a.push_back(m + K_a.x());
 * \endcode
 * アリスは有理点`R_a`と暗号文`C_a`をボブに送ります。\n
 * \section pegsdecryption 復号
 * ボブはアリスから受け取った有理点`R_a`を秘密鍵`d_b`でスカラー倍算し<!--
 * -->ます。\n
 * \code
 * auto K_b = (d_b * R_a).project();
 * \endcode
 * 有理点`K_b`はアフィン座標系で扱います。\n
 * \n
 * ここまでの計算で、
 * <table border="0"><tr><td>
 * \f{eqnarray*}{
 *     K_a &=& r_a \times P_b\\
 *         &=& r_a \times d_b \times G\\
 *         &=& d_b \times r_a \times G\\
 *         &=& d_b \times R_a\\
 *         &=& K_b\\
 * \f} 
 * </td></tr></table>
 * となり、有理点`K_a`と有理点`K_b`は等しくなります。\n
 * \n
 * ボブはアリスから受け取った暗号文の各要素から有理点`K_b`のx座標を減<!--
 * -->算し、平文を作ります。\n
 * \code
 * std::vector<ecc::element<int>> M_b;
 * for (auto c : C_a) M_b.push_back(c - K_b.x());
 * \endcode
 * ここまでの計算で、
 * <table border="0"><tr><td>
 * \f{eqnarray*}{
 *     K_ax &=& K_bx\\
 *     C_ai &=& M_ai+K_ax\\
 *     M_bi &=& C_ai-K_bx\\
 *          &=& M_ai+K_ax-K_bx\\
 *          &=& M_ai\\
 * \f}
 * </td></tr></table>
 * となり、平文`M_a`と平文`M_b`は等しくなるので、正しく復号できたこと<!--
 * -->になります。\n
 * \section pegsimplimentation 実装例
 * ECElGamal暗号を実装したサンプルプログラムのソースコードを示します。\n
 * \code
 * // ECElGamal暗号のサンプルプログラム
 * 
 * #include <eccrypto.h>
 * #include <iostream>
 * #include <memory>
 * #include <vector>
 * 
 * int main(int argc, char** argv) {
 *     // 楕円曲線の定義
 *     auto F = std::make_shared<ecc::PrimeField<int>>(7);
 *     auto fe = ecc::make_factory<ecc::element<int>>(F.get());
 *     auto E = std::make_shared<ecc::EllipticCurve<int>>(F.get(), fe(2), fe(6));
 * 
 *     // 基準点の定義
 *     auto rp = ecc::make_factory<ecc::rational_point<int>>(E.get());
 *     auto B = rp(fe(1), fe(3));
 *     auto G = std::make_shared<ecc::PrimeField<int>>(11);
 *     auto ge = ecc::make_factory<ecc::element<int>>(G.get());
 * 
 *     // 鍵の生成
 *     auto rng = std::make_shared<ecc::RandomNumberGenerator<int>>();
 *     auto d_b = ge(rng->getNumber(1, G->order() - 1));
 *     auto P_b = d_b * B;
 * 
 *     // 暗号化
 *     auto r_a = ge(rng->getNumber(1, G->order() - 1));
 *     auto R_a = r_a * B;
 *     auto K_a = (P_b * r_a).project();
 *     std::vector<ecc::element<int>> M_a;
 *     for (size_t i = 0; i < 3; ++i)
 *         M_a.push_back(fe(rng->getNumber(0, F->order() - 1)));
 *     std::vector<ecc::element<int>> C_a;
 *     for (auto m : M_a) C_a.push_back(m + K_a.x());
 * 
 *     // 復号
 *     auto K_b = (d_b * R_a).project();
 *     std::vector<ecc::element<int>> M_b;
 *     for (auto c : C_a) M_b.push_back(c - K_b.x());
 * 
 *     // 結果の表示
 *     std::cout << "F->order(): " << F->order() << std::endl;
 *     std::cout << "E->coefficientA(): " << E->coefficientA() << std::endl;
 *     std::cout << "E->coefficientB(): " << E->coefficientB() << std::endl;
 *     std::cout << "B: " << B << std::endl;
 *     std::cout << "G->order(): " << G->order() << std::endl;
 *     std::cout << "d_b: " << d_b << std::endl;
 *     std::cout << "P_b: " << P_b.project() << std::endl;
 *     std::cout << "r_a: " << r_a << std::endl;
 *     std::cout << "R_a: " << R_a.project() << std::endl;
 *     std::cout << "K_a: " << K_a << std::endl;
 *     for (size_t i = 0; i < M_a.size(); ++i)
 *         std::cout << "M_a[" << i << "]: " << M_a[i] << std::endl;
 *     for (size_t i = 0; i < C_a.size(); ++i)
 *         std::cout << "C_a[" << i << "]: " << C_a[i] << std::endl;
 *     std::cout << "K_b: " << K_b << std::endl;
 *     for (size_t i = 0; i < M_b.size(); ++i)
 *         std::cout << "M_b[" << i << "]: " << M_b[i] << std::endl;
 * 
 *     return 0;
 * }
 * \endcode
 * 平文は乱数から作っています。\n
 * \n
 * 実行した例を示します。\n
 * \code
 * F->order(): 7
 * E->coefficientA(): 2
 * E->coefficientB(): 6
 * B: {1, 3, 1}
 * G->order(): 11
 * d_b: 5
 * P_b: {4, 1, 1}
 * r_a: 8
 * R_a: {5, 6, 1}
 * K_a: {3, 2, 1}
 * M_a[0]: 1
 * M_a[1]: 4
 * M_a[2]: 2
 * C_a[0]: 4
 * C_a[1]: 0
 * C_a[2]: 5
 * K_b: {3, 2, 1}
 * M_b[0]: 1
 * M_b[1]: 4
 * M_b[2]: 2
 * \endcode
 * 暗号化される平文`M_a`と復号した平文`M_b`の内容が等しくなっているこ<!--
 * -->とがわかります。\n
 * \n
 * \ref pecdsa "次のページへ"
 */
/*!\page pecdsa ECDSA署名
 * アリスは平文が改竄されていないことを示すために、署名文をボブに送り<!--
 * -->ます。\n
 * \section pdsasgkeys 鍵の生成
 * アリスは乱数を生成します。\n
 * \code
 * auto d_a = ge(rng->getNumber(1, G->order() - 1));
 * \endcode
 * アリスは乱数`d_a`を秘密鍵として保管します。\n
 * \n
 * アリスは基準点`B`を秘密鍵`d_a`でスカラー倍算します。\n
 * \code
 * auto P_a = d_a * B;
 * \endcode
 * アリスは有理点`P_a`を公開鍵として公開します。\n
 * \section pdsashash ハッシュ
 * アリスとボブは平文のハッシュ値を計算します。\n
 * 平文の入力からハッシュ値の出力までを1つのラインとして構築し、実行し<!--
 * -->ます。\n
 * ラインについては\ref pstreaming "ストリーミング"で説明しています。\n
 * \code
 * auto h = ge(0);
 * ecc::concatinate(
 *     std::make_shared<ecc::InputProcess<int>>
 *         (ecc::input_from(ecc::make_getter_from_range<int>(M.begin(), M.end()))),
 *     std::make_shared<ecc::InputOutputProcess<int, int>>
 *         (ecc::numbers_to_bytes<int>()),
 *     std::make_shared<ecc::InputOutputProcess<int, int>>
 *         (ecc::pad_message(64)),
 *     std::make_shared<ecc::InputOutputProcess<int, uint32_t>>
 *         (ecc::bytes_to_numbers<uint32_t>()),
 *     std::make_shared<ecc::InputOutputProcess<uint32_t, uint32_t>>
 *         (ecc::sha1()),
 *     std::make_shared<ecc::InputOutputProcess<uint32_t, int>>
 *         (ecc::numbers_to_bytes<uint32_t>()),
 *     std::make_shared<ecc::InputOutputProcess<int, ecc::element<int>>>
 *         (ecc::transform<int, ecc::element<int>>(ge)),
 *     std::make_shared<ecc::OutputProcess<ecc::element<int>>>
 *         (ecc::output_to(ecc::make_putter_from_sum<ecc::element<int>>(&h)))
 * )->run();
 * \endcode
 * 各工程の処理内容は以下のようになります。\n
 * -# 平文を`int`整数列として出力する。\n
 * -# `int`整数列をバイト列に変換する。\n
 * -# バイト列の末尾に詰め物を付加する。\n
 * -# バイト列を`uint32_t`整数列に変換する。\n
 * -# SHA-1アルゴリズムによる計算を行う。\n
 * -# `uint32_t`整数列をバイト列に変換する。\n
 * -# バイト列を素体\f$ \mathbb{G}_l \f$の元の列に変換する。\n
 * -# 素体\f$ \mathbb{G}_l \f$の元の列をハッシュ値`h`に合算する。\n
 * .
 * SHA-1ではメッセージをブロック単位で入力します。\n
 * そのため`ecc::pad_message`関数で作った処理関数でデータの末尾に詰め<!--
 * -->物を付加します。\n
 * 整数列やバイト列の変換には補助関数が用意されています。\n
 * \section pdsassign 署名
 * アリスは乱数を生成します。\n
 * \code
 * auto r_a = ge(rng->getNumber(1, G->order() - 1));
 * \endcode
 * アリスは基準点`B`を乱数`r_a`でスカラー倍算します。\n
 * \code
 * auto U_a = (r_a * B).project();
 * \endcode
 * 有理点`U_a`はアフィン座標系で扱います。\n
 * \n
 * アリスはスカラー`u_a`を計算します。\n
 * \code
 * auto u_a = (h + ge(U_a.x()) * d_a) / r_a;
 * \endcode
 * `u_a`は後ほどボブが除数として使うので`0`になってはいけません。\n
 * `0`になる場合は乱数`r_a`の生成からやり直します。\n
 * \n
 * アリスは署名文として有理点`U_a`とスカラー`u_a`をボブに送ります。\n
 * \section pdsasverify 検証
 * ボブはアリスから受け取ったスカラー`u_a`の逆数を計算します。\n
 * \code
 * auto u_b = ge(1) / u_a;
 * \endcode
 * ボブはハッシュ値、スカラー`u_b`、アリスから受け取った有理点`U_a`、<!--
 * -->アリスの公開鍵から有理点`U_b`を計算します。\n
 * \code
 * auto U_b = h * u_b * B + ge(U_a.x()) * u_b * P_a;
 * \endcode
 * ここまでの計算で、
 * <table border="0"><tr><td>
 * \f{eqnarray*}{
 *     u_a &=& \frac{1}{u_b}\\
 *     r_a &=& \frac{h + U_ax \times d_a}{u_a}\\
 *         &=& \frac{h + U_ax \times d_a}{ \frac{1}{u_b} }\\
 *         &=& (h + U_ax \times d_a) \times u_b\\
 *     U_a &=& r_a \times B\\
 *         &=& (h + U_ax \times d_a) \times u_b \times B\\
 *         &=& h \times u_b \times B + U_ax \times u_b \times d_a \times B\\
 *         &=& h \times u_b \times B + U_ax \times u_b \times P_a\\
 *         &=& U_b\\
 * \f} 
 * </td></tr></table>
 * となり、有理点`U_a`と有理点`U_b`は等しくなるので、平文は改竄されて<!--
 * -->いないことになります。\n
 * \section pdsasimplimentation 実装例
 * ECDSA署名を実装したサンプルプログラムのソースコードを示します。\n
 * \code
 * // ECDSA署名のサンプルプログラム
 * 
 * #include <cstdint>
 * #include <eccrypto.h>
 * #include <iostream>
 * #include <memory>
 * #include <vector>
 * 
 * int main(int argc, char** argv) {
 *     // 楕円曲線の定義
 *     auto F = std::make_shared<ecc::PrimeField<int>>(7);
 *     auto fe = ecc::make_factory<ecc::element<int>>(F.get());
 *     auto E = std::make_shared<ecc::EllipticCurve<int>>(F.get(), fe(2), fe(6));
 * 
 *     // 基準点の定義
 *     auto rp = ecc::make_factory<ecc::rational_point<int>>(E.get());
 *     auto B = rp(fe(1), fe(3));
 *     auto G = std::make_shared<ecc::PrimeField<int>>(11);
 *     auto ge = ecc::make_factory<ecc::element<int>>(G.get());
 * 
 *     // 鍵の生成
 *     auto rng = std::make_shared<ecc::RandomNumberGenerator<int>>();
 *     auto d_a = ge(rng->getNumber(1, G->order() - 1));
 *     auto P_a = d_a * B;
 * 
 *     // 平文の作成
 *     std::vector<ecc::element<int>> M;
 *     for (size_t i = 0; i < 3; ++i)
 *         M.push_back(fe(rng->getNumber(0, F->order() - 1)));
 * 
 *     // ハッシュ
 *     auto h = ge(0);
 *     ecc::concatinate(
 *         std::make_shared<ecc::InputProcess<int>>
 *             (ecc::input_from(ecc::make_getter_from_range<int>(M.begin(), M.end()))),
 *         std::make_shared<ecc::InputOutputProcess<int, int>>
 *             (ecc::numbers_to_bytes<int>()),
 *         std::make_shared<ecc::InputOutputProcess<int, int>>
 *             (ecc::pad_message(64)),
 *         std::make_shared<ecc::InputOutputProcess<int, uint32_t>>
 *             (ecc::bytes_to_numbers<uint32_t>()),
 *         std::make_shared<ecc::InputOutputProcess<uint32_t, uint32_t>>
 *             (ecc::sha1()),
 *         std::make_shared<ecc::InputOutputProcess<uint32_t, int>>
 *             (ecc::numbers_to_bytes<uint32_t>()),
 *         std::make_shared<ecc::InputOutputProcess<int, ecc::element<int>>>
 *             (ecc::transform<int, ecc::element<int>>(ge)),
 *         std::make_shared<ecc::OutputProcess<ecc::element<int>>>
 *             (ecc::output_to(ecc::make_putter_from_sum<ecc::element<int>>(&h)))
 *     )->run();
 * 
 *     // 署名
 *     ecc::element<int> r_a, u_a;
 *     ecc::rational_point<int> U_a;
 *     do {
 *         r_a = ge(rng->getNumber(1, G->order() - 1));
 *         U_a = (r_a * B).project();
 *         u_a = (h + ge(U_a.x()) * d_a) / r_a;
 *     } while (!u_a);
 * 
 *     // 検証
 *     auto u_b = ge(1) / u_a;
 *     auto U_b = h * u_b * B + ge(U_a.x()) * u_b * P_a;
 * 
 *     // 結果の表示
 *     std::cout << "F->order(): " << F->order() << std::endl;
 *     std::cout << "E->coefficientA(): " << E->coefficientA() << std::endl;
 *     std::cout << "E->coefficientB(): " << E->coefficientB() << std::endl;
 *     std::cout << "B: " << B << std::endl;
 *     std::cout << "G->order(): " << G->order() << std::endl;
 *     std::cout << "d_a: " << d_a << std::endl;
 *     std::cout << "P_a: " << P_a.project() << std::endl;
 *     for (size_t i = 0; i < M.size(); ++i)
 *         std::cout << "M[" << i << "]: " << M[i] << std::endl;
 *     std::cout << "h: " << h << std::endl;
 *     std::cout << "r_a: " << r_a << std::endl;
 *     std::cout << "U_a: " << U_a << std::endl;
 *     std::cout << "u_a: " << u_a << std::endl;
 *     std::cout << "u_b: " << u_b << std::endl;
 *     std::cout << "U_b: " << U_b.project() << std::endl;
 * 
 *     return 0;
 * }
 * \endcode
 * 平文は乱数から作っています。\n
 * \n
 * 実行した例を示します。\n
 * \code
 * F->order(): 7
 * E->coefficientA(): 2
 * E->coefficientB(): 6
 * B: {1, 3, 1}
 * G->order(): 11
 * d_a: 4
 * P_a: {3, 5, 1}
 * M[0]: 3
 * M[1]: 0
 * M[2]: 6
 * h: 10
 * r_a: 6
 * U_a: {4, 6, 1}
 * u_a: 8
 * u_b: 7
 * U_b: {4, 6, 1}
 * \endcode
 * 有理点`U_a`と有理点`U_b`の座標が等しくなっていることがわかります。\n
 * \n
 * \ref pstreaming "次のページへ"
 */
/*!\page pstreaming ストリーミング
 * 大きなデータを小さなデータ片に分割し、少しずつ処理することでメモリ<!--
 * -->の消費量を抑えることができます。\n
 * このページではストリーミングを行うコードをどのように書けばよいのか<!--
 * -->について考えます。\n
 * \section pssstructure 構造
 * 基本的な構造はデータ片を入力し、処理し、出力することの繰り返しです。\n
 * 例えばバイト単位で入力し、演算し、出力するとします。\n
 * \code
 * for (;;) put_byte(get_byte() ^ 0xff);
 * \endcode
 * まだ`EOF`については考えないことにします。\n
 * \section psstransformation 変換
 * 入力と出力でデータ片の大きさが異なる場合は変換する処理を行います。\n
 * 例えばワード単位で入力し、ビット単位で出力するとします。\n
 * \code
 * for (;;) {
 *     uint16_t wor = get_word();
 *     for (size_t i = 0; i < 16; ++i) put_bit((wor >> i) & 1);
 * }
 * \endcode
 * 直接変換するのではなく、ワードからバイトへの変換とバイトからビット<!--
 * -->への変換を分け、それらを連結するほうがよさそうです。\n
 * \section pssconcatination 連結
 * 処理と処理を連結するにはどうすればよいでしょうか。\n
 * 例えば`proc1`関数は`proc2`関数に出力し、`proc2`関数は`proc1`関数か<!--
 * -->ら入力するとします。\n
 * \code
 * void proc1() {
 *     for (;;) put_byte_to_proc2(get_byte() ^ 0xff);
 * }
 *
 * void proc2() {
 *     for (;;) put_byte(get_byte_from_proc1() & 0x77);
 * }
 * \endcode
 * `get...`関数はスレッドをブロックするので、`proc1`関数と`proc2`関数<!--
 * -->は別々のスレッドで実行しなければなりません。\n
 * \section pssline ライン
 * `ecc::Process`クラスを基底とするオブジェクトはストリーミングの「工<!--
 * -->程」を表します。\n
 * 工程オブジェクトを使えば、処理ごとに独立したスレッドで実行する「ラ<!--
 * -->イン」を構築することができます。\n
 * 例えば最初の工程で果物の名前を出力し、次の工程で値段を出力し、最後<!--
 * -->の工程で表示するラインは以下のようになります。\n
 * \code
 * auto inpPro = std::make_shared<ecc::InputProcess<std::string>>(
 *     [] (const std::function<void(const std::string& nam)>& putNam) {
 *         putNam("りんご");
 *         putNam("メロン");
 *         putNam("みかん");
 *     }
 * );
 * auto inpOutPro = std::make_shared<ecc::InputOutputProcess<std::string, int>>(
 *     [] (
 *         const std::function<std::string()>& getNam,
 *         const std::function<void(const int& pri)>& putPri
 *     ) {
 *         static const std::map<std::string, int> PRIS({
 *             std::make_pair(std::string("みかん"), 20),
 *             std::make_pair(std::string("りんご"), 50),
 *             std::make_pair(std::string("メロン"), 2500),
 *         });
 *         for (;;) putPri(PRIS.at(getNam()));
 *     }
 * );
 * auto outPro = std::make_shared<ecc::OutputProcess<int>>(
 *     [] (const std::function<int()>& getPri) {
 *         for (;;) std::cout << getPri() << "円" << std::endl;
 *     }
 * );
 * ecc::concatinate(inpPro, inpOutPro, outPro);
 * outPro->run();
 * \endcode
 * 実行した結果を示します。\n
 * \code
 * 50円
 * 2500円
 * 20円
 * \endcode
 * 最初の工程で出力した名前の順番通りに値段が表示されました。\n
 * 3つの工程が連携して動作していることがわかります。\n
 * \section pssprocess 工程
 * 各工程の処理内容は以下のようになります。\n
 * -# 果物の名前を`std::string`列として出力する。\n
 * -# `std::string`列を入力し、対応する値段を`int`整数列として出力する。\n
 * -# `int`整数列を入力し、末尾に単位を付加した文字列を標準出力に書き<!--
 *    -->込む。\n
 * .
 * ラインを構築するには、まず先頭に`ecc::InputProcess`オブジェクトを配<!--
 * -->置し、処理関数でデータを入力します。\n
 * その次に`ecc::InputOutputProcess`オブジェクトを配置し、処理関数で何<!--
 * -->らかの加工を行います。\n
 * `ecc::InputOutputProcess`オブジェクトは必要なら何個でも続けて配置で<!--
 * -->きます。\n
 * 最後は末尾に`ecc::OutputProcess`オブジェクトを配置し、処理関数でデ<!--
 * -->ータを出力します。\n
 * \n
 * 前の工程で出力するデータ片の型は次の工程で入力するデータ片の型にな<!--
 * -->ります。\n
 * 前の工程の処理が終わると、次の工程の`get...`関数は<!--
 * -->`ecc::eof_exception`をスローするので、ループを抜けることができま<!--
 * -->す。\n
 * \n
 * `ecc::concatinate`関数は可変長引数を取り、隣接する工程同士を連結し<!--
 * -->ます。\n
 * ここでは`inpPro`と`inpOutPro`、`inpOutPro`と`outPro`をそれぞれ連結<!--
 * -->しています。\n
 * \section pssmthreading マルチスレッディング
 * すべての工程を連結したら、`ecc::OutputProcess::run`メンバ関数を呼ん<!--
 * -->で実行します。\n
 * ライン上にある工程ごとにスレッドが起動し、それぞれの処理を独立した<!--
 * -->スレッドで実行し、すべてのスレッドの終了を待ってから制御を戻しま<!--
 * -->す。\n
 * \n
 * このように、ラインを構築すればストリーミングにおける各工程の処理を<!--
 * -->簡潔に書くことができ、簡単に連結できます。\n
 * \n
 * \ref pbinteger "次のページへ"
 */
/*!\page pbinteger 多倍長整数
 * 楕円曲線暗号の安全性は鍵の長さに依存します。\n
 * 安全な鍵の長さは160ビット以上と言われていますが、組み込み整数型では<!--
 * -->扱うことが出来ません。\n
 * `ecc::big_int`クラスは160ビット以上の長さの鍵を扱うことができるので<!--
 * -->、安全な暗号の実現に貢献します。\n
 * \section pbisinstanciation 構築
 * `ecc::big_int`クラスのインスタンスは組み込み整数型の値や文字列から<!--
 * -->構築します。\n
 * \code
 * auto a = ecc::big_int<2>(100);
 * auto b = ecc::big_int<10>("123456789012345678901234567890");
 * auto c = ecc::big_int<20>("abcdefabcdefabcdefabcdefabcdef", std::ios::hex);
 * std::cout << "a: " << std::dec << a << std::endl;
 * std::cout << "b: " << std::dec << b << std::endl;
 * std::cout << "c: " << std::hex << c << std::endl;
 * \endcode
 * 実行した結果を示します。\n
 * \code
 * a: 100
 * b: 123456789012345678901234567890
 * c: abcdefabcdefabcdefabcdefabcdef
 * \endcode
 * テンプレート引数には大きさをバイト長で渡します。\n
 * この大きさまでならオーバーフローせずに乗算できることが保証されます。\n
 * \n
 * 文字列から作る場合はコンストラクタの2番目の引数に基数を示すフラグを<!--
 * -->渡すことができます。\n
 * \section pbisoperation 演算
 * `int`と同様に四則演算、ビット演算、シフト演算、関係演算を行うことが<!--
 * -->できます。\n
 * 二項演算における左右の`ecc::big_int`同士の大きさは同じでなければな<!--
 * -->りません。\n
 * \code
 * auto a = ecc::big_int<4>("200000000");
 * auto b = ecc::big_int<4>("100000000");
 * auto c = ecc::big_int<8>("3333333333333333", std::ios::hex);
 * auto d = ecc::big_int<8>("5555555555555555", std::ios::hex);
 * auto e = ecc::big_int<2>(8);
 * auto f = ecc::big_int<2>(-7);
 * std::cout << "a + b: " << std::dec << (a + b) <<std::endl;
 * std::cout << "a - b: " << std::dec << (a - b) <<std::endl;
 * std::cout << "a * b: " << std::dec << (a * b) <<std::endl;
 * std::cout << "a / b: " << std::dec << (a / b) <<std::endl;
 * std::cout << "-a: " << std::dec << (-a) <<std::endl;
 * std::cout << "c & d: " << std::hex << (c & d) << std::endl;
 * std::cout << "c | d: " << std::hex << (c | d) << std::endl;
 * std::cout << "c ^ d: " << std::hex << (c ^ d) << std::endl;
 * std::cout << "~c: " << std::hex << (~c) << std::endl;
 * std::cout << "e << 2: " << std::dec << (e << 2) << std::endl;
 * std::cout << "e >> 2: " << std::dec << (e >> 2) << std::endl;
 * std::cout << "f << 2: " << std::hex << (f << 2) << std::endl;
 * std::cout << "f >> 2: " << std::hex << (f >> 2) << std::endl;
 * std::cout << "e == e: " << std::dec << (e == e) << std::endl;
 * std::cout << "e == f: " << std::dec << (e == f) << std::endl;
 * std::cout << "e != f: " << std::dec << (e != f) << std::endl;
 * std::cout << "f != f: " << std::dec << (f != f) << std::endl;
 * std::cout << "e < f: " << std::dec << (e < f) << std::endl;
 * std::cout << "e <= f: " << std::dec << (e <= f) << std::endl;
 * std::cout << "e > f: " << std::dec << (e > f) << std::endl;
 * std::cout << "e >= f: " << std::dec << (e >= f) << std::endl;
 * \endcode
 * 実行した結果を示します。\n
 * \code
 * a + b: 300000000
 * a - b: 100000000
 * a * b: 20000000000000000
 * a / b: 2
 * -a: -200000000
 * c & d: 1111111111111111
 * c | d: 7777777777777777
 * c ^ d: 6666666666666666
 * ~c: ffffffffffffffffffffcccccccccccccccc
 * e << 2: 32
 * e >> 2: 2
 * f << 2: ffffffffffe4
 * f >> 2: fffffffffffe
 * e == e: 1
 * e == f: 0
 * e != f: 1
 * f != f: 0
 * e < f: 0
 * e <= f: 0
 * e > f: 1
 * e >= f: 1
 * \endcode
 * \section pbisaccess アクセス
 * 整数のデータは32ビットごとに分割され、「フィールド」の配列に格納さ<!--
 * -->れます。\n
 * \code
 * auto a = ecc::big_int<4, true>(-3);
 * for (size_t i = 0; i < ecc::big_int<4, true>::NUMBER_OF_FIELDS; ++i)
 *     std::cout << std::hex << a[i] << " ";
 * \endcode
 * 実行した結果を示します。\n
 * \code
 * fffffffd ffffffff
 * \endcode
 * `ecc::big_int::NUMBER_OF_FIELDS`定数にフィールドの数が定義されます。\n
 * 各フィールドには`ecc::big_int::operator[]`演算子でアクセスできます。\n
 * \n
 * 組み込み整数型に変換すると最下位部分を取得できます。\n
 * \code
 * auto a = ecc::big_int<2>(0x1234);
 * do std::cout << std::hex << (int(a) & 0xff) << " ";
 * while (a >>= 8);
 * \endcode
 * 実行した結果を示します。\n
 * \code
 * 34 12 
 * \endcode
 * \section pbisuse 使用
 * これまで`int`を渡していたところで`ecc::big_int`を渡してください。\n
 * 例えば`ecc::big_int`を使った素体\f$ \mathbb{F}_p \f$は次のように定<!--
 * -->義します。\n
 * \code
 * auto F = std::make_shared<ecc::PrimeField<ecc::big_int<20>>>
 *     (ecc::big_int<20>("1461501637330902918203684832716283019653785059327"));
 * \endcode
 * 160ビット長なら`ecc::big_int<20>`、256ビット長なら<!--
 * -->`ecc::big_int<32>`を使ってください。\n
 * \n
 * \ref pstools "次のページへ"
 */
/*!\page pstools サンプルツール
 * 参考までに`ecc::big_int`クラスを使った5つのサンプルツールを用意しま<!--
 * -->した。\n
 * 楕円曲線と基準点の定義は以下の値をハードコーディングしています。\n
 * <table border="0"><tr><td>
 * \f{eqnarray*}{
 *         素体の位数p &=& 1461501637330902918203684832716283019653785059327\\
 *     楕円曲線の係数a &=& -3\\
 *               係数b &=& 163235791306168110546604919403271579530548345413\\
 *       基準点のx座標 &=& 425826231723888350446541592701409065913635568770\\
 *               y座標 &=& 203520114162904107873991457957346892027982641970\\
 *             点位数l &=& 1461501637330902918203687197606826779884643492439\\
 * \f} 
 * </td></tr></table>
 * \section pstskey key
 * `usage: key`\n
 * \n
 * `key`は鍵を生成します。\n
 * 1行目に秘密鍵、2行目に公開鍵を表示します。\n
 * \n
 * 実行した例を示します。\n
 * \code
 * $ ./key
 * QLRqG8K4cy/g106ocTQKTIF23bk=
 * Mfx+uhX0C+U0MH7CfP0UTe0amwD5XavdkAVzCXkEWf9l+N6YR3okUw==
 * \endcode
 * \section pstsencrypt encrypt
 * `usage: encrypt publicKey messageFile cipherFile`\n
 * \n
 * `encrypt`は平文を暗号化します。\n
 * 引数には受信者の公開鍵、平文ファイル名、暗号文ファイル名を渡します。\n
 * \n
 * 実行した例を示します。\n
 * \code
 * $ echo いろはにほへと > message.txt
 *
 * $ ./encrypt Mfx+uhX0C+U0MH7CfP0UTe0amwD5XavdkAVzCXkEWf9l+N6YR3okUw== message.txt message.txt.cip
 * \endcode
 * \section pstsdecrypt decrypt
 * `usage: decrypt privateKey cipherFile messageFile`\n
 * \n
 * `decrypt`は暗号文から平文を復号します。\n
 * 引数には受信者の秘密鍵、暗号文ファイル名、平文ファイル名を渡します。\n
 * \n
 * 実行した例を示します。\n
 * \code
 * $ ./decrypt QLRqG8K4cy/g106ocTQKTIF23bk= message.txt.cip message.txt.cip.txt
 * 
 * $ cat message.txt.cip.txt
 * いろはにほへと
 * \endcode
 * \section pstssign sign
 * `usage: sign privateKey messageFile`\n
 * \n
 * `sign`は平文に対する署名文を表示します。\n
 * 引数には送信者の秘密鍵、平文ファイル名を渡します。\n
 * \n
 * 実行した例を示します。\n
 * \code
 * $ ./sign QLRqG8K4cy/g106ocTQKTIF23bk= message.txt
 * qHqZOT7qPHuqCL+9gUCaKiRSzheR5Oa3DcT6Dg2WRvsGAvtGF3G1wELn9F+qS0CFrBy4NgOShriILXOt
 * \endcode
 * \section pstsverify verify
 * `usage: verify publicKey signature messageFile`\n
 * \n
 * `verify`は署名文を検証します。\n
 * 正しければ`OK`、間違っていれば`NG`と表示します。\n
 * 引数には送信者の公開鍵、署名文、平文ファイル名を渡します。\n
 * \n
 * 実行した例を示します。\n
 * \code
 * $ ./verify Mfx+uhX0C+U0MH7CfP0UTe0amwD5XavdkAVzCXkEWf9l+N6YR3okUw== qHqZOT7qPHuqCL+9gUCaKiRSzheR5Oa3DcT6Dg2WRvsGAvtGF3G1wELn9F+qS0CFrBy4NgOShriILXOt message.txt
 * OK
 * \endcode
 * <a href="modules.html"><b>モジュールへ</b></a>\n
 */
#ifndef ECCRYPTO_H
#define ECCRYPTO_H

#include <algorithm>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <functional>
#include <ios>
#include <iterator>
#include <memory>
#include <mutex>
#include <ostream>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

/*!\brief 楕円曲線暗号を用いた暗号システムの構築を支援する。
 *
 * \section neccclasses クラス
 * クラスは以下の2つに分類される。\n
 * - 「参照」クラス: new演算でインスタンス化され、ポインタを介して扱わ<!--
 *   -->れ、非constである。\n
 *   例えば`ecc::PrimeField`クラスが属する。\n
 * - 「値」クラス: 直接インスタンス化され、演算子を定義でき、const、非<!--
 *   -->constを使い分けることができる。\n
 *   例えば`ecc::rational_point`クラスが属する。\n
 * .
 * \section neccnaming 命名規則
 * 命名は以下の規則に従う。\n
 * - 参照クラス名は先頭が大文字の単語をつなげる。\n
 *   例えば`ecc::EllipticCurve`となる。\n
 * - 値クラス名は先頭が小文字の単語をアンダーバーでつなげる。\n
 *   例えば`ecc::big_int`となる。\n
 * - メンバ名は属しているクラスの名前と同じ規則に従う。\n
 *   ただし参照クラスのメンバ名の頭文字は小文字とする。\n
 *   例えば`ecc::EllipticCurve::coefficientA`となる。\n
 * - グローバル関数名は値クラスと同じ規則に従う。\n
 *   例えば`ecc::numbers_to_bytes`となる。\n
 * - 仮引数とローカル変数の名前は参照クラスのメンバ名と同じ規則に従う。\n
 * - テンプレート引数名は単語の頭文字をつなげる。\n
 *   例えば`First Input`なら`FI`となる。\n
 * - constexpr、static const修飾するものの名前はすべて大文字の単語をア<!--
 *   -->ンダーバーでつなげる。\n
 *   例えば`ecc::big_int::NUMBER_OF_FIELDS`となる。\n
 * .
 */
namespace ecc {
    //---- forward declaration ----

    template <typename O> class AbstractInputProcess;
    template <typename I> class AbstractOutputProcess;
    template <typename V> class EllipticCurve;
    template <typename O> class InputProcess;
    template <typename I, typename O> class InputOutputProcess;
    template <typename V> class Joint;
    template <typename I> class OutputProcess;
    template <typename V> class PrimeField;
    class Process;
    class PseudoRandomBitGenerator;
    template <typename V, class RBG> class RandomNumberGenerator;
    template <size_t S, bool C> class big_int;
    template <typename V> class element;
    class eof_exception;
    template <typename V, class P> class factory;
    class initializer;
    template <typename V> class rational_point;

    /*!\brief ビットを入力する関数。
     * \return 入力したビット。\n
     */
    using get_bit = std::function<bool()>;

    /*!\brief バイトを入力する関数。
     * \return 入力したバイト。\n
     */
    using get_byte = std::function<int()>;

    /*!\brief 整数を入力する関数。
     * \tparam V 整数の型。\n
     * \return 入力した整数。\n
     */
    template <typename V> using get_number = std::function<V()>;

    /*!\brief 値を入力する関数。
     * \tparam V 値の型。\n
     * \return 入力した値。\n
     */
    template <typename V> using get_value = std::function<V()>;

    /*!\brief ビットを出力する関数。
     * \param bit 出力するビット。\n
     */
    using put_bit = std::function<void(const bool& bit)>;

    /*!\brief バイトを出力する関数。
     * \param byt 出力するバイト。\n
     */
    using put_byte = std::function<void(const int& byt)>;

    /*!\brief 整数を出力する関数。
     * \tparam V 整数の型。\n
     * \param num 出力する整数。\n
     */
    template <typename V> using put_number =
        std::function<void(const V& num)>;

    /*!\brief データ片を出力する関数。
     * \tparam V データ片の型。\n
     * \param num 出力するデータ片。\n
     */
    template <typename V> using put_value =
        std::function<void(const V& val)>;

    /*!\brief ビットを読み込んでビットを出力する処理関数。
     * \param getBit ビットを入力する関数。\n
     * \param putBit ビットを出力する関数。\n
     */
    using bit_to_bit = std::function<void(
        const get_bit& getBit,
        const put_bit& putBit
    )>;

    /*!\brief ビットを読み込んでバイトを出力する処理関数。
     * \param getBit ビットを入力する関数。\n
     * \param putByt バイトを出力する関数。\n
     */
    using bit_to_byte = std::function<void(
        const get_bit& getBit,
        const put_byte& putByt
    )>;

    /*!\brief バイトを読み込んでビットを出力する処理関数。
     * \param getByt バイトを入力する関数。\n
     * \param putBit ビットを出力する関数。\n
     */
    using byte_to_bit = std::function<void(
        const get_byte& getByt,
        const put_bit& putBit
    )>;

    /*!\brief バイトを読み込んでバイトを出力する処理関数。
     * \param getByt バイトを入力する関数。\n
     * \param putByt バイトを出力する関数。\n
     */
    using byte_to_byte = std::function<void(
        const get_byte& getByt,
        const put_byte& putByt
    )>;

    /*!\brief バイトを読み込んで整数を出力する処理関数。
     * \tparam V 整数の型。\n
     * \param getByt バイトを入力する関数。\n
     * \param putNum 整数を出力する関数。\n
     */
    template <typename V> using byte_to_number = std::function
        <void(const get_byte& getByt, const put_number<V>& putNum)>;

    /*!\brief 整数を読み込んでバイトを出力する処理関数。
     * \tparam V 整数の型。\n
     * \param getNum 整数を入力する関数。\n
     * \param putByt バイトを出力する関数。\n
     */
    template <typename V> using number_to_byte = std::function
        <void(const get_number<V>& getNum, const put_byte& putByt)>;

    /*!\brief 整数を読み込んで整数を出力する処理関数。
     * \tparam I 入力する整数の型。\n
     * \tparam O 出力する整数の型。\n
     * \param getNum 整数を入力する関数。\n
     * \param putNum 整数を出力する関数。\n
     */
    template <typename I, typename O> using number_to_number =
        std::function<void(
            const get_number<I>& getNum,
            const put_number<O>& putNum
        )>;

    /*!\brief 値を読み込んで値を出力する処理関数。
     * \tparam I 入力する値の型。\n
     * \tparam O 出力する値の型。\n
     * \param getNum 値を入力する関数。\n
     * \param putNum 値を出力する関数。\n
     */
    template <typename I, typename O> using value_to_value =
        std::function<void(
            const get_value<I>& getVal,
            const put_value<O>& putVal
        )>;

    //---- ecurve declaration ----

    /*!\defgroup mecurve 楕円曲線
     * このモジュールには楕円曲線暗号システムの核となる4つのクラスが含<!--
     * -->まれる。\n
     * これらのクラスは以下のような階層関係をなし、下位のオブジェクト<!--
     * -->は上位のオブジェクトに依存する。\n
     * - `ecc::PrimeField`
     *   + `ecc::element`
     *   + `ecc::EllipticCurve`
     *     + `ecc::rational_point`
     * .
     * ファクトリーを構築する際に`ecc::element`は`ecc::PrimeField`、<!--
     * -->`ecc::rational_point`は`ecc::EllipticCurve`を渡す。\n
     * \code
     * auto F = std::make_shared<ecc::PrimeField<int>>(7);
     * auto fe = ecc::make_factory<ecc::element<int>>(F.get());
     * auto E = std::make_shared<ecc::EllipticCurve<int>>(F.get(), fe(2), fe(6));
     * auto rp = ecc::make_factory<ecc::rational_point<int>>(E.get());
     * auto B = rp(fe(1), fe(3));
     * \endcode
     * スカラー倍算は`ecc::element`と`ecc::rational_point`を乗算するこ<!--
     * -->とで行う。 \n
     * 左右のオペランドは可換である。\n
     * \code
     * auto G = std::make_shared<ecc::PrimeField<int>>(11);
     * auto ge = ecc::make_factory<ecc::element<int>>(G.get());
     * auto P = ge(10) * B;
     * auto Q = B * ge(10);
     * assert(P == Q);
     * \endcode
     @{*/

    /*!\brief 素体を表す。
     *
     * 位数を保持する。\n
     * `ecc::element`と`ecc::EllipticCurve`の生成源である。\n
     * \tparam V 値の型(整数型)。\n
     */
    template <typename V> class PrimeField {
    public:
        /*!\brief インスタンスを構築する。
         * \param ord 位数。\n
         * \warning `ord`は素数でなければならない。\n
         */
        PrimeField(const V& ord);

        /*!\brief 位数を取得する。
         * \return 取得した位数。\n
         */
        V order();
    private:
        V ord_;
    };

    /*!\brief 素体の元を表す。
     *
     * 値、生成源である`ecc::PrimeField`へのバックポインタを保持する。\n
     * 演算はモジュラー算術によって行われる。\n
     * \tparam V 値の型(整数型)。\n
     */
    template <typename V> class element {
    public:
        /*!\brief デフォルトコンストラクタ。
         * \post 値の内容は未定義となる。\n
         */
        element() = default;

        /*!\brief コピーコンストラクタ。
         * \param ano 他方のインスタンス。\n
         */
        element(const element& ano) = default;

        /*!\brief 素体、値からインスタンスを構築する。
         *
         * 基本的に`ecc::factory::operator()`からしか呼ばれない。\n
         * \code
         * auto F = std::make_shared<ecc::PrimeField<int>>(7);
         * auto e_1 = ecc::element<int>(F.get(), 5); // 煩雑
         * auto fe = ecc::make_factory<ecc::element<int>>(F.get());
         * auto e_2 = fe(5);                         // 単純
         * \endcode
         * \param priFie 素体。\n
         * \param val 値。\n
         */
        element(PrimeField<V>*const priFie, const V& val);

        /*!\brief 素体、元からインスタンスを構築する。
         *
         * 素体を入れ替えるときに使う。\n
         * \code
         * auto F = std::make_shared<ecc::PrimeField<int>>(7);
         * auto fe = ecc::make_factory<ecc::element<int>>(F.get());
         * auto e_1 = fe(5);   // 5 mod 7
         * auto G = std::make_shared<ecc::PrimeField<int>>(11);
         * auto ge = ecc::make_factory<ecc::element<int>>(G.get());
         * auto e_2 = ge(e_1); // 5 mod 11
         * \endcode
         * \param priFie 素体。\n
         * \param ele 元。\n
         */
        element(PrimeField<V>*const priFie, const element& ele);

        /*!\brief 等しくないかどうかを判定する。
         * \param rhs 右側のオペランド。\n
         * \return 等しくなければ真、等しければ偽。\n
         */
        bool operator!=(const element& rhs) const;

        /*!\brief モジュラー算術で乗算する。
         *
         * 積の値は素体の位数を法とした剰余になる。\n
         * 素体は左側のものを使い、右側のものは無視される。\n
         * \param rhs 乗数。\n
         * \return 計算した積。\n
         */
        element operator*(const element& rhs) const;

        /*!\brief スカラー倍算する。
         * \param rhs 有理点。\n
         * \return 計算したスカラー倍点。\n
         * \remarks 元と有理点を入れ替えても同じ結果となる。\n
         */
        rational_point<V> operator*(const rational_point<V>& rhs) const;

        /*!\brief モジュラー算術で乗算し、代入する。
         *
         * 積の値は素体の位数を法とした剰余になる。\n
         * 素体は左側のものを使い、右側のものは無視される。\n
         * \param rhs 乗数。\n
         * \return 計算した積を代入した被乗数。\n
         */
        element& operator*=(const element& rhs);

        /*!\brief モジュラー算術で加算する。
         *
         * 和の値は素体の位数を法とした剰余になる。\n
         * 素体は左側のものを使い、右側のものは無視される。\n
         * \param rhs 加数。\n
         * \return 計算した和。\n
         */
        element operator+(const element& rhs) const;

        /*!\brief モジュラー算術で加算し、代入する。
         *
         * 和の値は素体の位数を法とした剰余になる。\n
         * 素体は左側のものを使い、右側のものは無視される。\n
         * \param rhs 加数。\n
         * \return 計算した和を代入した被加数。\n
         */
        element& operator+=(const element& rhs);

        /*!\brief モジュラー算術で正負を反転する。
         *
         * 正負を反転した値は素体の位数を法とした剰余になる。\n
         * \return 正負を反転した結果。\n
         */
        element operator-() const;

        /*!\brief モジュラー算術で減算する。
         *
         * 差の値は素体の位数を法とした剰余になる。\n
         * 素体は左側のものを使い、右側のものは無視される。\n
         * \param rhs 減数。\n
         * \return 計算した差。\n
         */
        element operator-(const element& rhs) const;

        /*!\brief モジュラー算術で減算し、代入する。
         *
         * 差の値は素体の位数を法とした剰余になる。\n
         * 素体は左側のものを使い、右側のものは無視される。\n
         * \param rhs 減数。\n
         * \return 計算した差を代入した被減数。\n
         */
        element& operator-=(const element& rhs);

        /*!\brief モジュラー算術で除算する。
         *
         * 商の値は素体の位数を法とした剰余になる。\n
         * 素体は左側のものを使い、右側のものは無視される。\n
         * \param rhs 除数。\n
         * \return 計算した商。\n
         * \warning `rhs`は非`0`でなければならない。\n
         */
        element operator/(const element& rhs) const;

        /*!\brief モジュラー算術で除算し、代入する。
         *
         * 商の値は素体の位数を法とした剰余になる。\n
         * 素体は左側のものを使い、右側のものは無視される。\n
         * \param rhs 除数。\n
         * \return 計算した商を代入した被除数。\n
         * \warning `rhs`は非`0`でなければならない。\n
         */
        element& operator/=(const element& rhs);

        /*!\brief 代入する。
         * \param rhs 右側のオペランド。\n
         */
        element& operator=(const element& rhs) = default;

        /*!\brief 等しいかどうかを判定する。
         * \param rhs 右側のオペランド。\n
         * \return 等しければ真、等しくなければ偽。\n
         */
        bool operator==(const element& rhs) const;

        /*!\brief 値が非`0`かどうかを判定する。
         * \return 非`0`なら真、`0`なら偽。\n
         */
        operator bool() const;

        /*!\brief 値を取得する。
         *
         * 値は必ず`0`以上、素体の位数未満の範囲内となる。\n
         * \return 取得した値。\n
         */
        V value() const;
    private:
        PrimeField<V>* pri_fie_;
        V val_;
        template <typename V_> friend std::ostream& operator<<
            (std::ostream& os, const element<V_>& ele);
    };

    /*!\brief 素体上の楕円曲線を表す。
     *
     * 係数、生成源である`ecc::PrimeField`へのバックポインタを保持する。\n
     * `ecc::rational_point`の生成源である。\n
     * \tparam V 値の型(整数型)。\n
     */
    template <typename V> class EllipticCurve {
    public:
        /*!\brief 素体、係数からインスタンスを構築する。
         * \param priFie 素体。\n
         * \param coeA 1次の項の係数。\n
         * \param coeB 0次の項の係数。\n
         */
        EllipticCurve(
            PrimeField<V>*const priFie,
            const element<V>& coeA,
            const element<V>& coeB
        );

        /*!\brief 1次の項の係数を取得する。\n
         * \return 取得した1次の項の係数。\n
         */
        element<V> coefficientA();

        /*!\brief 0次の項の係数を取得する。\n
         * \return 取得した0次の項の係数。\n
         */
        element<V> coefficientB();

        /*!\brief 素体を取得する。
         * \return 取得した素体。\n
         */
        PrimeField<V>* primeField();
    private:
        element<V> coeA_, coeB_;
        PrimeField<V>* priFie_;
    };

    /*!\brief 楕円曲線上の有理点を表す。
     *
     * 無限遠点フラグ、座標、生成源である`ecc::EllipticCurve`へのバッ<!--
     * -->クポインタを保持する。\n
     * 座標はヤコビアン座標系(x, y, z)である。\n
     * アフィン座標系(X, Y)とは次のような関係になる。\n
     * \n
     * \f$ (x, y, z) = ( \frac{x}{z^2}, \frac{y}{z^3}, 1) = (X, Y, 1) \f$\n
     * \n
     * \tparam V 値の型(整数型)。\n
     */
    template <typename V> class rational_point {
    public:
        /*!\brief 無限遠点を構築する。
         * \post 座標の値は未定義となる。\n
         */
        rational_point();

        /*!\brief 楕円曲線、x座標、y座標からインスタンスを構築する。
         * \param ec 楕円曲線。\n
         * \param x x座標。\n
         * \param y y座標。\n
         * \warning 座標で示される有理点が存在しなければならない。\n
         * \post z座標は`1`となる。\n
         */
        rational_point(
            EllipticCurve<V>*const ec,
            const element<V>& x,
            const element<V>& y
        );

        /*!\brief 楕円曲線、x座標、y座標、z座標からインスタンスを構築<!--
         * -->する。
         * \param ec 楕円曲線。\n
         * \param x x座標。\n
         * \param y y座標。\n
         * \param z z座標。\n
         * \warning 座標で示される有理点が存在しなければならない。\n
         */
        rational_point(
            EllipticCurve<V>*const ec,
            const element<V>& x,
            const element<V>& y,
            const element<V>& z
        );

        /*!\brief コピーコンストラクタ。
         */
        rational_point(const rational_point& ano) = default;

        /*!\brief 無限遠点フラグを取得する。
         * \return 無限遠点なら真、無限遠点以外なら偽。\n
         */
        bool is_infinity() const;

        /*!\brief 等しくないかどうかを判定する。
         * \param rhs 右側のオペランド。\n
         */
        bool operator!=(const rational_point& rhs) const;

        /*!\brief スカラー倍算する。
         * \param rhs 元。\n
         * \return 計算したスカラー倍点。\n
         * \remarks 元と有理点を入れ替えても同じ結果となる。\n
         */
        rational_point operator*(const element<V>& rhs) const;

        /*!\brief スカラー倍算し、代入する。
         * \param rhs 元。\n
         * \return 計算したスカラー倍点を代入した有理点。\n
         * \remarks 元と有理点を入れ替えても同じ結果となる。\n
         */
        rational_point& operator*=(const element<V>& rhs);

        /*!\brief 加算する。
         * \param rhs 加数。\n
         * \return 計算した和。\n
         */
        rational_point operator+(const rational_point& rhs) const;

        /*!\brief 加算し、代入する。
         * \param rhs 加数。\n
         * \return 計算した和を代入した被加数。\n
         */
        rational_point& operator+=(const rational_point& rhs);

        /*!\brief 正負を反転する。
         *
         * y座標の正負だけを反転する。\n
         * \return 正負を反転した結果。\n
         */
        rational_point operator-() const;

        /*!\brief 代入する。
         * \return 代入したインスタンス。\n
         */
        rational_point& operator=(const rational_point& rhs) = default;

        /*!\brief 等しいかどうかを判定する。
         * \param rhs 右側のオペランド。\n
         * \return 等しければ真、等しくなければ偽。\n
         */
        bool operator==(const rational_point& rhs) const;

        /*!\brief アフィン座標系に射影する。
         *
         * 射影した結果の座標は\f$ ( \frac{x}{z^2}, \frac{y}{z^3}, 1) \f$<!--
         * -->となる。\n
         * \return アフィン座標系に射影した結果。\n
         */
        rational_point<V> project() const;

        /*!\brief x座標を取得する。
         * \return 取得したx座標。\n
         */
        element<V> x() const;

        /*!\brief y座標を取得する。
         * \return 取得したy座標。\n
         */
        element<V> y() const;

        /*!\brief z座標を取得する。
         * \return 取得したz座標。\n
         */
        element<V> z() const;
    private:
        EllipticCurve<V>* ec_;
        bool inf_;
        element<V> x_, y_, z_, w_;
        rational_point(
            EllipticCurve<V>*const ec,
            const element<V>& x,
            const element<V>& y,
            const element<V>& z,
            const element<V>& w
        );
        template <typename V_> friend std::ostream& operator<<
            (std::ostream& os, const rational_point<V_>& poi);
    };

    //@}

    //---- random declaration ----

    /*!\defgroup mrandom 乱数
     * このモジュールには乱数を生成するためのクラスが含まれる。\n
     * `ecc::RandomNumberGenerator`は乱数を生成するためにランダムビッ<!--
     * -->ト生成器を用いる。\n
     * 既定のランダムビット生成器は`ecc::PseudoRandomBitGenerator`であ<!--
     * -->る。\n
     * `ecc::PseudoRandomBitGenerator`はメルセンヌ・ツイスタ19937によ<!--
     * -->る擬似ランダムビットを生成する。\n
     * `ecc::RandomNumberGenerator`の2番目のテンプレート引数にランダム<!--
     * -->ビット生成器の型を渡すことで生成方法を選ぶことができる。\n
     @{*/

    /*!\brief 擬似ランダムビット生成器を表す。
     *
     * 生成方法はメルセンヌ・ツイスタ19937である。\n
     */
    class PseudoRandomBitGenerator {
    public:
        //!\brief インスタンスを構築する。
        PseudoRandomBitGenerator();

        /*!\brief 次のランダムビットを取得する。
         * \return 取得した次のランダムビット。\n
         */
        bool getBit();
    private:
        std::uniform_int_distribution<int> dis_;
        std::mt19937 gen_;
    };

    /*!\brief 乱数生成器を表す。
     *
     * 任意の整数型の乱数を生成する。\n
     * \tparam V 値の型(整数型)。\n
     * \tparam RBG ランダムビット生成器の型。既定値は<!--
     * -->`ecc::PseudoRandomBitGenerator`。\n
     * \warning `RBG`はデフォルトコンストラクタ、`getBit`メンバ関数を<!--
     * -->実装していなければならない。\n
     */
    template <typename V, class RBG = PseudoRandomBitGenerator>
        class RandomNumberGenerator
    {
    public:
        //!\brief インスタンスを構築する。
        RandomNumberGenerator();

        /*!\brief 次の乱数を取得する。
         * \param min 最小値。\n
         * \param max 最大値。\n
         * \return 取得した次の乱数。\n
         * \warning `max - min`は`V`における最大値以下でなければならな<!--
         * -->い。\n
         */
        V getNumber(const V& min, const V& max);
    private:
        std::shared_ptr<RBG> rbg_;
    };

    //@}

    //---- hash declaration ----

    /*!\defgroup mhash ハッシュ
     * このモジュールには平文のハッシュ値を計算するための関数が含まれ<!--
     * -->る。\n
     * これらの関数は`ecc::InputOutputProcess`に渡すための処理関数を作<!--
     * -->る。\n
     @{*/

    uint32_t ch(const uint32_t& x, const uint32_t& y, const uint32_t& z);
    uint32_t lcsig0(const uint32_t& x);
    uint32_t lcsig1(const uint32_t& x);
    uint32_t maj(const uint32_t& x, const uint32_t& y, const uint32_t& z);
    uint32_t parity
        (const uint32_t& x, const uint32_t& y, const uint32_t& z);
    uint32_t rotl(const uint32_t& x, const size_t& len);
    uint32_t rotr(const uint32_t& x, const size_t& len);

    /*!\brief SHA-1でハッシュ値を計算する処理関数を作る。
     *
     * メッセージはブロック単位で入力する。\n
     * ブロックの長さは512ビット(4バイト×16)である。\n
     * 出力するハッシュ値は160ビット(4バイト×5)である。\n
     * \return 作った処理関数。\n
     * \warning メッセージの末尾にはメッセージ全体の長さをビット長で付<!--
     * -->加しなければならない。\n
     */
    number_to_number<uint32_t, uint32_t> sha1();

    /*!\brief SHA-256でハッシュ値を計算する処理関数を作る。
     *
     * メッセージはブロック単位で入力する。\n
     * ブロックの長さは512ビット(4バイト×16)である。\n
     * 出力するハッシュ値は256ビット(4バイト×8)である。\n
     * \return 作った処理関数。\n
     * \warning メッセージの末尾にはメッセージ全体の長さをビット長で付<!--
     * -->加しなければならない。\n
     */
    number_to_number<uint32_t, uint32_t> sha256();
    uint32_t ucsig0(const uint32_t& x);
    uint32_t ucsig1(const uint32_t& x);

    //@}

    //---- encoding declaration ----

    /*!\defgroup mencoding 符号化
     * このモジュールにはBase64、メッセージに関する関数が含まれる。\n
     * これらの関数は`ecc::InputOutputProcess`に渡すための処理関数を作<!--
     * -->る。\n
     @{*/

    /*!\brief Base64文字列を元のデータに変換する処理関数を作る。
     * \return 作った処理関数。\n
     * \remarks 入力した文字が`([A-Z]|[a-z]|[0-9]|+|/)`以外の場合はス<!--
     * -->キップする。\n
     */
    byte_to_bit base64_decode();

    /*!\brief データをBase64文字列に変換する処理関数を作る。
     * \return 作った処理関数。\n
     */
    bit_to_byte base64_encode();

    /*!\brief メッセージの末尾に詰め物を付加する処理関数を作る。
     *
     * 出力データの大きさはブロックの大きさの倍数になる。\n
     * 詰め物の大きさを`p`、メッセージの大きさを`m`、ブロックの大きさ<!--
     * -->を`b`とすると、`p`と`m`と`b`は次のような関係となる。\n
     * \n
     * \f$ p = b - m \% b \f$\n
     * \n
     * ただしpが9バイト未満の場合はさらにbを加算する。\n
     * \n
     * 詰め物の先頭の１バイトは開始を示す`0x80`とし、続くバイト列を<!--
     * -->`0x00`で埋め、末尾の8バイトはメッセージのビット長を示す整数<!--
     * -->とする。\n
     * ビット長のバイトオーダーはビッグエンディアンとする。\n
     * \param sizBlo ブロックの大きさ(バイト長)。\n
     * \return 作った処理関数。
     */
    byte_to_byte pad_message(const size_t& sizBlo);

    /*!\brief メッセージから詰め物を取り去る処理関数を作る。
     *
     * 入力データの末尾に付加された詰め物の末尾の8バイトをメッセージの<!--
     * -->ビット長とみなし、メッセージの後に付加された詰め物を出力しな<!--
     * -->いようにする。\n
     * \param sizBlo ブロックの大きさ(バイト長)。\n
     * \return 作った処理関数。
     * \remarks 入力データの末尾に詰め物が付加されていない場合は適当に<!--
     * -->出力を打ち切る。\n
     */
    byte_to_byte strip_message(const size_t& sizBlo);

    //@}

    //---- streaming declaration ----

    /*!\defgroup mstreaming ストリーミング
     * このモジュールにはストリーミングラインを構築するためのクラスが<!--
     * -->含まれる。\n
     * これらのクラスは`ecc::Process`をルートとする階層をなす。\n
     * 基底となる3つの抽象クラスはそれぞれ以下のような機能を持つ。\n
     * - `ecc::Process`: スレッド\n
     * - `ecc::AbstractInputProcess`: 入力\n
     * - `ecc::AbstractOutputProcess`: 出力\n
     * .
     * ラインの構築に使う3つの具象クラスはこれらの抽象クラスの機能を組<!--
     * -->み合わせることで実現している。\n
     @{*/

    template <typename V> class Joint {
    public:
        Joint(const size_t& cap);
        void close();
        std::function<V()> getter();
        void initialize();
        std::function<void(const V& val)> putter();
    private:
        size_t cap_;
        std::condition_variable conVar_;
        bool eof_;
        std::queue<V> excBuf_;
        std::queue<V> inpBuf_;
        std::mutex mut_;
        std::queue<V> outBuf_;
    };

    /*!\brief ストリーミングにおける工程を表す。
     *
     * 工程のスレッドを保持する。\n
     * 工程のスレッドを起動する。\n
     * 工程のスレッドに合流する。\n
     */
    class Process {
    public:
        /*!\brief 初期化する。
         *
         * 何もしない。\n
         */ 
        virtual void initialize();

        /*!\brief 工程のスレッドに合流する。
         *
         * 呼んだスレッドは工程のスレッドが終了するまでブロックされる。\n
         */
        virtual void join();

        /*!\brief 工程のスレッドを起動する。
         *
         * 起動された工程のスレッドは処理関数を実行する。\n
         * \pre 初期化済みでなければならない。\n
         */
        virtual void start();
    protected:
        Process() = default;
        virtual void enter() = 0;
    private:
        std::thread the_;
    };

    /*!\brief ストリーミングにおける抽象的な入力の工程を表す。
     *
     * 直後の工程と同期し、データ片を出力する。\n
     * \tparam O 直後の工程に出力するデータ片の型。\n
     */
    template <typename O> class AbstractInputProcess :
        public virtual Process
    {
    public:
        /*!\brief 初期化する。
         *
         * 直後の工程とデータ片を入出力できるように準備する。\n
         */
        virtual void initialize() override;
        Joint<O>* outputJoint();
    protected:
        std::shared_ptr<Joint<O>> outJoi_;
        AbstractInputProcess(const size_t& cap);
    };

    /*!\brief ストリーミングにおける抽象的な出力の工程を表す。
     *
     * 直前の工程と同期し、データ片を入力する。\n
     * \tparam I 直前の工程から入力するデータ片の型。\n
     */
    template <typename I> class AbstractOutputProcess :
        public virtual Process
    {
    public:
        void inputProcess
            (const std::shared_ptr<AbstractInputProcess<I>>& inpPro);
        /*!\brief 工程のスレッドに合流する。
         *
         * 自分の工程のスレッドと自分より前にあるすべての工程のスレッ<!--
         * -->ドに合流する。\n
         * 呼んだスレッドは自分の工程のスレッドと自分より前にあるすべ<!--
         * -->ての工程のスレッドが終了するまでブロックされる。\n
         */
        virtual void join() override;

        /*!\brief 工程のスレッドを起動する。
         *
         * 自分の工程のスレッドと自分より前にあるすべての工程のスレッ<!--
         * -->ドを起動する。\n
         * 起動された工程のスレッドは処理関数を実行する。\n
         * \pre 自分と自分より前にあるすべての工程が初期化済みでなけれ<!--
         * -->ばならない。\n
         */
        virtual void start() override;
    protected:
        std::shared_ptr<AbstractInputProcess<I>> inpPro_;
    };

    /*!\brief ストリーミングにおける入力の工程を表す。
     *
     * ラインの先頭に配置する。\n
     * 処理関数を保持する。\n
     * 工程のスレッドが起動されると処理関数を呼ぶ。\n
     * \tparam O 直後の工程に出力するデータ片の型。\n
     */
    template <typename O> class InputProcess :
        public AbstractInputProcess<O>
    {
    public:
        /*!\brief 処理関数。
         *
         * 工程のスレッドで実行される処理を定義する。\n
         * \param putVal 出力関数。\n
         */
        using procedure = std::function<void(const put_value<O>& putVal)>;

        /*!\brief インスタンスを構築する。
         * \param pro 処理関数。\n
         * \param cap 出力するデータ片をバッファリングする数。既定値は<!--
         * -->`1024`．\n
         * \remarks まだ工程のスレッドは起動しない。\n
         */
        InputProcess(const procedure& pro, const size_t& cap = 1024);
    private:
        using super = AbstractInputProcess<O>;
        procedure pro_;
        virtual void enter() override;
    };

    /*!\brief ストリーミングにおける入出力の工程を表す。
     *
     * ラインの中間に配置する。\n
     * 処理関数を保持する。\n
     * 工程のスレッドが起動されると処理関数を呼ぶ。\n
     * \tparam I 直前の工程から入力するデータ片の型。\n
     * \tparam O 直後の工程に出力するデータ片の型。\n
     */
    template <typename I, typename O> class InputOutputProcess :
        public AbstractInputProcess<O>, public AbstractOutputProcess<I>
    {
    public:
        /*!\brief 処理関数。
         *
         * 工程のスレッドで実行される処理を定義する。\n
         * \param getVal 入力関数。\n
         * \param putVal 出力関数。\n
         */
        using procedure = std::function<void(
            const get_value<I>& getVal, const put_value<O>& putVal
        )>;

        /*!brief インスタンスを構築する。
         * \param pro 処理関数。
         * \param cap 出力するデータ片をバッファリングする数。既定値は<!--
         * -->`1024`．\n
         * \remarks まだ工程のスレッドは起動しない。\n
         */
        InputOutputProcess
            (const procedure& pro, const size_t& cap = 1024);
    private:
        using superInput = AbstractInputProcess<O>;
        using superOutput = AbstractOutputProcess<I>;
        procedure pro_;
        virtual void enter() override;
    };

    /*!\brief ストリーミングにおける出力の工程を表す。
     *
     * ラインの末尾に配置する。\n
     * 処理関数を保持する。\n
     * 工程のスレッドが起動されると処理関数を呼ぶ。\n
     * \tparam I 直前の工程から入力するデータ片の型。\n
     */
    template <typename I> class OutputProcess :
        public AbstractOutputProcess<I>
    {
    public:
        /*!\brief 処理関数。
         *
         * 工程のスレッドで実行される処理を定義する。\n
         * \param getVal 入力関数。\n
         */
        using procedure = std::function<void(const get_value<I>& getVal)>;

        /*!brief インスタンスを構築する。
         * \param pro 処理関数。
         * \remarks まだ工程のスレッドは起動しない。\n
         */
        OutputProcess(const procedure& pro);

        /*!\brief ストリーミングラインを実行する。
         *
         * ライン上にあるすべての工程を初期化し、工程のスレッドを起動<!--
         * -->し、合流する。\n
         * 呼んだスレッドはすべての工程のスレッドが終了するまでブロッ<!--
         * -->クされる。\n
         */
        void run();
    private:
        using super = AbstractOutputProcess<I>;
        procedure pro_;
        virtual void enter() override;
    };

    //!\brief EOFによる処理の終了を表す。
    class eof_exception : public std::exception {
    public:
        /*!\brief 型名を取得する。
         * \return 取得した型名。\n
         */
        virtual const char* what() const throw() override;
    };

    /*!\brief 任意の数の工程を連結する。
     * \tparam F 1番目の工程の型。\n
     * \tparam S 2番目の工程の型。\n
     * \tparam PS 3番目以降の工程の型。\n
     * \param firPro 1番目の工程。\n
     * \param secPro 2番目の工程。\n
     * \param pros 3番目以降の工程。\n
     * \return 末尾の工程。\n
     */
    template <class F, class S, class ...PS> auto concatinate
        (const F& firPro, const S& secPro, PS&& ...pros) ->
            decltype(concatinate(secPro, pros...));

    /*!\brief 入出力の工程を連結する。
     * \tparam FI 1番目の工程が入力するデータ片の型。\n
     * \tparam FO 1番目の工程が出力するデータ片の型。\n
     * \tparam SO 2番目の工程が出力するデータ片の型。\n
     * \param firInpOutPro 1番目の入出力の工程。\n
     * \param secInpOutPro 2番目の入出力の工程。\n
     * \return 2番目の入出力の工程。\n
     */
    template <typename FI, typename FO, typename SO> auto concatinate(
        const std::shared_ptr<InputOutputProcess<FI, FO>>& firInpOutPro,
        const std::shared_ptr<InputOutputProcess<FO, SO>>& secInpOutPro
    ) -> decltype(secInpOutPro);

    /*!\brief 入力の工程と入出力の工程を連結する。
     * \tparam FO 1番目の工程が出力するデータ片の型。\n
     * \tparam SO 2番目の工程が出力するデータ片の型。\n
     * \param firInpPro 1番目の入力の工程。\n
     * \param secInpOutPro 2番目の入出力の工程。\n
     * \return 2番目の入出力の工程。\n
     */
    template <typename FO, typename SO> auto concatinate(
        const std::shared_ptr<InputProcess<FO>>& firInpPro,
        const std::shared_ptr<InputOutputProcess<FO, SO>>& secInpOutPro
    ) -> decltype(secInpOutPro);

    /*!\brief 入出力の工程と出力の工程を連結する。
     * \tparam FI 1番目の工程が入力するデータ片の型。\n
     * \tparam FO 1番目の工程が出力するデータ片の型。\n
     * \param firInpOutPro 1番目の入出力の工程。\n
     * \param secOutPro 2番目の出力の工程。\n
     * \return 2番目の出力の工程。\n
     */
    template <typename FI, typename FO> auto concatinate(
        const std::shared_ptr<InputOutputProcess<FI, FO>>& firInpOutPro,
        const std::shared_ptr<OutputProcess<FO>>& secOutPro
    ) -> decltype(secOutPro);

    /*!\brief 入力の工程と出力の工程を連結する。
     * \tparam FO 1番目の工程が出力するデータ片の型。\n
     * \param firInpPro 1番目の入力の工程。\n
     * \param secOutPro 2番目の出力の工程。\n
     * \return 2番目の出力の工程。\n
     */
    template <typename FO> auto concatinate(
        const std::shared_ptr<InputProcess<FO>>& firInpPro,
        const std::shared_ptr<OutputProcess<FO>>& secOutPro
    ) -> decltype(secOutPro);

    /*!\brief 入力関数から入力工程の処理関数を作る。
     * \tparam O 出力するデータ片の型。\n
     * \param getVal 入力関数。\n
     * \return 作った処理関数。\n
     */
    template <typename O> typename InputProcess<O>::procedure
        input_from(const get_value<O>& getVal);

    /*!\brief 出力関数から出力工程の処理関数を作る。
     * \tparam I 入力するデータ片の型。\n
     * \param putVal 出力関数。\n
     * \return 作った処理関数。\n
     */
    template <typename I> typename OutputProcess<I>::procedure
        output_to(const put_value<I>& putVal);

    /*!\brief 変換関数から入出力工程の処理関数を作る。
     * \tparam I 入力するデータ片の型。\n
     * \tparam O 出力するデータ片の型。\n
     * \tparam T 変換関数の型。\n
     * \param tra 変換関数。\n
     * \return 作った処理関数。\n
     */
    template <typename I, typename O, typename T>
        typename InputOutputProcess<I, O>::procedure
            transform(const T& tra);

    //@}

    //---- binteger declaration -----

    /*!\defgroup mbinteger 多倍長整数
     * このモジュールには`ecc::big_int`のみが含まれる。\n
     * \n
     * `ecc::big_int`は`int`などの組み込み整数型と同様に扱うことができ<!--
     * -->るので、整数型を要求するクラステンプレートや関数テンプレート<!--
     * -->に渡すことができる。\n
     * \n
     * 演算するよりもフィールドに直接アクセスしたほうが速い場合がある。\n
     * \code
     * auto num = ecc::big_int<20>("1461501637330902918203684832716283019653785059327");
     * int a = int(num >> 144) & 0xff; // 遅い
     * int b = (num[4] >> 16) & 0xff;  // 速い
     * \endcode
     */
    //@{

    /*!\brief 多倍長整数。
     *
     * 任意の大きさの整数を表す。\n
     * \section mbicbilsize 論理的な大きさ
     * 整数同士の演算において結果の大きさが最も大きくなるのは乗算であ<!--
     * -->る。\n
     * 乗数と被乗数の大きさを最大で`N`ビットとすると結果の大きさは最大<!--
     * -->で`2N`ビットになる。\n
     * \code
     * int a =  1 *  1; //    1(1) ×    1(1) =        1(1)
     * int b =  3 *  3; //   11(2) ×   11(2) =     1001(4)
     * int c =  7 *  7; //  111(3) ×  111(3) =    11001(5)
     * int d = 15 * 15; // 1111(4) × 1111(4) = 11100001(8)
     * \endcode
     * 予期せぬオーバーフローを防ぐために`2N`ビットの記憶領域を確保し<!--
     * -->ておき、`N`ビットまでの整数を演算する。\n
     * また`N`に符号ビットを含めない場合はさらに`1`ビットを必要とする。\n
     * この`2N+1`を物理的な大きさとすると`N`は論理的な大きさと言える。\n
     * \section mbicbifield フィールド
     * フィールドは整数のデータ片である。\n
     * フィールドはできるだけ大きいほうが、計算量が少なく効率的である。\n
     * したがって組み込み整数型の中で最も大きい64ビット整数とする。\n
     * \n
     * 多倍長整数同士を演算する際は左右のオペランドのフィールド同士を<!--
     * -->そのまま演算する。\n
     * 乗算でオーバーフローしないように、フィールドは64ビットのうち下<!--
     * -->位32ビットだけを使い、上位32ビットは`0`で埋める。\n
     * \n
     * 整数のビット列は32ビットごとに分割され、最下位から順にフィール<!--
     * -->ドの配列に格納される。\n
     * フィールド内のバイトオーダーはリトルエンディアンとする。\n
     * \section mbicbinegative 負
     * 負の値は2の補数で表す。\n
     * 正負は最上位の符号ビットで区別できる。\n
     * 負の場合でもフィールドの上位32ビットは`0`で埋める。\n
     * \tparam S 多倍長整数の論理的な大きさ(バイト長)。\n
     * \tparam C Sに符号ビットを含めるかどうかのフラグ。真なら含める、<!--
     * -->偽なら含めない。既定値は偽。\n
     * \warning `S`は`1`以上でなければならない。\n
     */
    template <size_t S, bool C = false> class big_int {
    public:
        //!\brief 多倍長整数の論理的な大きさ(バイト長)。
        static constexpr size_t LOGICAL_SIZE = S;

        //!\brief フィールドの大きさ(バイト長)。
        static constexpr size_t SIZE_OF_FIELD = sizeof(uint64_t) / 2;

        //!\brief フィールドの長さ(ビット長)。
        static constexpr size_t LENGTH_OF_BITS_ON_FIELD =
            8 * SIZE_OF_FIELD;

        //!\brief フィールドを抽出するためのマスク。
        static constexpr uint64_t FIELD_MASK =
            (uint64_t(1) << LENGTH_OF_BITS_ON_FIELD) - 1;

        //!\brief フィールドの数。
        static constexpr size_t NUMBER_OF_FIELDS =
            LOGICAL_SIZE < SIZE_OF_FIELD ?  2 :
                (LOGICAL_SIZE * 2 + (C ? 0 : 1) + SIZE_OF_FIELD - 1) /
                    SIZE_OF_FIELD;

        /*!\brief デフォルトコンストラクタ。
         * \post すべてのフィールドの値は未定義となる。\n
         */
        big_int() = default;

        /*!\brief コピーコンストラクタ。
         * \param ano 他方のインスタンス。\n
         */
        big_int(const big_int& ano);

        /*!\brief `int`整数からインスタンスを構築する。
         * \param num `int`整数。\n
         */
        explicit big_int(const int& num);

        /*!\brief `unsigned`整数からインスタンスを構築する。
         * \param num `unsigned`整数。\n
         */
        explicit big_int(const unsigned& num);

        /*!\brief `int64_t`整数からインスタンスを構築する。
         * \param num `int64_t`整数。\n
         */
        explicit big_int(const int64_t& num);

        /*!\brief `uint64_t`整数からインスタンスを構築する。
         * \param num `uint64_t`整数。\n
         */
        explicit big_int(const uint64_t& num);

        /*!\brief 文字列からインスタンスを構築する。
         * \param str 文字列。\n
         * \param bas 基数のフラグ。既定値は`std::ios::dec`。\n
         * \warning 文字が`[A-Z]|[a-z]|[0-9]`以外の場合の動作は未定義<!--
         * -->となる。\n
         */
        explicit big_int(
            const char*const str,
            const std::ios::fmtflags& bas = std::ios::dec
        );

        /*!\brief 文字列からインスタンスを構築する。
         * \param str 文字列。\n
         * \param bas 基数のフラグ。既定値は`std::ios::dec`。\n
         * \warning 文字が`[A-Z]|[a-z]|[0-9]`以外の場合の動作は未定>義!--
         * -->となる。\n
         */
        explicit big_int(
            const std::string& str,
            const std::ios::fmtflags& bas = std::ios::dec
        );

        /*!\brief 文字の反復子の範囲からインスタンスを構築する。
         * \tparam I 反復子の型。\n
         * \param beg 開始の文字を指す反復子。\n
         * \param end 終了の文字を指す反復子。\n
         * \param bas 基数のフラグ。既定値は`std::ios::dec`。\n
         * \warning 文字が`[A-Z]|[a-z]|[0-9]`以外の場合の動作は未定義<!--
         * -->となる。\n
         */
        template <typename I> big_int(
            const I& beg,
            const I& end,
            const std::ios::fmtflags& bas = std::ios::dec
        );

        /*!\brief ビット論理積を計算する。
         * \param rhs 右側のオペランド。\n
         * \return 計算したビット論理積。\n
         */
        big_int operator&(const big_int& rhs) const;

        /*!\brief ビット論理積を計算し、代入する。
         * \param rhs 右側のオペランド。\n
         * \return ビット論理積を代入した左側のオペランド。\n
         */
        big_int& operator&=(const big_int& rhs);

        /*!\brief 非`0`かどうかを判定する。
         * \return 非`0`なら真、`0`なら偽。\n
         */
        operator bool() const;

        /*!\brief `int`整数に変換する。
         * \return 変換した`int`整数。\n
         * \remarks 最下位部分を取得する。\n
         */
        explicit operator int() const;

        /*!\brief `int64_t`整数に変換する。
         * \return 変換した`int64_t`整数。\n
         * \remarks 最下位部分を取得する。\n
         */
        explicit operator int64_t() const;

        /*!\brief `uint64_t`整数に変換する。
         * \return 変換した`uint64_t`整数。\n
         * \remarks 最下位部分を取得する。\n
         */
        explicit operator uint64_t() const;

        /*!\brief `unsigned`整数に変換する。
         * \return 変換した`unsigned`整数。\n
         * \remarks 最下位部分を取得する。\n
         */
        explicit operator unsigned() const;

        /*!\brief 等しくないかどうかを判定する。
         * \param rhs 右側のオペランド。\n
         * \return 等しくなければ真、等しければ偽。\n
         */
        bool operator!=(const big_int& rhs) const;

        /*!\brief 剰余を計算する。
         * \param rhs 除数。\n
         * \return 計算した剰余。\n
         * \warning `rhs`は非`0`でなければならない。\n
         */
        big_int operator%(const big_int& rhs) const;

        /*!\brief 剰余を計算し、代入する。
         * \param rhs 除数。\n
         * \return 剰余を代入した被除数。\n
         * \warning `rhs`は非`0`でなければならない。\n
         */
        big_int& operator%=(const big_int& rhs);

        /*!\brief 乗算する。
         * \param rhs 乗数。\n
         * \return 計算した積。\n
         */
        big_int operator*(const big_int& rhs) const;

        /*!\brief 乗算し、代入する。
         * \param rhs 乗数。\n
         * \return 計算した積を代入した被乗数。\n
         */
        big_int& operator*=(const big_int& rhs);

        /*!\brief 加算する。
         * \param rhs 加数。\n
         * \return 計算した和。\n
         */
        big_int operator+(const big_int& rhs) const;

        /*!\brief インクリメントする。
         * \return インクリメントした後のインスタンス。\n
         */
        big_int& operator++();

        /*!\brief インクリメントする。
         * \return インクリメントする前のインスタンス。\n
         */
        big_int operator++(int);

        /*!\brief 加算し、代入する。
         * \param rhs 加数。\n
         * \return 計算した和を代入した被加数。\n
         */
        big_int& operator+=(const big_int& rhs);

        /*!\brief 正負を反転する。
         * \return 正負を反転した結果。\n
         */
        big_int operator-() const;

        /*!\brief 減算する。
         * \param rhs 減数。\n
         * \return 計算した差。\n
         */
        big_int operator-(const big_int& rhs) const;

        /*!\brief 減算し、代入する。
         * \param rhs 減数。\n
         * \return 計算した差を代入した被減数。\n
         */
        big_int& operator-=(const big_int& rhs);

        /*!\brief 除算する。
         * \param rhs 除数。\n
         * \return 計算した商。\n
         * \warning `rhs`は非`0`でなければならない。\n
         */
        big_int operator/(const big_int& rhs) const;

        /*!\brief 除算し、代入する。
         * \param rhs 除数。\n
         * \return 計算した商を代入した被除数。\n
         * \warning `rhs`は非`0`でなければならない。\n
         */
        big_int& operator/=(const big_int& rhs);

        /*!\brief 小さいかどうかを判定する。
         * \param rhs 右側のオペランド。\n
         * \return 小さければ真、等しい、または大きければ偽。\n
         */
        bool operator<(const big_int& rhs) const;

        /*!\brief 左にビットシフトする。
         * \param len シフトする長さ(ビット長)。\n
         * \return シフトした結果。\n
         * \remarks `len`が負の場合は右にビットシフトする。\n
         */
        big_int operator<<(const int& len) const;

        /*!\brief 左にビットシフトする。
         * \param len シフトする長さ(ビット長)。\n
         * \return シフトした結果。\n
         */
        big_int operator<<(const size_t& len) const;

        /*!\brief 左にビットシフトし、代入する。
         * \param len シフトする長さ(ビット長)。\n
         * \return シフトした結果を代入した左側のオペランド。\n
         * \remarks `len`が負の場合は右にビットシフトする。\n
         */
        big_int& operator<<=(const int& len);

        /*!\brief 左にビットシフトし、代入する。
         * \param len シフトする長さ(ビット長)。\n
         * \return シフトした結果を代入した左側のオペランド。\n
         */
        big_int& operator<<=(const size_t& len);

        /*!\brief 小さい、または等しいかどうかを判定する。
         * \param rhs 右側のオペランド。\n
         * \return 小さい、または等しければ真、大きければ偽。\n
         */
        bool operator<=(const big_int& rhs) const;

        /*!\brief 代入する。
         * \param rhs 代入するインスタンス。\n
         * \return 代入した左側のオペランド。\n
         */
        big_int& operator=(const big_int& rhs);

        /*!\brief 等しいかどうかを判定する。
         * \param rhs 右側のオペランド。\n
         * \return 等しければ真、等しくなければ偽。\n
         */
        bool operator==(const big_int& rhs) const;

        /*!\brief 大きいかどうかを判定する。
         * \param rhs 右側のオペランド。\n
         * \return 大きければ真、等しい、または小さければ偽。\n
         */
        bool operator>(const big_int& rhs) const;

        /*!\brief 大きい、または等しいかどうかを判定する。
         * \param rhs 右側のオペランド。\n
         * \return 大きい、または等しければ真、小さければ偽。\n
         */
        bool operator>=(const big_int& rhs) const;

        /*!\brief 右にビットシフトする。
         * \param len シフトする長さ(ビット長)。\n
         * \return シフトした結果。\n
         * \remarks `len`が負の場合は左にビットシフトする。\n
         */
        big_int operator>>(const int& len) const;

        /*!\brief 右にビットシフトする。
         * \param len シフトする長さ(ビット長)。\n
         * \return シフトした結果。\n
         */
        big_int operator>>(const size_t& len) const;

        /*!\brief 右にビットシフトし、代入する。
         * \param len シフトする長さ(ビット長)。\n
         * \return シフトした結果を代入した左側のオペランド。\n
         * \remarks `len`が負の場合は左にビットシフトする。\n
         */
        big_int& operator>>=(const int& len);

        /*!\brief 右にビットシフトし、代入する。
         * \param len シフトする長さ(ビット長)。\n
         * \return シフトした結果を代入した左側のオペランド。\n
         */
        big_int& operator>>=(const size_t& len);

        /*!\brief フィールドを参照する。
         * \param index フィールドのインデックス。\n
         * \return 参照したフィールド。\n
         * \remarks フィールドはconst。\n
         * \warning `index`は`0`以上、`NUMBER_OF_FIELDS`未満でなければ<!--
         * -->ならない。\n
         */
        const uint64_t& operator[](const size_t& index) const;

        /*!\brief フィールドを参照する。
         * \param index フィールドのインデックス。\n
         * \return 参照したフィールド。\n
         * \warning `index`は`0`以上、`NUMBER_OF_FIELDS`未満でなければ<!--
         * -->ならない。\n
         * フィールドの上位32ビットを非`0`に変更してはならない。\n
         */
        uint64_t& operator[](const size_t& index);

        /*!\brief ビット排他的論理和を計算する。
         * \param rhs 右側のオペランド。\n
         * \return 計算したビット排他的論理和。\n
         */
        big_int operator^(const big_int& rhs) const;

        /*!\brief ビット排他的論理和を計算し、代入する。
         * \param rhs 右側のオペランド。\n
         * \return 計算したビット排他的論理和を代入した左側のオペラン<!--
         * -->ド。\n
         */
        big_int& operator^=(const big_int& rhs);

        /*!\brief ビット論理和を計算する。
         * \param rhs 右側のオペランド。\n
         * \return 計算したビット論理和。\n
         */
        big_int operator|(const big_int& rhs) const;

        /*!\brief ビット論理和を計算し、代入する。
         * \param rhs 右側のオペランド。\n
         * \return 計算したビット論理和を代入した左側のオペランド。\n
         */
        big_int& operator|=(const big_int& rhs);

        /*!\brief ビット否定を計算する。
         * \return 計算したビット否定。\n
         */
        big_int operator~() const;
    private:
        uint64_t fies_[NUMBER_OF_FIELDS];
        int64_t compare(const big_int& rhs) const;
        big_int divide(const big_int& rhs, big_int*const quo) const;
        bool msb() const;
        template <size_t S_, bool C_> friend std::ostream& operator<<
            (std::ostream& os, const big_int<S_, C_>& num);
        template <size_t S_, bool C_> friend std::istream& operator>>
            (std::istream& is, big_int<S_, C_>& num);
    };

    template <size_t S, bool C> constexpr size_t
        LOGICAL_SIZE_OF_SPECIALIZED(const big_int<S, C>& val);
    template <size_t S, bool O, size_t Z, bool C> big_int<Z, C>
        bytes_to_number_specialized
            (const big_int<Z, C>& val, const get_byte& getByt);
    template <size_t S, bool O, size_t Z, bool C>
        void number_to_bytes_specialized
            (const big_int<Z, C>& num, const put_byte& putByt);

    //@}

    //---- helper declaration ----

    /*!\defgroup mhelper 補助
     * このモジュールには`eccrypto`ライブラリの使用を補助するための機<!--
     * -->能が含まれる。\n
     @{*/

    /*!\brief 値オブジェクトを構築するファクトリーを表す。
     *
     * 値オブジェクトを構築する際に生成源となるオブジェクトを渡す。\n
     * \tparam V 値オブジェクトの型。\n
     * \tparam G 生成源の型。\n
     */
    template <typename V, class G> class factory {
    public:
        /*!\brief インスタンスを構築する。
         * \param gen 生成源。\n
         */
        factory(G*const gen);

        /*!\brief 生成源から値オブジェクトを構築する。
         *
         * 値オブジェクトのコンストラクタには生成源のみを渡す。\n
         * \return 構築した値オブジェクト。\n
         */
        V operator()() const;

        /*!\brief 生成源と可変長引数から値オブジェクトを構築する。
         *
         * 値オブジェクトのコンストラクタには生成源と可変長引数を渡す。\n
         * \tparam AS 可変長引数の型。\n
         * \param args 可変長引数。\n
         */
        template <typename ...AS> V operator()(AS&& ...args) const;
    private:
        G* gen_;
    };

    class initializer {
    public:
        template <typename P> initializer(const P& proc);
    };

    /*!\brief 値の論理的な大きさを取得する。
     * \tparam V 値の型。\n
     * \return 取得した論理的な大きさ(バイト長)。\n
     */
    template <typename V> constexpr size_t LOGICAL_SIZE_OF();

    template <typename V> constexpr size_t
        LOGICAL_SIZE_OF_SPECIALIZED(const V& val);
    template <typename V, size_t S = LOGICAL_SIZE_OF<V>(), bool O = true>
        V bytes_to_number(const get_byte& getByt);

    template <size_t S, bool O, typename V> V
        bytes_to_number_specialized(const V& val, const get_byte& getByt);

    /*!\brief バイト列を整数列に変換する処理関数を作る。
     * \tparam V 整数の型。\n
     * \tparam S 整数の大きさ(バイト長)。既定値は論理的な大きさ。\n
     * \tparam O バイトオーダー。ビッグエンディアンなら真、リトルエン<!--
     * -->ディアンなら偽。既定値は真。\n
     * \return 作った処理関数。\n
     */
    template <typename V, size_t S = LOGICAL_SIZE_OF<V>(), bool O = true>
        byte_to_number<V> bytes_to_numbers();

    /*!\brief 型変換する処理関数を作る。
     * \tparam I 入力するデータ片の型。\n
     * \tparam O 出力するデータ片の型。\n
     * \return 作った処理関数。\n
     */
    template <typename I, typename O> value_to_value<I, O> cast();

    template <typename ...AS> std::string describe(AS&& ...args);
    template <typename ...AS> std::string describe_with
        (const std::string& sep, AS&& ...args);
    template <typename A> void describe_with_to
        (const std::string& sep, std::ostream& os, A&& arg);
    template <typename L, typename ...TS> void describe_with_to
        (const std::string& sep, std::ostream& os, L&& lea, TS&& ...tras);
    template <typename V> auto doubling(const V& val) ->
        decltype(val+val);
    template <typename V> V fit_within_range
        (const V& num, const V& min, const V& max);
    template <typename V> V gcdm
        (const V& a, const V& b, V*const x, V*const y);
    template <typename V> V gcdm(
        const V& px,
        const V& py,
        const V& pr,
        const V& cx,
        const V& cy,
        const V& cr,
        V*const x,
        V*const y
    );

    /*!\brief 値オブジェクトを構築するファクトリーを作る。
     * \tparam V 値オブジェクトの型。\n
     * \tparam G 生成源の型。\n
     * \param gen 生成源。\n
     * \return 作ったファクトリー。\n
     */
    template <typename V, class G> factory<V, G>
        make_factory(G*const gen);

    /*!\brief 値の反復子の範囲から入力関数を作る。
     *
     * 入力関数は`ite`を参照し、インクリメントし、取得した値を返す。\n
     * 反復を終えると`ecc::eof_exception`をスローする。\n
     * \tparam V 入力するデータ片の型。\n
     * \tparam I 反復子の型。\n
     * \param beg 開始の値を指す反復子。\n
     * \param end 終了の値を指す反復子。\n
     * \return 作った入力関数。\n
     */
    template <typename V, typename I> get_value<V>
        make_getter_from_range(const I& beg, const I& end);

    /*!\brief 値の反復子から出力関数を作る。
     *
     * 出力関数は`ite`を参照し、出力された値を代入し、インクリメントす<!--
     * -->る。\n
     * \tparam V 出力するデータ片の型。\n
     * \tparam O 反復子の型。\n
     * \param ite 反復子。\n
     * \return 作った出力関数。\n
     */
    template <typename V, typename O> put_value<V>
        make_putter_from_iterator(const O& ite);

    /*!\brief 合計から出力関数を作る。
     *
     * 出力関数は出力された値を`sum`に加算する。\n
     * \tparam V 出力するデータ片の型。\n
     * \tparam S 合計の型。\n
     * \param sum 合計。\n
     * \return 作った出力関数。\n
     * \warning `sum`はoperator+=演算子を実装していなければならない。\n
     */
    template <typename V, typename S> put_value<V>
        make_putter_from_sum(S*const sum);

    template <typename V> void non_adjacent_form(
        const V& num,
        const std::function<void(const int& sigBit)>& putSigBit
    );

    template <typename V, size_t S = LOGICAL_SIZE_OF<V>(), bool O = true>
        void number_to_bits(const V& num, const put_bit& putBit);
    template <typename V, size_t S = LOGICAL_SIZE_OF<V>(), bool O = true>
        void number_to_bytes(const V& num, const put_byte& putByt);

    template <size_t S, bool O, typename V>
        void number_to_bytes_specialized
            (const V& num, const put_byte& putByt);

    /*!\brief 整数列をバイト列に変換する処理関数を作る。
     * \tparam V 整数の型。\n
     * \tparam S 整数の大きさ。既定値は論理的な大きさ。\n
     * \tparam O バイトオーダー。ビッグエンディアンなら真、リトルエン<!--
     * -->ディアンなら偽。既定値は真。\n
     * \return 作った処理関数。\n
     */
    template <typename V, size_t S = LOGICAL_SIZE_OF<V>(), bool O = true>
        number_to_byte<V> numbers_to_bytes();

    template <typename V> auto square(const V& val) -> decltype(val*val);
    template <typename V> value_to_value<V, V> through();

    int base_to_radix(const std::ios::fmtflags& bas);
    int bits_to_byte(const get_bit& getBit);

    /*!\brief ビット列をバイト列に変換する処理関数を作る。
     * \return 作った処理関数。\n
     */
    bit_to_byte bits_to_bytes();

    void byte_to_bits(const int& byt, const put_bit& putBit);

    /*!\brief バイト列をビット列に変換する処理関数を作る。
     * \return 作った処理関数。\n
     */
    byte_to_bit bytes_to_bits();

    int letter_to_digit(const int& let);

    /*!\brief 入力ストリームからバイト列の入力関数を作る。
     *
     * 入力関数はEOFに達すると`ecc::eof_exception`をスローする。\n
     * \param is 入力ストリーム。\n
     * \return 作った入力関数。\n
     */
    get_byte make_getter_from_istream(std::istream& is);

    /*!\brief 出力ストリームからバイト列の出力関数を作る。
     *
     * \param io 出力ストリーム。\n
     * \return 作った出力関数。\n
     */
    put_byte make_putter_from_ostream(std::ostream& os);

    //@}

    //---- ecurve definition ----

    template <typename V> PrimeField<V>::PrimeField
        (const V& ord) : ord_(ord) {}

    template <typename V> V PrimeField<V>::order() {
        return ord_;
    }

    template <typename V> element<V>::element
        (PrimeField<V>*const priFie, const V& val) :
            pri_fie_(priFie), val_(val)
    {
        val_ %= pri_fie_->order();
        if (val_ < V(0)) val_ += pri_fie_->order();
    }

    template <typename V> element<V>::element
        (PrimeField<V>*const priFie, const element& ele) :
            element(priFie, ele.val_) {}

    template <typename V> bool element<V>::operator!=(const element& rhs)
        const
    {
        return val_ != rhs.val_;
    }

    template <typename V> element<V> element<V>::operator*
        (const element& rhs) const
    {
        return element(pri_fie_, val_ * rhs.val_);
    }

    template <typename V> rational_point<V> element<V>::operator*
        (const rational_point<V>& rhs) const
    {
        return rhs * *this;
    }

    template <typename V> element<V>& element<V>::operator*=
        (const element& rhs)
    {
        return *this = *this * rhs;
    }

    template <typename V> element<V> element<V>::operator+
        (const element& rhs) const
    {
        return element(pri_fie_, val_ + rhs.val_);
    }

    template <typename V> element<V>& element<V>::operator+=
        (const element& rhs)
    {
        return *this = *this + rhs;
    }

    template <typename V> element<V> element<V>::operator-() const {
        return element(pri_fie_, -val_);
    }

    template <typename V> element<V> element<V>::operator-
        (const element& rhs) const
    {
        return element(pri_fie_, val_ - rhs.val_);
    }

    template <typename V> element<V>& element<V>::operator-=
        (const element& rhs)
    {
        return *this = *this - rhs;
    }

    template <typename V> element<V> element<V>::operator/
        (const element& rhs) const
    {
        V x, y;
        gcdm(pri_fie_->order(), rhs.val_, &x, &y);
        return element(pri_fie_, val_ * y);
    }

    template <typename V> element<V>& element<V>::operator/=
        (const element& rhs)
    {
        return *this = *this / rhs;
    }

    template <typename V> bool element<V>::operator==
        (const element& rhs) const
    {
        return val_ == rhs.val_;
    }

    template <typename V> element<V>::operator bool() const {
        return bool(val_);
    }

    template <typename V> V element<V>::value() const {
        return val_;
    }

    template <typename V_> std::ostream& operator<<
        (std::ostream& os, const element<V_>& ele)
    {
        return os << ele.val_;
    }

    template <typename V> EllipticCurve<V>::EllipticCurve(
        PrimeField<V>*const priFie,
        const element<V>& coeA,
        const element<V>& coeB
    ) : coeA_(coeA), coeB_(coeB), priFie_(priFie) {}

    template <typename V> element<V> EllipticCurve<V>::coefficientA() {
        return coeA_;
    }

    template <typename V> element<V> EllipticCurve<V>::coefficientB() {
        return coeB_;
    }

    template <typename V> PrimeField<V>* EllipticCurve<V>::primeField() {
        return priFie_;
    }

    template <typename V> rational_point<V>::rational_point() :
        inf_(true) {}

    template <typename V> rational_point<V>::rational_point(
        EllipticCurve<V>*const ec,
        const element<V>& x,
        const element<V>& y
    ) : rational_point(ec, x, y, element<V>(ec->primeField(), V(1))) {}

    template <typename V> rational_point<V>::rational_point(
        EllipticCurve<V>*const ec,
        const element<V>& x,
        const element<V>& y,
        const element<V>& z
    ) : rational_point
        (ec, x, y, z, ec->coefficientA() * square(square(z))) {}

    template <typename V> bool rational_point<V>::is_infinity() const {
        return inf_;
    }

    template <typename V> bool rational_point<V>::operator!=
        (const rational_point& rhs) const
    {
        return !(*this == rhs);
    }

    template <typename V> rational_point<V>
        rational_point<V>::operator*(const element<V>& rhs) const
    {
        std::vector<int> sigBits;
        non_adjacent_form(
            rhs.value(),
            make_putter_from_iterator<int>(std::back_inserter(sigBits))
        );
        rational_point<V> res;
        const auto negPoi = -*this;
        for (const int& sigBit : sigBits) {
            res += res;
            if (sigBit < 0) res += negPoi;
            else if (sigBit > 0) res += *this;
        }
        return res;
    }

    template <typename V> rational_point<V>&
        rational_point<V>::operator*=(const element<V>& rhs)
    {
        return *this = *this * rhs;
    }

    template <typename V> rational_point<V>
        rational_point<V>::operator+(const rational_point& rhs) const
    {
        rational_point res;
        if (inf_) res = rhs;
        else if (rhs.inf_) res = *this;
        else {
            const auto e =
                factory<element<V>, PrimeField<V>>(ec_->primeField());
            if (*this != rhs) {
                const element<V>
                    ZZ = square(z_),
                    Zz = z_ * rhs.z_,
                    zz = square(rhs.z_),
                    u1 = x_ * zz,
                    u2 = rhs.x_ * ZZ,
                    h = u2 - u1,
                    z = Zz * h;
                if (z) {
                    const element<V>
                        ZZZ = ZZ * z_,
                        zzz = zz * rhs.z_,
                        hh = square(h),
                        hhh = hh * h,
                        u1hh = u1 * hh,
                        s1 = y_ * zzz,
                        s2 = rhs.y_ * ZZZ,
                        r = s2 - s1,
                        rr = square(r),
                        x = -hhh - e(V(2)) * u1hh + rr,
                        y = -s1 * hhh + r * (u1hh - x);
                    res = rational_point(ec_, x, y, z);
                }
            } else {
                const element<V> z(e(V(2)) * y_ * z_);
                if (z) {
                    const element<V>
                        XX = square(x_),
                        YY = square(y_),
                        YYYY = square(YY),
                        YYYY8 = e(V(8)) * YYYY,
                        YYYY16 = doubling(YYYY8),
                        s = e(V(4)) * x_ * YY,
                        m = e(V(3)) * XX + w_,
                        mm = square(m),
                        x = -e(V(2)) * s + mm,
                        y = -YYYY8 + m * (s - x),
                        w = YYYY16 * w_;
                    res = rational_point(ec_, x, y, z, w);
                }
            }
        }
        return res;
    }

    template <typename V> rational_point<V>&
        rational_point<V>::operator+=(const rational_point& rhs)
    {
        return *this = *this + rhs;
    }

    template <typename V> rational_point<V>
        rational_point<V>::operator-() const
    {
        rational_point res;
        if (!inf_) res = rational_point(ec_, x_, -y_, z_, w_);
        return res;
    }

    template <typename V> bool rational_point<V>::operator==
        (const rational_point& rhs) const
    {
        bool res;
        if (inf_ || rhs.inf_) res = inf_ && rhs.inf_;
        else if (z_ == rhs.z_) res = x_ == rhs.x_ && y_ == rhs.y_;
        else {
            const auto
                ZZ = square(z_),
                ZZZ = ZZ * z_,
                zz = square(rhs.z_),
                zzz = zz * rhs.z_,
                x = x_ * zz,
                y = y_ * zzz,
                X = rhs.x_ * ZZ,
                Y = rhs.y_ * ZZZ;
            res = x == X && y == Y;
        }
        return res;
    }

    template <typename V> rational_point<V> rational_point<V>::project()
        const
    {
        rational_point<V> res;
        if (!inf_) {
            const auto ZZ = square(z_), ZZZ = ZZ * z_;
            res = rational_point<V>(ec_, x_ / ZZ, y_ / ZZZ);
        }
        return res;
    }

    template <typename V> element<V> rational_point<V>::x() const {
        return x_;
    }

    template <typename V> element<V> rational_point<V>::y() const {
        return y_;
    }

    template <typename V> element<V> rational_point<V>::z() const {
        return z_;
    }

    template <typename V> rational_point<V>::rational_point(
        EllipticCurve<V>*const ec,
        const element<V>& x,
        const element<V>& y,
        const element<V>& z,
        const element<V>& w
    ) : ec_(ec), inf_(false), x_(x), y_(y), z_(z), w_(w) {}

    template <typename V_> std::ostream& operator<<
        (std::ostream& os, const rational_point<V_>& poi)
    {
        std::ostringstream oss;
        if (poi.inf_) oss << "∞";
        else oss << '{' <<
            describe_with(", ", poi.x_, poi.y_, poi.z_) <<
        '}';
        return os << oss.str();
    }

    //---- random definition ----

    template <typename V, class RBG>
        RandomNumberGenerator<V, RBG>::RandomNumberGenerator
            () : rbg_(std::make_shared<RBG>()) {}

    template <typename V, class RBG> V
        RandomNumberGenerator<V, RBG>::getNumber
            (const V& min, const V& max)
    {
        V num(0);
        for (size_t i = 0; i < 8 * LOGICAL_SIZE_OF<V>(); ++i)
            num = (num << 1) | V(rbg_->getBit());
        return fit_within_range(num, min, max);
    }

    //---- streaming definition ----

    template <typename V> Joint<V>::Joint(const size_t& cap) :
        cap_(cap) {}

    template <typename V> void Joint<V>::initialize() {
        eof_ = false;
        std::queue<V>().swap(inpBuf_);
        std::queue<V>().swap(excBuf_);
        std::queue<V>().swap(outBuf_);
    }

    template <typename V> void Joint<V>::close() {
        std::unique_lock<std::mutex> loc(mut_);
        while (!excBuf_.empty()) conVar_.wait(loc);
        excBuf_.swap(inpBuf_);
        eof_ = true;
        conVar_.notify_one();
    }

    template <typename V> std::function<V()> Joint<V>::getter() {
        return [this] () -> V {
            if (outBuf_.empty()) {
                std::unique_lock<std::mutex> loc(mut_);
                while (excBuf_.empty() && !eof_) conVar_.wait(loc);
                if (!excBuf_.empty()) {
                    excBuf_.swap(outBuf_);
                    conVar_.notify_one();
                } else throw eof_exception();
            }
            const auto val = outBuf_.front();
            outBuf_.pop();
            return val;
        };
    }

    template <typename V> std::function<void(const V& val)>
        Joint<V>::putter()
    {
        return [this] (const V& val) {
            if (inpBuf_.size() == cap_) {
                std::unique_lock<std::mutex> loc(mut_);
                while (!excBuf_.empty()) conVar_.wait(loc);
                excBuf_.swap(inpBuf_);
                conVar_.notify_one();
            }
            inpBuf_.push(val);
        };
    }

    template <typename O> void AbstractInputProcess<O>::initialize() {
        outJoi_->initialize();
    }

    template <typename O> Joint<O> *AbstractInputProcess<O>::outputJoint()
    {
        return outJoi_.get();
    }

    template <typename O> AbstractInputProcess<O>::AbstractInputProcess
        (const size_t& cap) : outJoi_(new Joint<O>(cap)) {}

    template <typename I> void AbstractOutputProcess<I>::inputProcess
        (const std::shared_ptr<AbstractInputProcess<I>>& inpPro)
    {
        inpPro_ = inpPro;
    }

    template <typename I> void AbstractOutputProcess<I>::join()
    {
        inpPro_->join();
        Process::join();
    }

    template <typename I> void AbstractOutputProcess<I>::start()
    {
        inpPro_->start();
        Process::start();
    }

    template <typename O> InputProcess<O>::InputProcess
        (const procedure& pro, const size_t& cap) :
            AbstractInputProcess<O>(cap), pro_(pro) {}

    template <typename O> void InputProcess<O>::enter() {
        try {
            pro_(super::outJoi_->putter());
        } catch (const eof_exception& exc) {}
        super::outJoi_->close();
    }

    template <typename I, typename O>
        InputOutputProcess<I, O>::InputOutputProcess
            (const procedure& pro, const size_t& cap) :
                AbstractInputProcess<O>(cap), pro_(pro) {}

    template <typename I, typename O>
        void InputOutputProcess<I, O>::enter()
    {
        try {
            pro_(
                superOutput::inpPro_->outputJoint()->getter(),
                superInput::outJoi_->putter()
            );
        } catch (const eof_exception& exc) {}
        superInput::outJoi_->close();
    }

    template <typename I> OutputProcess<I>::OutputProcess
        (const procedure& pro) : pro_(pro) {}

    template <typename I> void OutputProcess<I>::run() {
        super::start();
        super::join();
    }

    template <typename I> void OutputProcess<I>::enter() {
        try {
            pro_(super::inpPro_->outputJoint()->getter());
        } catch (const eof_exception& eof) {}
    }

    template <class F, class S, class ...PS> auto concatinate
        (const F& firPro, const S& secPro, PS&& ...pros) ->
            decltype(concatinate(secPro, pros...))
    {
        return concatinate(concatinate(firPro, secPro), pros...);
    }

    template <typename FI, typename FO, typename SO> auto concatinate(
        const std::shared_ptr<InputOutputProcess<FI, FO>>& firInpOutPro,
        const std::shared_ptr<InputOutputProcess<FO, SO>>& secInpOutPro
    ) -> decltype(secInpOutPro) {
        secInpOutPro->inputProcess(firInpOutPro);
        return secInpOutPro;
    }

    template <typename FO, typename SO> auto concatinate(
        const std::shared_ptr<InputProcess<FO>>& firInpPro,
        const std::shared_ptr<InputOutputProcess<FO, SO>>& secInpOutPro
    ) -> decltype(secInpOutPro) {
        secInpOutPro->inputProcess(firInpPro);
        return secInpOutPro;
    }

    template <typename FI, typename FO> auto concatinate(
        const std::shared_ptr<InputOutputProcess<FI, FO>>& firInpOutPro,
        const std::shared_ptr<OutputProcess<FO>>& secOutPro
    ) -> decltype(secOutPro) {
        secOutPro->inputProcess(firInpOutPro);
        return secOutPro;
    }

    template <typename FO> auto concatinate(
        const std::shared_ptr<InputProcess<FO>>& firInpPro,
        const std::shared_ptr<OutputProcess<FO>>& secOutPro
    ) -> decltype(secOutPro) {
        secOutPro->inputProcess(firInpPro);
        return secOutPro;
    }

    template <typename O> typename InputProcess<O>::procedure
        input_from(const get_value<O>& getVal)
    {
        return [getVal] (const put_value<O>& putVal) {
            for (;;) putVal(getVal());
        };
    }

    template <typename I> typename OutputProcess<I>::procedure
        output_to(const put_value<I>& putVal)
    {
        return [putVal] (const get_value<I>& getVal) {
            for (;;) putVal(getVal());
        };
    }

    template <typename I, typename O, typename T>
        typename InputOutputProcess<I, O>::procedure
            transform(const T& tra)
    {
        return [tra] (
            const get_value<I>& getVal,
            const put_value<O>& putVal
        ) {
            for (;;) putVal(tra(getVal()));
        };
    }

    //---- binteger definition ----

    template <size_t S, bool C> constexpr size_t
        big_int<S, C>::LOGICAL_SIZE;
    template <size_t S, bool C> constexpr size_t
        big_int<S, C>::SIZE_OF_FIELD;
    template <size_t S, bool C> constexpr size_t
        big_int<S, C>::LENGTH_OF_BITS_ON_FIELD;
    template <size_t S, bool C> constexpr uint64_t
        big_int<S, C>::FIELD_MASK;
    template <size_t S, bool C> constexpr size_t
        big_int<S, C>::NUMBER_OF_FIELDS;

    template <size_t S, bool C> big_int<S, C>::big_int
        (const big_int& ano)
    {
        std::memcpy
            (fies_, ano.fies_, sizeof(uint64_t) * NUMBER_OF_FIELDS);
    }

    template <size_t S, bool C> big_int<S, C>::big_int
        (const int& num) : big_int(int64_t(num)) {}

    template <size_t S, bool C> big_int<S, C>::big_int
        (const unsigned& num) : big_int(uint64_t(num)) {}

    template <size_t S, bool C> big_int<S, C>::big_int
        (const int64_t&num)
    {
        auto n = big_int(uint64_t(num));
        if (num < 0) {
            for (size_t i = 2; i < NUMBER_OF_FIELDS; ++i)
                n.fies_[i] = FIELD_MASK;
        }
        *this = n;
    }

    template <size_t S, bool C> big_int<S, C>::big_int
        (const uint64_t&num)
    {
        fies_[0] = num & FIELD_MASK;
        fies_[1] = num >> LENGTH_OF_BITS_ON_FIELD;
        std::memset
            (fies_ + 2, 0, sizeof(uint64_t) * (NUMBER_OF_FIELDS - 2));
    }

    template <size_t S, bool C> big_int<S, C>::big_int
        (const char*const str, const std::ios::fmtflags& bas) :
            big_int(str, str + std::strlen(str), bas) {}

    template <size_t S, bool C> big_int<S, C>::big_int
        (const std::string& str, const std::ios::fmtflags& bas) :
            big_int(str.begin(), str.end(), bas) {}

    template <size_t S, bool C> template <typename I>
        big_int<S, C>::big_int
            (const I& beg, const I& end, const std::ios::fmtflags& bas)
    {
        if (beg != end) {
            I ite = beg;
            const int lea = *ite;
            if (lea == '+' || lea == '-') ++ite;
            const auto rad = big_int(base_to_radix(bas));
            auto num = big_int(0);
            for (; ite != end; ++ite)
                num = num * rad + big_int(letter_to_digit(*ite));
            if ((bas & std::ios::dec) && lea == '-') num = -num;
            *this = num;
        }
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator&(const big_int& rhs) const
    {
        big_int<S, C> res;
        for (size_t i = 0; i < NUMBER_OF_FIELDS; ++i)
            res.fies_[i] = fies_[i] & rhs.fies_[i];
        return res;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator&=(const big_int& rhs)
    {
        return *this = *this & rhs;
    }

    template <size_t S, bool C> big_int<S, C>::operator bool() const {
        bool res = false;
        for (size_t i = 0; i < NUMBER_OF_FIELDS; ++i) {
            if (fies_[i]) {
                res = true;
                break;
            }
        }
        return res;
    }

    template <size_t S, bool C> big_int<S, C>::operator int() const {
        return int(uint64_t(*this));
    }

    template <size_t S, bool C> big_int<S, C>::operator int64_t() const {
        return int64_t(uint64_t(*this));
    }

    template <size_t S, bool C> big_int<S, C>::operator uint64_t() const {
        return fies_[0] | (fies_[1] << LENGTH_OF_BITS_ON_FIELD);
    }

    template <size_t S, bool C> big_int<S, C>::operator unsigned() const {
        return unsigned(uint64_t(*this));
    }

    template <size_t S, bool C> bool
        big_int<S, C>::operator!=(const big_int& rhs) const
    {
        return compare(rhs) != 0;
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator%(const big_int& rhs) const
    {
        return divide(rhs, nullptr);
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator%=(const big_int& rhs)
    {
        return *this = *this % rhs;
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator*(const big_int& rhs) const
    {
        const bool lmsb = msb(), rmsb = rhs.msb();
        big_int alhs, arhs;
        if (lmsb) alhs = -*this;
        else alhs = *this;
        if (rmsb) arhs = -rhs;
        else arhs = rhs;
        big_int pro(0);
        for (size_t i = 0; i < NUMBER_OF_FIELDS; ++i) {
            if (alhs.fies_[i]) {
                for (size_t j = 0; j < NUMBER_OF_FIELDS - i; ++j) {
                    if (arhs.fies_[j]) {
                        const size_t k = i + j;
                        const uint64_t
                            par = alhs.fies_[i] * arhs.fies_[j],
                            low = par & FIELD_MASK,
                            hig = par >> LENGTH_OF_BITS_ON_FIELD;
                        pro.fies_[k] += low;
                        if (k < NUMBER_OF_FIELDS - 1)
                            pro.fies_[k + 1] += hig;
                    }
                }
            }
        }
        for (size_t i = 0; i < NUMBER_OF_FIELDS - 1; ++i) {
            const uint64_t
                low = pro.fies_[i] & FIELD_MASK,
                hig = pro.fies_[i] >> LENGTH_OF_BITS_ON_FIELD;
            pro.fies_[i] = low;
            if (i < NUMBER_OF_FIELDS - 1) pro.fies_[i + 1] += hig;
        }
        if (lmsb != rmsb) pro = -pro;
        return pro;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator*=(const big_int& rhs)
    {
        return *this = *this * rhs;
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator+(const big_int& rhs) const
    {
        big_int sum;
        uint64_t car = 0;
        for (size_t i = 0; i < NUMBER_OF_FIELDS; ++i) {
            sum.fies_[i] = fies_[i] + rhs.fies_[i] + car;
            car = sum.fies_[i] >> LENGTH_OF_BITS_ON_FIELD;
            sum.fies_[i] &= FIELD_MASK;
        }
        return sum;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator++()
    {
        return *this += big_int(1);
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator++(int)
    {
        const big_int las(*this);
        *this += big_int(1);
        return las;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator+=(const big_int& rhs)
    {
        return *this = *this + rhs;
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator-() const
    {
        return ~*this + big_int(1);
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator-(const big_int& rhs) const
    {
        big_int dif;
        uint64_t bor = 0;
        for (size_t i = 0; i < NUMBER_OF_FIELDS; ++i) {
            dif.fies_[i] = fies_[i] - rhs.fies_[i] - bor;
            bor = (dif.fies_[i] >> LENGTH_OF_BITS_ON_FIELD) & 1;
            dif.fies_[i] &= FIELD_MASK;
        }
        return dif;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator-=(const big_int& rhs)
    {
        return *this = *this - rhs;
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator/(const big_int& rhs) const
    {
        big_int quo;
        divide(rhs, &quo);
        return quo;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator/=(const big_int& rhs)
    {
        return *this = *this / rhs;
    }

    template <size_t S, bool C> bool
        big_int<S, C>::operator<(const big_int& rhs) const
    {
        return compare(rhs) < 0;
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator<<(const int& len) const
    {
        big_int res;
        if (!len) res = *this;
        if (len < 0) res = *this >> size_t(-len);
        else res = *this << size_t(len);
        return res;
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator<<(const size_t& len) const
    {
        big_int res;
        const size_t
            dis = std::min
                (len / LENGTH_OF_BITS_ON_FIELD, NUMBER_OF_FIELDS),
            rem = len - dis * LENGTH_OF_BITS_ON_FIELD;
        if (rem) {
            for (size_t i = 0; i < NUMBER_OF_FIELDS - dis; ++i) {
                const size_t j = NUMBER_OF_FIELDS - i - 1, k = j - dis;
                uint64_t low;
                if (k > 0)
                    low = fies_[k - 1] >> (LENGTH_OF_BITS_ON_FIELD - rem);
                else low = 0;
                const uint64_t hig = (fies_[k] << rem) & FIELD_MASK;
                res.fies_[j] = low | hig;
            }
        } else
            std::memcpy(
                res.fies_ + dis,
                fies_,
                sizeof(uint64_t) * (NUMBER_OF_FIELDS - dis)
            );
        std::memset(res.fies_, 0, sizeof(uint64_t) * dis);
        return res;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator<<=(const int& len)
    {
        return *this = *this << len;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator<<=(const size_t& len)
    {
        return *this = *this << len;
    }

    template <size_t S, bool C> bool
        big_int<S, C>::operator<=(const big_int& rhs) const
    {
        return compare(rhs) <= 0;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator=(const big_int& rhs)
    {
        if (this != &rhs) std::memcpy
            (fies_, rhs.fies_, sizeof(uint64_t) * NUMBER_OF_FIELDS);
        return *this;
    }

    template <size_t S, bool C> bool
        big_int<S, C>::operator==(const big_int& rhs) const
    {
        return compare(rhs) == 0;
    }

    template <size_t S, bool C> bool
        big_int<S, C>::operator>(const big_int& rhs) const
    {
        return compare(rhs) > 0;
    }

    template <size_t S, bool C> bool
        big_int<S, C>::operator>=(const big_int& rhs) const
    {
        return compare(rhs) >= 0;
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator>>(const int& len) const
    {
        big_int res;
        if (!len) res = *this;
        else if (len < 0) res = *this << size_t(-len);
        else res = *this >> size_t(len);
        return res;
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator>>(const size_t& len) const
    {
        big_int res;
        const bool msb_ = msb();
        const size_t
            dis = std::min
                (len / LENGTH_OF_BITS_ON_FIELD, NUMBER_OF_FIELDS),
            rem = len - dis * LENGTH_OF_BITS_ON_FIELD;
        if (rem) {
            for (size_t i = 0; i < NUMBER_OF_FIELDS - dis; ++i) {
                const size_t j = i + dis;
                const uint64_t low = fies_[j] >> rem;
                uint64_t hig;
                if (j < NUMBER_OF_FIELDS - 1)
                    hig = (fies_[j + 1] <<
                        (LENGTH_OF_BITS_ON_FIELD - rem)
                    ) & FIELD_MASK;
                else if (msb_)
                    hig = (FIELD_MASK <<
                        (LENGTH_OF_BITS_ON_FIELD - rem)
                    ) & FIELD_MASK;
                else hig = 0;
                res.fies_[i] = low | hig;
            }
        } else std::memcpy(
            res.fies_,
            fies_ + dis,
            sizeof(uint64_t) * (NUMBER_OF_FIELDS - dis)
        );
        if (msb_) {
            for (
                size_t i = NUMBER_OF_FIELDS - dis;
                i < NUMBER_OF_FIELDS;
                ++i
            ) res.fies_[i] = FIELD_MASK;
        }
        else std::memset(
            res.fies_ + NUMBER_OF_FIELDS - dis,
            0,
            sizeof(uint64_t) * dis
        );
        return res;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator>>=(const int& len)
    {
        return *this = *this >> len;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator>>=(const size_t& len)
    {
        return *this = *this >> len;
    }

    template <size_t S, bool C> const uint64_t&
        big_int<S, C>::operator[](const size_t& index) const
    {
        return fies_[index];
    }

    template <size_t S, bool C> uint64_t&
        big_int<S, C>::operator[](const size_t& index)
    {
        return fies_[index];
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator^(const big_int& rhs) const
    {
        big_int<S, C> res;
        for (size_t i = 0; i < NUMBER_OF_FIELDS; ++i)
            res.fies_[i] = fies_[i] ^ rhs.fies_[i];
        return res;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator^=(const big_int& rhs)
    {
        return *this = *this ^ rhs;
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator|(const big_int& rhs) const
    {
        big_int<S, C> res;
        for (size_t i = 0; i < NUMBER_OF_FIELDS; ++i)
            res.fies_[i] = fies_[i] | rhs.fies_[i];
        return res;
    }

    template <size_t S, bool C> big_int<S, C>&
        big_int<S, C>::operator|=(const big_int& rhs)
    {
        return *this = *this | rhs;
    }

    template <size_t S, bool C> big_int<S, C>
        big_int<S, C>::operator~() const
    {
        big_int<S, C> res;
        for (size_t i = 0; i < NUMBER_OF_FIELDS; ++i)
            res.fies_[i] = fies_[i] ^ FIELD_MASK;
        return res;
    }

    template <size_t S, bool C> int64_t
        big_int<S, C>::compare(const big_int& rhs) const
    {
        int64_t res = rhs.msb() - msb();
        if (!res) {
            for (size_t i = 0; i < NUMBER_OF_FIELDS; ++i) {
                const size_t j = NUMBER_OF_FIELDS - i - 1;
                res = int64_t(fies_[j]) - int64_t(rhs.fies_[j]);
                if (res) break;
            }
        }
        return res;
    }

    template <size_t S, bool C> big_int<S, C>big_int<S, C>::divide
        (const big_int& rhs, big_int*const quo) const
    {
        const bool lmsb = msb(), rmsb = rhs.msb();
        big_int alhs, arhs;
        if (lmsb) alhs = -*this;
        else alhs = *this;
        if (rmsb) arhs = -rhs;
        else arhs = rhs;
        if (quo) *quo = big_int(0);
        auto rem = big_int(0);
        int64_t resCmp = alhs.compare(arhs);
        if (resCmp < 0) rem = *this;
        else if (resCmp == 0) {
            if (quo) *quo = big_int(1);
        } else {
            size_t rmsvi;
            for (size_t i = 0; i < NUMBER_OF_FIELDS; ++i) {
                rmsvi = NUMBER_OF_FIELDS - i - 1;
                if (arhs.fies_[rmsvi]) break;
            }
            std::vector<big_int> stes({arhs});
            for (size_t i = 0; i < NUMBER_OF_FIELDS; ++i) {
                const size_t j = NUMBER_OF_FIELDS - i - 1;
                rem <<= LENGTH_OF_BITS_ON_FIELD;
                rem.fies_[0] = alhs.fies_[j];
                resCmp = rem.compare(arhs);
                if (resCmp <= 0) {
                    if (quo) *quo <<= LENGTH_OF_BITS_ON_FIELD;
                    if (!resCmp) {
                        if (quo) quo->fies_[0] = 1;
                        rem = big_int(0);
                    }
                } else {
                    uint64_t lmsv = rem.fies_[rmsvi];
                    if (rmsvi < NUMBER_OF_FIELDS - 1)
                        lmsv |= rem.fies_[rmsvi + 1] <<
                            LENGTH_OF_BITS_ON_FIELD;
                    const uint64_t& rmsv = arhs.fies_[rmsvi];
                    uint64_t qlsv = std::min(lmsv / rmsv, FIELD_MASK);
                    const auto pro = arhs * big_int(qlsv);
                    if (rem < pro) {
                        size_t len = stes.size() - 1;
                        bool asc = true;
                        for (;;) {
                            const auto nexRem = rem + stes.at(len);
                            resCmp = nexRem.compare(pro);
                            if (resCmp <= 0 || !len) {
                                rem = nexRem;
                                qlsv -= 1 << len;
                                if (!resCmp || (resCmp > 0 && !len))
                                    break;
                                if (asc) {
                                    ++len;
                                    stes.push_back(doubling(stes.back()));
                                } else if (len) --len;
                            } else {
                                --len;
                                asc = false;
                            }
                        }
                    }
                    if (quo) {
                        *quo <<= LENGTH_OF_BITS_ON_FIELD;
                        quo->fies_[0] = qlsv;
                    }
                    rem -= pro;
                }
            }
            if (lmsb) rem = -rem;
        }
        if (quo && lmsb != rmsb) *quo = -*quo;
        return rem;
    }

    template <size_t S, bool C> bool big_int<S, C>::msb() const {
        return fies_[NUMBER_OF_FIELDS - 1] >>
            (LENGTH_OF_BITS_ON_FIELD - 1);
    }

    template <size_t S_, bool C_> std::ostream&
        operator<<(std::ostream& os, const big_int<S_, C_>& num)
    {
        std::ostringstream oss;
        oss.flags(os.flags() &
            (std::ios::basefield | std::ios::uppercase));
        if (oss.flags() & std::ios::hex) {
            auto n = big_int<S_, C_>(num);
            static constexpr size_t LEN =
                big_int<S_, C_>::NUMBER_OF_FIELDS *
                    big_int<S_, C_>::LENGTH_OF_BITS_ON_FIELD / 4;
            for (size_t i = 0; i < LEN; ++i) {
                oss << (n[0] & 0xf);
                n >>= 4;
                if (!n) break;
            }
        } else if (oss.flags() & std::ios::oct) {
            auto n = big_int<S_, C_>(num);
            static constexpr size_t LEN = (
                big_int<S_, C_>::NUMBER_OF_FIELDS *
                    big_int<S_, C_>::LENGTH_OF_BITS_ON_FIELD + (3 - 1)
            ) / 3;
            for (size_t i = 0; i < LEN; ++i) {
                oss << (n[0] & 0x7);
                n >>= 3;
                if (!n) break;
            }
        } else {
            big_int<S_, C_> n, nn, rem, rad(10);
            if (num < big_int<S_, C_>(0)) n = -num;
            else n = num;
            do {
                nn = n / rad;
                rem = n - rad * nn;
                oss << rem[0];
                n = nn;
            } while (n);
        }
        if (os.flags() & std::ios::internal) {
            int wid = os.width() - oss.tellp();
            if (os.flags() & std::ios::showbase) {
                if (os.flags() & std::ios::hex) --wid;
                if (os.flags() & (std::ios::hex | std::ios::oct)) --wid;
            }
            if (os.flags() & std::ios::dec &&
                ((os.flags() & std::ios::showpos) || num < big_int<S_, C_>(0))
            ) --wid;
            if (wid > 0) {
                for (size_t i = 0; i < wid; ++i) oss << os.fill();
            }
        }
        if (os.flags() & std::ios::showbase) {
            if (os.flags() & std::ios::hex) {
                if (os.flags() & std::ios::uppercase) oss << 'X';
                else oss << 'x';
            }
            if (os.flags() & (std::ios::hex | std::ios::oct)) oss << '0';
        }
        if (os.flags() & std::ios::dec) {
            if (num < big_int<S_, C_>(0)) oss << '-';
            else if (os.flags() & std::ios::showpos) oss << '+';
        }
        std::string str = oss.str();
        std::reverse(str.begin(), str.end());
        return os << str;
    }

    template <size_t S_, bool C_> std::istream&
        operator>>(std::istream& is, big_int<S_, C_>& num)
    {
        std::string str;
        is >> str;
        num = big_int<S_, C_>(str, is.flags() & std::ios::basefield);
        return is;
    }

    template <size_t S, bool C> constexpr size_t
        LOGICAL_SIZE_OF_SPECIALIZED(const big_int<S, C>& val)
    {
        return big_int<S, C>::LOGICAL_SIZE;
    }

    template <size_t S, bool O, size_t Z, bool C> big_int<Z, C>
        bytes_to_number_specialized
            (const big_int<Z, C>& val, const get_byte& getByt)
    {
        static constexpr size_t
            QUO = S / big_int<Z>::SIZE_OF_FIELD,
            REM = S % big_int<Z>::SIZE_OF_FIELD;
        auto num = big_int<Z>(0);
        if (O) {
            num[QUO] = bytes_to_number<uint64_t, REM, O>(getByt);
            for (size_t i = 0; i < QUO; ++i)
                num[QUO - i - 1] = bytes_to_number
                    <uint64_t, big_int<Z>::SIZE_OF_FIELD, O>(getByt);
        } else {
            for (size_t i = 0; i < QUO; ++i)
                num[i] = bytes_to_number
                    <uint64_t, big_int<Z>::SIZE_OF_FIELD, O>(getByt);
            num[QUO] = bytes_to_number<uint64_t, REM, O>(getByt);
        }
        return num;
    }

    template <size_t S, bool O, size_t Z, bool C>
        void number_to_bytes_specialized
            (const big_int<Z, C>& num, const put_byte& putByt)
    {
        static constexpr size_t
            QUO = S / big_int<Z>::SIZE_OF_FIELD,
            REM = S % big_int<Z>::SIZE_OF_FIELD;
        if (O) {
            number_to_bytes<uint64_t, REM, O>(num[QUO], putByt);
            for (size_t i = 0; i < QUO; ++i)
                number_to_bytes<uint64_t, big_int<Z>::SIZE_OF_FIELD, O>
                    (num[QUO - i - 1], putByt);
        } else {
            for (size_t i = 0; i < QUO; ++i)
                number_to_bytes<uint64_t, big_int<Z>::SIZE_OF_FIELD, O>
                    (num[i], putByt);
            number_to_bytes<uint64_t, REM, O>(num[QUO], putByt);
        }
    }

    //---- helper definition ----

    template <typename V, class G> factory<V, G>::factory
        (G*const gen) : gen_(gen) {}

    template <typename V, class G> V factory<V, G>::operator()() const {
        return V(gen_);
    }

    template <typename V, class G> template <typename ...AS>
        V factory<V, G>::operator()(AS&& ...args) const
    {
        return V(gen_, args...);
    }

    template <typename P> initializer::initializer(const P& proc) {
        proc();
    }

    template <typename V> constexpr size_t LOGICAL_SIZE_OF() {
        return LOGICAL_SIZE_OF_SPECIALIZED(V());
    }

    template <typename V> constexpr size_t
        LOGICAL_SIZE_OF_SPECIALIZED(const V& val)
    {
        return sizeof(V);
    }

    template <typename V, size_t S, bool O> V
        bytes_to_number(const get_byte& getByt)
    {
        return bytes_to_number_specialized<S, O>(V(), getByt);
    }

    template <size_t S, bool O, typename V> V
        bytes_to_number_specialized(const V& val, const get_byte& getByt)
    {
        auto num = V(0);
        for (size_t i = 0; i < S; ++i) {
            const auto byt = V(getByt());
            if (O) num = (num << 8) | byt;
            else if (i < LOGICAL_SIZE_OF<V>()) num |= byt << (8 * i);
        }
        return num;
    }

    template <typename V, size_t S, bool O>
        byte_to_number<V> bytes_to_numbers()
    {
        return [] (const get_byte& getByt, const put_number<V>& putNum) {
            for (;;) putNum(bytes_to_number<V, S, O>(getByt));
        };
    }

    template <typename I, typename O> value_to_value<I, O> cast() {
        return [] (
            const get_value<I>& getVal,
            const put_value<O>& putVal
        ) {
            for (;;) putVal(O(getVal()));
        };
    }

    template <typename ...AS> std::string describe(AS&& ...args) {
        return describe_with("", args...);
    }

    template <typename ...AS> std::string describe_with
        (const std::string& sep, AS&& ...args)
    {
        std::ostringstream oss;
        describe_with_to(sep, oss, args...);
        return oss.str();
    }

    template <typename A> void describe_with_to
        (const std::string& sep, std::ostream& os, A&& arg)
    {
        os << arg;
    }

    template <typename L, typename ...TS> void describe_with_to
        (const std::string& sep, std::ostream& os, L&& lea, TS&& ...tras)
    {
        os << lea;
        if (!sep.empty()) os << sep;
        describe_with_to(sep, os, tras...);
    }

    template <typename V> auto doubling(const V& val) -> decltype(val+val)
    {
        return val + val;
    }

    template <typename V> V fit_within_range
        (const V& num, const V& min, const V& max)
    {
        V res = num;
        if (res < min || res > max) {
            const V ran = max - min + V(1);
            res %= ran;
            if (res < V(0)) res += max;
            else res += min;
        }
        return res;
    }

    template <typename V> V gcdm
        (const V& a, const V& b, V*const x, V*const y)
    {
        return gcdm<V>(V(1), V(0), a, V(0), V(1), b, x, y);
    }

    template <typename V> V gcdm(
        const V& px,
        const V& py,
        const V& pr,
        const V& cx,
        const V& cy,
        const V& cr,
        V*const x,
        V*const y
    ) {
        V c, cq = pr / cr, nr = pr - cq * cr;
        if (nr) {
            const V nx = px - cq * cx, ny = py - cq * cy;
            c = gcdm(cx, cy, cr, nx, ny, nr, x, y);
        } else {
            *x = cx;
            *y = cy;
            c = cr;
        }
        return c;
    }

    template <typename V, class G> factory<V, G> make_factory(G*const gen)
    {
        return factory<V, G>(gen);
    }

    template <typename V, typename I> get_value<V>
        make_getter_from_range(const I& beg, const I& end)
    {
        I ite = beg;
        return [ite, end] () mutable -> V {
            if (ite == end) throw eof_exception();
            return *ite++;
        };
    }

    template <typename V, typename O> put_value<V>
        make_putter_from_iterator(const O& ite)
    {
        O mutIte = ite;
        return [mutIte] (const V& val) mutable {
            *mutIte = val;
            ++mutIte;
        };
    }

    template <typename V, typename S> put_value<V>
        make_putter_from_sum(S*const sum)
    {
        return [sum] (const V& val) {
            *sum += val;
        };
    }

    template <typename V> void non_adjacent_form(
        const V& num,
        const std::function<void(const int& sigBit)>& putSigBit
    ) {
        std::vector<bool> bits, bits3;
        number_to_bits<V, LOGICAL_SIZE_OF<V>() + 1>(
            num,
            make_putter_from_iterator<bool>(std::back_inserter(bits))
        );
        number_to_bits<V, LOGICAL_SIZE_OF<V>() + 1>(
            V(3) * num,
            make_putter_from_iterator<bool>(std::back_inserter(bits3))
        );
        bool rea = false;
        for (size_t i = 0; i < bits.size() - 1; ++i) {
            const int sigBit = bits3.at(i) - bits.at(i);
            rea = rea || sigBit;
            if (rea) putSigBit(sigBit);
        }
    }

    template <typename V, size_t S, bool O>
        void number_to_bits(const V& num, const put_bit& putBit)
    {
        number_to_bytes<V, S, O>(num, [&putBit] (const int& byt) {
            byte_to_bits(byt, putBit);
        });
    }

    template <typename V, size_t S, bool O>
        void number_to_bytes(const V& num, const put_byte& putByt)
    {
        number_to_bytes_specialized<S, O>(num, putByt);
    }

    template <size_t S, bool O, typename V>
        void number_to_bytes_specialized
            (const V& num, const put_byte& putByt)
    {
        for (size_t i = 0; i < S; ++i) {
            if (O) {
                const size_t j = S - i - 1;
                if (j >= LOGICAL_SIZE_OF<V>()) putByt(0);
                else putByt(int(num >> (8 * (S - i - 1))) & 0xff);
            } else {
                if (i >= LOGICAL_SIZE_OF<V>()) putByt(0);
                else putByt(int(num >> (8 * i)) & 0xff);
            }
        }
    }

    template <typename V, size_t S, bool O>
        number_to_byte<V> numbers_to_bytes()
    {
        return [] (const get_number<V>& getNum, const put_byte& putByt) {
            for (;;) number_to_bytes<V, S, O>(getNum(), putByt);
        };
    }

    template <typename V> auto square(const V& val) -> decltype(val*val) {
        return val * val;
    }

    template <typename V> value_to_value<V, V> through() {
        return [] (
            const get_value<V>& getVal,
            const put_value<V>& putVal
        ) {
            for (;;) putVal(getVal());
        };
    }
}

#endif
