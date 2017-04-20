#ifndef CUKE_CUKEDLL_HPP_
#define CUKE_CUKEDLL_HPP_

#ifndef CUKE_API_
#if CUKE_LINKED_AS_SHARED_LIBRARY
#define CUKE_API_ __declspec(dllimport)
#elif CUKE_CREATE_SHARED_LIBRARY
#define CUKE_API_ __declspec(dllexport)
#else
#define CUKE_API_
#endif
#endif

#endif /* CUKE_CUKEDLL_HPP_ */
