#ifndef CUKE_REGEX_HPP_
#define CUKE_REGEX_HPP_

#include <cstddef>
#include <vector>

#include <regex>

namespace cucumber {
namespace internal {

struct RegexSubmatch {
    std::string value;
    std::ptrdiff_t position;
};

class RegexMatch {
public:
    typedef std::vector<RegexSubmatch> submatches_type;

    virtual ~RegexMatch() = default;

    bool matches();
    const submatches_type& getSubmatches();

protected:
    bool regexMatched;
    submatches_type submatches;
};

class FindRegexMatch : public RegexMatch {
public:
    FindRegexMatch(const std::regex& regexImpl, const std::string& expression);
};

class FindAllRegexMatch : public RegexMatch {
public:
    FindAllRegexMatch(const std::regex& regexImpl, const std::string& expression);
};

class Regex {
private:
    std::regex regexImpl;
    const std::string regexString;

public:
    Regex(std::string expr);

    std::shared_ptr<RegexMatch> find(const std::string& expression) const;
    std::shared_ptr<RegexMatch> findAll(const std::string& expression) const;

    std::string str() const;
};

}
}

#endif /* CUKE_REGEX_HPP_ */
