#ifndef ECCRYPTOTEST_H
#define ECCRYPTOTEST_H

#include "../eccrypto.h"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <CppUTest/TestHarness.h>

#define DEBUG_PRINT(x) cout <<\
    #x <<\
    ":'" <<\
    (x) <<\
    "'" <<\
    endl
#define DEBUG_PRINTF(x, ...) cout <<\
    #x <<\
    ":'" <<\
    describe(__VA_ARGS__, (x)) <<\
    "'" <<\
    endl

using namespace std;

namespace ecc {
    template <class V> SimpleString StringFrom(const V& val);

    void dump_bits_to
        (ostream& os, const function<int()>& getBit);
    void dump_bytes_to
        (ostream& os, const function<int()>& getByt);

    template <class V> SimpleString StringFrom(const V& val) {
        ostringstream oss;
        oss << val;
        return SimpleString(oss.str().c_str());
    }
}

#endif
