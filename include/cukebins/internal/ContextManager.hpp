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

typedef std::vector<shared_ptr<void> > contexts_type;

class ContextManager {
public:
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

contexts_type ContextManager::contexts = contexts_type();


template<class T>
class SessionContextPtr {
public:
    SessionContextPtr() {
         if (contextReference.expired()) {
              contextReference = contextManager.addContext<T> ();
         }
         context = contextReference.lock();
    }

    T& operator*() { return *(context.get()); }
    T* operator->() { return (context.get()); }

private:
    shared_ptr<T> context;
    static weak_ptr<T> contextReference;

    ContextManager contextManager;
};

template<class T> weak_ptr<T> SessionContextPtr<T>::contextReference;

}
}

#endif /* CUKEBINS_CONTEXTMANAGER_HPP_ */
