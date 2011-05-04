#ifndef CUKEBINS_TAG_HPP_
#define CUKEBINS_TAG_HPP_

#include <string>
#include <list>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <cukebins/internal/utils/Regex.hpp>

namespace cukebins {
namespace internal {

class TagExpression {
public:
    typedef std::list<std::string> tag_list;

    virtual bool matches(const tag_list &tags) = 0;
};

class OrTagExpression : public TagExpression {
public:
    OrTagExpression(const std::string &csvTagNotation);
    bool matches(const tag_list &tags);

private:
    bool orTagMatchesTagList(const std::string &currentOrTag, const tag_list &tags);

    tag_list orTags;

    static Regex & csvTagNotationRegex();
};

class AndTagExpression : public TagExpression {
public:
    AndTagExpression(const std::string &csvTagNotation);
    bool matches(const tag_list &tags);

private:
    typedef std::list<shared_ptr<OrTagExpression> > or_expressions_type;
    or_expressions_type orExpressions;

    static Regex & csvTagNotationRegex();
};

}
}

#endif /* CUKEBINS_TAG_HPP_ */
