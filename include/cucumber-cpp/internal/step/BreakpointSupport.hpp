#ifndef CUKE_BREAKPOINTSUPPORT_HPP_
#define CUKE_BREAKPOINTSUPPORT_HPP_

#include "../CukeDll.hpp"

namespace cucumber {
namespace internal {

#if defined(_MSC_VER)
// The ", true" makes the expression to be of type bool, which is required
// for the CUKE_BREAK_INTO_DEBUGGER macro
#define CUKE_BREAK_INTO_DEBUGGER (__debugbreak(), true)

#elif defined(__clang__) && defined(__APPLE__) && !TARGET_OS_IPHONE
#define CUKE_BREAK_INTO_DEBUGGER                                     \
  _Pragma("clang diagnostic push")                                   \
  _Pragma("clang diagnostic ignored \"-Wgnu-statement-expression\"") \
    ({ asm("int3"); true; })                                         \
  _Pragma("clang diagnostic pop")

#else // Generic Unix
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>

#define CUKE_BREAK_INTO_DEBUGGER raise(SIGTRAP)

#endif // COMPILER CHECK


class CUKE_API_ BreakpointSupport {
public:
  static void shouldBreakIntoDebuggerInNextStep();
  static bool checkAndClearShouldBreakIntoDebugger();
};

}
}

#endif /* CUKE_BREAKPOINTSUPPORT_HPP_ */
