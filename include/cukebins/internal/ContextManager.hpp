#ifndef CUKEBINS_CONTEXTMANAGER_HPP_
#define CUKEBINS_CONTEXTMANAGER_HPP_

#include <vector>
#include <sstream>
#include <stdexcept>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace cukebins {
namespace internal {

using boost::shared_ptr;
using boost::weak_ptr;

class ContextManager {
public:
    typedef std::vector<shared_ptr<void> > contexts_type;

    void purgeContexts() {
        contexts.clear();
    }

    template<class T>
    weak_ptr<T> addContext() {
        shared_ptr<T> shared(new T);
        contexts.push_back(shared);
        return weak_ptr<T> (shared);
    }

protected:
    static contexts_type contexts;
};

ContextManager::contexts_type ContextManager::contexts = ContextManager::contexts_type();

}
}

#endif /* CUKEBINS_CONTEXTMANAGER_HPP_ */
