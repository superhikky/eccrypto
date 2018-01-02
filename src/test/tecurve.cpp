#include "ecctest.h"

namespace ecc {
    const auto F = make_shared<PrimeField<int>>(7);
    const auto fe = make_factory<element<int>>(F.get());
    const auto E = make_shared<EllipticCurve<int>>(F.get(), fe(2), fe(6));
    const auto rp = make_factory<rational_point<int>>(E.get());
    const auto B = rp(fe(1), fe(3));
    const auto G = make_shared<PrimeField<int>>(11);
    const auto ge = make_factory<element<int>>(G.get());

    TEST_GROUP(ecurve) {};

    TEST(ecurve, PrimeField) {
        {
            auto P = make_shared<PrimeField<int>>(7);
            CHECK_EQUAL(7, P->order());
        }
    }

    TEST(ecurve, element_new) {
        {
            element<int> e;
        }
        {
            auto a = fe(3);
            element<int> b;
            b = a;
            CHECK_EQUAL(3, b.value());
        }
        {
            auto a = fe(10);
            CHECK_EQUAL(3, a.value());
        }
        {
            auto a = ge(10);
            auto b = fe(a);
            CHECK_EQUAL(3, b.value());
        }
    }

    TEST(ecurve, element_equal) {
        {
            auto a = fe(3);
            auto b = fe(6);
            CHECK(a == a);
            CHECK_FALSE(a == b);
            CHECK_FALSE(b == a);
            CHECK_FALSE(a != a);
            CHECK(a != b);
            CHECK(b != a);
        }
    }

    TEST(ecurve, element_multiply) {
        {
            auto a = fe(3);
            auto b = fe(6);
            auto c = a * b;
            CHECK_EQUAL(4, c.value());
        }
        {
            auto a = fe(3);
            auto b = fe(6);
            a *= b;
            CHECK_EQUAL(4, a.value());
        }
    }

    TEST(ecurve, element_scalar_multiply) {
        {
            auto P = (ge(2) * B).project();
            CHECK_EQUAL(2, P.x().value());
            CHECK_EQUAL(2, P.y().value());
        }
    }

    TEST(ecurve, element_add) {
        {
            auto a = fe(3);
            auto b = fe(6);
            auto c = a + b;
            CHECK_EQUAL(2, c.value());
        }
        {
            auto a = fe(3);
            auto b = fe(6);
            a += b;
            CHECK_EQUAL(2, a.value());
        }
    }

    TEST(ecurve, element_negate) {
        {
            auto a = fe(3);
            auto b = -a;
            CHECK_EQUAL(4, b.value());
        }
    }

    TEST(ecurve, element_subtract) {
        {
            auto a = fe(3);
            auto b = fe(6);
            auto c = a - b;
            CHECK_EQUAL(4, c.value());
        }
        {
            auto a = fe(3);
            auto b = fe(6);
            a -= b;
            CHECK_EQUAL(4, a.value());
        }
    }

    TEST(ecurve, element_assign) {
        {
            auto a = fe(3);
            auto b = fe(6);
            a = b;
            CHECK_EQUAL(6, a.value());
        }
    }

    TEST(ecurve, element_to_bool) {
        {
            auto a = fe(0);
            CHECK_FALSE(bool(a));
        }
        {
            auto a = fe(1);
            CHECK(bool(a));
        }
    }

    TEST(ecurve, EllipticCurve) {
        {
            auto P = make_shared<PrimeField<int>>(7);
            auto C = make_shared<EllipticCurve<int>>(P.get(), fe(2), fe(6));
            CHECK_EQUAL(7, C->primeField()->order());
            CHECK_EQUAL(2, C->coefficientA().value());
            CHECK_EQUAL(6, C->coefficientB().value());
        }
    }

