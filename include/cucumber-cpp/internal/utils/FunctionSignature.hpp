#ifndef CUKE_FUNCTIONSIGNATURE_HPP_
#define CUKE_FUNCTIONSIGNATURE_HPP_

#include <boost/config.hpp>

namespace cucumber {
namespace internal {

template <typename>
struct FunctionSignature;

#ifdef BOOST_NO_VARIADIC_TEMPLATES
// Special case for zero arguments, only thing we bother to support on C++98
template <typename = void>
struct FunctionArgs
{
    enum { size = 0 };
};

template <typename R>
struct FunctionSignature<R()>
{
    typedef R              result_type;
    typedef FunctionArgs<> args_type;
};
#else
template <typename... Args>
struct FunctionArgs
{
    static constexpr auto size = sizeof...(Args);
};

template <typename R, typename... Args>
struct FunctionSignature<R(Args...)>
{
    typedef R                     result_type;
    typedef FunctionArgs<Args...> args_type;
};
#endif

}
}

#endif /* CUKE_FUNCTIONSIGNATURE_HPP_ */
