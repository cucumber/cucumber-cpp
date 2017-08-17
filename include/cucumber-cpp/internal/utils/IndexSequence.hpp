#ifndef CUKE_INDEXSEQ_HPP_
#define CUKE_INDEXSEQ_HPP_

#include <boost/config.hpp>

#ifndef BOOST_NO_VARIADIC_TEMPLATES

#if __cplusplus < 201402L && !(_MSC_VER > 1800)
    #include <cstddef> // for std::size_t
#else
    #include <utility>
#endif

namespace cucumber {
namespace internal {

#if __cplusplus < 201402L && !(_MSC_VER > 1800)
// Based on https://stackoverflow.com/questions/17424477/implementation-c14-make-integer-sequence

template <std::size_t... N>
struct index_sequence {
    typedef index_sequence type;
};

template <typename Seq1, typename Seq2>
struct concat_index_sequence;

template <std::size_t... N1, std::size_t... N2>
struct concat_index_sequence<
    index_sequence<N1...>
  , index_sequence<N2...>
  > : public index_sequence<
    N1...
  , (sizeof...(N1) + N2)...
  > {
};

template <std::size_t N>
struct make_index_sequence : public concat_index_sequence<
    typename make_index_sequence<  N/2>::type
  , typename make_index_sequence<N-N/2>::type
  > {
};

template <> struct make_index_sequence<0> : public index_sequence< > {};
template <> struct make_index_sequence<1> : public index_sequence<0> {};

template <typename... Ts>
using index_sequence_for = make_index_sequence<sizeof...(Ts)>;
#else
using ::std::index_sequence;
using ::std::index_sequence_for;
#endif

}
}

#endif /* !BOOST_NO_VARIADIC_TEMPLATES */

#endif /* CUKE_INDEXSEQ_HPP_ */
