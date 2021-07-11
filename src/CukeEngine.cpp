#include "cucumber-cpp/internal/CukeEngine.hpp"

namespace cucumber {
namespace internal {

InvokeException::InvokeException(const std::string & message) :
    message(message) {
}

InvokeException::InvokeException(const InvokeException &rhs) :
    message(rhs.message) {
}

const std::string InvokeException::getMessage() const {
    return message;
}


CukeEngine::CukeEngine() {}

CukeEngine::~CukeEngine() {}
}
}
