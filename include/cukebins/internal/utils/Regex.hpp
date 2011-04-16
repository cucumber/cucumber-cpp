#ifndef CUKEBINS_REGEX_HPP_
#define CUKEBINS_REGEX_HPP_

#include <vector>

#include <boost/regex.hpp>

namespace cukebins {
namespace internal {

struct RegexSubmatch {
    std::string value;
    int position;
};

class RegexMatch {
public:
    typedef std::vector<RegexSubmatch> submatches_type;

    RegexMatch(const boost::regex &regexImpl, const std::string &expression);

    bool matches();
    submatches_type getSubmatches();

private:
    bool regexMatched;
    submatches_type submatches;
};

class Regex {
private:
    boost::regex regexImpl;

public:
    Regex(std::string expr);

    RegexMatch find(const std::string &expression);

    std::string str();
};

}
}

#endif /* CUKEBINS_REGEX_HPP_ */

