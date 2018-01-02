#include "eccrypto.h"
#include <typeinfo>

namespace ecc {
    void Process::initialize() {}

    void Process::join() {
        the_.join();
    }

    void Process::start() {
        initialize();
        the_ = std::thread(Process::enter, std::ref(*this));
    }

    const char* eof_exception::what() const throw() {
        return typeid(eof_exception).name();
    }
}
