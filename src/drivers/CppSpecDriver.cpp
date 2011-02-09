#include <cukebins/internal/drivers/CppSpecDriver.hpp>

#include <CppSpec/CppSpec.h>

namespace cukebins {
namespace internal {

const InvokeResult CppSpecStep::invokeStepBody() {
    InvokeResult result;
    try {
        stepBody();
        result.success = true;
    } catch (...) {
    }
    return result;
}

}
}
