#ifndef CUKE_INDEXSEQ_HPP_
#define CUKE_INDEXSEQ_HPP_

#if __cplusplus < 201402L
    #include <cstddef> // for std::size_t
#else
    #include <utility>
#endif

namespace cucumber {
namespace internal {

#if __cplusplus < 201103L
// Special case for zero, only thing we bother to support on C++98
template <typename = void>
struct index_sequence {
    typedef index_sequence type;
};

template <std::size_t N>
struct make_index_sequence;

template <> struct make_index_sequence<0> : public index_sequence< > {};
#elif __cplusplus < 201402L
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
#else
using ::std::index_sequence;
using ::std::make_index_sequence;
#endif

}
}

#endif /* CUKE_INDEXSEQ_HPP_ */
