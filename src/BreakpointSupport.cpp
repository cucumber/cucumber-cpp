#if defined(CUKE_ENABLE_BREAKPOINT_SUPPORT)

#include <cucumber-cpp/internal/step/BreakpointSupport.hpp>

namespace cucumber {
namespace internal {

static bool shouldBreakIntoDebugger = false;

CUKE_API_ void BreakpointSupport::shouldBreakIntoDebuggerInNextStep() {
  shouldBreakIntoDebugger = true;
}

CUKE_API_ bool BreakpointSupport::checkAndClearShouldBreakIntoDebugger() {
  if (shouldBreakIntoDebugger) {
    shouldBreakIntoDebugger = false;
    return true;
  }
  return false;
}

}
}

#endif // CUKE_ENABLE_BREAKPOINT_SUPPORT
