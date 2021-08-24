#include <cucumber-cpp/internal/utils/Regex.hpp>
#include <cucumber-cpp/internal/hook/HookRegistrar.hpp>
#include <boost/make_shared.hpp>

#include <algorithm>

namespace cucumber {
namespace internal {

Regex::Regex(std::string regularExpression) :
    regexImpl(regularExpression.c_str()) {
}

bool RegexMatch::matches() {
    return regexMatched;
}

const RegexMatch::submatches_type & RegexMatch::getSubmatches() {
    return submatches;
}

std::string Regex::str() const {
    return regexImpl.str();
}

boost::shared_ptr<RegexMatch> Regex::find(const std::string &expression) const {
    return boost::make_shared<FindRegexMatch>(regexImpl, expression);
}

namespace {
bool isUtf8CodeUnitStartOfCodepoint(unsigned int i) {
    return (i & 0xc0) != 0x80;
}

std::ptrdiff_t utf8CodepointOffset(const std::string& expression,
                                   const std::string::const_iterator& it) {
    return count_if(expression.begin(), it, &isUtf8CodeUnitStartOfCodepoint);
}
} // namespace

FindRegexMatch::FindRegexMatch(const boost::regex& regexImpl, const std::string& expression) {
    boost::smatch matchResults;
    regexMatched = boost::regex_search(
                       expression, matchResults, regexImpl, boost::regex_constants::match_extra)
                   && HookRegistrar::execStepMatchingHook(matchResults);
    if (regexMatched) {
        boost::smatch::const_iterator i = matchResults.begin();
        if (i != matchResults.end())
            // Skip capture group 0 which is the whole match, not a user marked sub-expression
            ++i;
        for (; i != matchResults.end(); ++i) {
            if (i->matched) {
                RegexSubmatch s = {*i, utf8CodepointOffset(expression, i->first)};
                submatches.push_back(s);
            } else {
                submatches.push_back(RegexSubmatch());
            }
        }
    }
}

boost::shared_ptr<RegexMatch> Regex::findAll(const std::string &expression) const {
    return boost::make_shared<FindAllRegexMatch>(regexImpl, expression);
}

FindAllRegexMatch::FindAllRegexMatch(const boost::regex &regexImpl, const std::string &expression) {
    boost::sregex_token_iterator i(expression.begin(), expression.end(), regexImpl, 1, boost::regex_constants::match_continuous);
    const boost::sregex_token_iterator end;
    for (; i != end; ++i) {
        RegexSubmatch s = {*i, -1};
        submatches.push_back(s);
    }
    regexMatched = !submatches.empty();
}

}
}
