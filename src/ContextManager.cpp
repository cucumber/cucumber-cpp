#include "cukebins/internal/ContextManager.hpp"

namespace cukebins {
namespace internal {

contexts_type ContextManager::contexts;

void ContextManager::purgeContexts() {
    contexts.clear();
}

}
}

