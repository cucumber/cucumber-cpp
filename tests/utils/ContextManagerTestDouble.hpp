#ifndef CUKEBINS_CONTEXTMANAGERTESTDOUBLE_HPP_
#define CUKEBINS_CONTEXTMANAGERTESTDOUBLE_HPP_

#include <cukebins/internal/ContextManager.hpp>

namespace cukebins {
namespace internal {

class ContextManagerTestDouble : public ContextManager {
public:
    contexts_type::size_type countContexts() {
        return ContextManager::contexts.size();
    }
};

}
}

#endif /* CUKEBINS_CONTEXTMANAGERTESTDOUBLE_HPP_ */
