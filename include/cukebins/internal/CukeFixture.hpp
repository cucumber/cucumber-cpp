#ifndef CUKEBINS_CUKEFIXTURE_HPP_
#define CUKEBINS_CUKEFIXTURE_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace cukebins {
namespace internal {

template<class T>
class BaseCukeFixture {
public:
    BaseCukeFixture();
protected:
    shared_ptr<T> context;
    static weak_ptr<T> contextReference;
private:
    ContextManager contextManager;
};

template<class T> BaseCukeFixture<T>::BaseCukeFixture() {
    if (contextReference.expired()) {
        contextReference = contextManager.addContext<T> ();
    }
    context = contextReference.lock();
}
;

template<class T> weak_ptr<T> BaseCukeFixture<T>::contextReference;

}
}

#endif /* CUKEBINS_CUKEFIXTURE_HPP_ */