    TEST(ecurve, rational_point_new) {
        {
            rational_point<int> P;
            CHECK(P.is_infinity());
        }
        {
            auto P = rp(fe(1), fe(3));
            CHECK_EQUAL(1, P.x().value());
            CHECK_EQUAL(3, P.y().value());
            CHECK_EQUAL(1, P.z().value());
        }
        {
            auto P = rp(fe(1), fe(3), fe(2));
            CHECK_EQUAL(1, P.x().value());
            CHECK_EQUAL(3, P.y().value());
            CHECK_EQUAL(2, P.z().value());
        }
    }

    TEST(ecurve, rational_point_equal) {
        {
            rational_point<int> P;
            rational_point<int> Q;
            CHECK(P == P);
            CHECK(P == Q);
            CHECK(Q == P);
            CHECK_FALSE(P != P);
            CHECK_FALSE(P != Q);
            CHECK_FALSE(Q != P);
        }
        {
            rational_point<int> P;
            auto Q = rp(fe(1), fe(3));;
            CHECK(P == P);
            CHECK_FALSE(P == Q);
            CHECK_FALSE(Q == P);
            CHECK_FALSE(P != P);
            CHECK(P != Q);
            CHECK(Q != P);
        }
        {
            auto P = rp(fe(1), fe(3));;
            auto Q = rp(fe(1), fe(3));;
            CHECK(P == P);
            CHECK(P == Q);
            CHECK(Q == P);
            CHECK_FALSE(P != P);
            CHECK_FALSE(P != Q);
            CHECK_FALSE(Q != P);
        }
        {
            auto P = rp(fe(1), fe(3));;
            auto Q = rp(fe(2), fe(2));;
            CHECK(P == P);
            CHECK_FALSE(P == Q);
            CHECK_FALSE(Q == P);
            CHECK_FALSE(P != P);
            CHECK(P != Q);
            CHECK(Q != P);
        }
        {
            auto P = rp(fe(6), fe(6), fe(5));;
            auto Q = rp(fe(5), fe(1), fe(1));;
            CHECK(P == P);
            CHECK(P == Q);
            CHECK(Q == P);
            CHECK_FALSE(P != P);
            CHECK_FALSE(P != Q);
            CHECK_FALSE(Q != P);
        }
    }

    TEST(ecurve, rational_point_scalar_multiply) {
        {
            auto P = (ge(2) * B).project();
            CHECK_EQUAL(2, P.x().value());
            CHECK_EQUAL(2, P.y().value());
            auto Q = (ge(3) * B).project();
            CHECK_EQUAL(5, Q.x().value());
            CHECK_EQUAL(1, Q.y().value());
        }
        {
            auto P = B;
            P *= ge(2);
            P = P.project();
            CHECK_EQUAL(2, P.x().value());
            CHECK_EQUAL(2, P.y().value());
        }
    }

    TEST(ecurve, rational_point_add) {
        {
            auto P = (B + B).project();
            CHECK_EQUAL(2, P.x().value());
            CHECK_EQUAL(2, P.y().value());
            auto Q = (P + B).project();
            CHECK_EQUAL(5, Q.x().value());
            CHECK_EQUAL(1, Q.y().value());
        }
        {
            auto P = B;
            P += B;
            P = P.project();
            CHECK_EQUAL(2, P.x().value());
            CHECK_EQUAL(2, P.y().value());
        }
    }

    TEST(ecurve, rational_point_negate) {
        {
            auto P = -B;
            CHECK_EQUAL(1, P.x().value());
            CHECK_EQUAL(4, P.y().value());
            CHECK_EQUAL(1, P.z().value());
        }
    }

    TEST(ecurve, rational_point_assign) {
        {
            auto P = rp(fe(2), fe(2), fe(2));
            P = B;
            CHECK_EQUAL(1, P.x().value());
            CHECK_EQUAL(3, P.y().value());
            CHECK_EQUAL(1, P.z().value());
        }
    }

    TEST(ecurve, rational_point_project) {
        {
            auto P = (rp(fe(1), fe(3), fe(2))).project();
            CHECK_EQUAL(2, P.x().value());
            CHECK_EQUAL(3, P.y().value());
            CHECK_EQUAL(1, P.z().value());
        }
    }
}
