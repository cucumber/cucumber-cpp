#include <cucumber-cpp/internal/hook/Tag.hpp>
#include <memory>

namespace cucumber {
namespace internal {

Regex& AndTagExpression::csvTagNotationRegex() {
    static Regex r("\\s*\"([^\"]+)\"\\s*(?:,|$)");
    return r;
}

AndTagExpression::AndTagExpression(const std::string& csvTagNotation) {
    const std::shared_ptr<RegexMatch> match(csvTagNotationRegex().findAll(csvTagNotation));
    const RegexMatch::submatches_type submatches = match->getSubmatches();
    orExpressions.reserve(submatches.size());
    for (RegexMatch::submatches_type::const_iterator i = submatches.begin(); i != submatches.end();
         ++i) {
        const std::string orCsvTagNotation = i->value;
        orExpressions.push_back(OrTagExpression(orCsvTagNotation));
    }
}

bool AndTagExpression::matches(const tag_list& tags) const {
    bool match = true;
    for (or_expressions_type::const_iterator i = orExpressions.begin();
         i != orExpressions.end() && match;
         ++i) {
        match &= i->matches(tags);
    }
    return match;
}

Regex& OrTagExpression::csvTagNotationRegex() {
    static Regex r("\\s*@(\\w+)\\s*(?:,|$)");
    return r;
}

OrTagExpression::OrTagExpression(const std::string& csvTagNotation) {
    const std::shared_ptr<RegexMatch> match(csvTagNotationRegex().findAll(csvTagNotation));
    const RegexMatch::submatches_type submatches = match->getSubmatches();
    orTags.reserve(submatches.size());
    for (RegexMatch::submatches_type::const_iterator i = submatches.begin(); i != submatches.end();
         ++i) {
        orTags.push_back(i->value);
    }
}

bool OrTagExpression::matches(const tag_list& tags) const {
    for (tag_list::const_iterator i = orTags.begin(); i != orTags.end(); ++i) {
        if (orTagMatchesTagList(*i, tags))
            return true;
    }
    return false;
}

bool OrTagExpression::orTagMatchesTagList(const std::string& currentOrTag, const tag_list& tags)
    const {
    for (tag_list::const_iterator i = tags.begin(); i != tags.end(); ++i) {
        if (*i == currentOrTag)
            return true;
    }
    return false;
}

}
}
