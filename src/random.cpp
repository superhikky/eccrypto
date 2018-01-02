#include "eccrypto.h"
#include <chrono>
#include <climits>

namespace ecc {
    PseudoRandomBitGenerator::PseudoRandomBitGenerator() :
        dis_(0, 1),
        gen_(std::chrono::system_clock::now().time_since_epoch().count())
    {}

    bool PseudoRandomBitGenerator::getBit() {
        return dis_(gen_);
    }
}
