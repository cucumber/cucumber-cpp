#include "cucumber-cpp/internal/drivers/GenericDriver.hpp"

namespace cucumber {
namespace internal {

const InvokeResult GenericStep::invokeStepBody() {
    // No try/catch block to throw the original exceptions to the testing framework
    body();
    return InvokeResult::success();
}

}
}
