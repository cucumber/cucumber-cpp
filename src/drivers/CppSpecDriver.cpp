#include <cukebins/internal/drivers/CppSpecDriver.hpp>

#include <CppSpec/CppSpec.h>

namespace cukebins {
namespace internal {

const InvokeResult CppSpecStep::invokeStepBody() {
    try {
        body();
        return InvokeResult::success();
    } catch (const ::CppSpec::SpecifyFailedException &e) {
        return InvokeResult::failure(e.message);
    }
}

}
}
