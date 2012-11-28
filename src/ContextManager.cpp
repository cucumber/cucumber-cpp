#include "cucumber-cpp/internal/ContextManager.hpp"

namespace cucumber {
namespace internal {

contexts_type ContextManager::contexts;

void ContextManager::purgeContexts() {
    contexts.clear();
}

}
}

