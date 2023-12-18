#include <cucumber-cpp/internal/utils/Regex.hpp>

#include <algorithm>

namespace cucumber {
namespace internal {

Regex::Regex(std::string regularExpression) :
    regexImpl(regularExpression),
    regexString(regularExpression) {
}

bool RegexMatch::matches() {
    return regexMatched;
}

const RegexMatch::submatches_type& RegexMatch::getSubmatches() {
    return submatches;
}

std::string Regex::str() const {
    return regexString;
}

std::shared_ptr<RegexMatch> Regex::find(const std::string& expression) const {
    return std::make_shared<FindRegexMatch>(regexImpl, expression);
}

namespace {
bool isUtf8CodeUnitStartOfCodepoint(unsigned int i) {
    return (i & 0xc0) != 0x80;
}

std::ptrdiff_t utf8CodepointOffset(
    const std::string& expression, const std::string::const_iterator& it
) {
    return count_if(expression.begin(), it, &isUtf8CodeUnitStartOfCodepoint);
}
} // namespace

FindRegexMatch::FindRegexMatch(const std::regex& regexImpl, const std::string& expression) {
    std::smatch matchResults;
    regexMatched = std::regex_search(expression, matchResults, regexImpl);
    if (regexMatched) {
        std::smatch::const_iterator i = matchResults.begin();
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

std::shared_ptr<RegexMatch> Regex::findAll(const std::string& expression) const {
    return std::make_shared<FindAllRegexMatch>(regexImpl, expression);
}

FindAllRegexMatch::FindAllRegexMatch(const std::regex& regexImpl, const std::string& expression) {
    std::sregex_token_iterator i(
        expression.begin(), expression.end(), regexImpl, 1, std::regex_constants::match_continuous
    );
    const std::sregex_token_iterator end;
    for (; i != end; ++i) {
        RegexSubmatch s = {*i, -1};
        submatches.push_back(s);
    }
    regexMatched = !submatches.empty();
}

}
}
