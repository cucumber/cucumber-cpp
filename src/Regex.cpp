#include <cukebins/internal/utils/Regex.hpp>

namespace cukebins {
namespace internal {

Regex::Regex(std::string regularExpression) :
    regexImpl(regularExpression.c_str()) {
}

RegexMatch Regex::find(const std::string &expression) {
    return RegexMatch(regexImpl, expression);
}

RegexMatch::RegexMatch(const boost::regex &regexImpl, const std::string &expression) {
    boost::cmatch matchResults;
    regexMatched = boost::regex_search(expression.c_str(), matchResults, regexImpl);
    if (regexMatched) {
        for (boost::cmatch::size_type i = 1; i < matchResults.size(); ++i) {
            RegexSubmatch s;
            s.value = matchResults.str(i);
            s.position = matchResults.position(i);
            submatches.push_back(s);
        }
    }
}

bool RegexMatch::matches() {
    return regexMatched;
}

RegexMatch::submatches_type RegexMatch::getSubmatches() {
    return submatches;
}

std::string Regex::str() {
    return regexImpl.str();
}

}
}
