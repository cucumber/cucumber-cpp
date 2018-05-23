#ifndef CUKE_CONTEXTMANAGER_HPP_
#define CUKE_CONTEXTMANAGER_HPP_

#include <cucumber-cpp/internal/CukeExport.hpp>

#include <vector>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace cucumber {

namespace internal {

typedef std::vector<boost::shared_ptr<void> > contexts_type;

class CUCUMBER_CPP_EXPORT ContextManager {
public:
    void purgeContexts();
    template<class T> boost::weak_ptr<T> addContext();

protected:
    static contexts_type contexts;
};

template<class T>
boost::weak_ptr<T> ContextManager::addContext() {
    boost::shared_ptr<T> shared(boost::make_shared<T>());
    contexts.push_back(shared);
    return boost::weak_ptr<T> (shared);
}

}

template<class T>
class ScenarioScope {
public:
    ScenarioScope();

    T& operator*();
    T* operator->();
    T* get();

private:
    internal::ContextManager contextManager;
    boost::shared_ptr<T> context;
    static boost::weak_ptr<T> contextReference;
};

template<class T>
boost::weak_ptr<T> ScenarioScope<T>::contextReference;

template<class T>
ScenarioScope<T>::ScenarioScope() {
     if (contextReference.expired()) {
          contextReference = contextManager.addContext<T> ();
     }
     context = contextReference.lock();
}

template<class T>
T& ScenarioScope<T>::operator*() {
    return *(context.get());
}

template<class T>
T* ScenarioScope<T>::operator->() {
    return (context.get());
}

template<class T>
T* ScenarioScope<T>::get() {
    return context.get();
}

}

#endif /* CUKE_CONTEXTMANAGER_HPP_ */
