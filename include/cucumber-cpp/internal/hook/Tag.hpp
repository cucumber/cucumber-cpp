#ifndef CUKE_TAG_HPP_
#define CUKE_TAG_HPP_

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "CukeExport.hpp"
#include "../utils/Regex.hpp"

namespace cucumber {
namespace internal {

class CUCUMBER_CPP_EXPORT TagExpression {
public:
    typedef std::vector<std::string> tag_list;

    virtual ~TagExpression() { }
    virtual bool matches(const tag_list &tags) = 0;
};

class CUCUMBER_CPP_EXPORT OrTagExpression : public TagExpression {
public:
    OrTagExpression(const std::string &csvTagNotation);
    bool matches(const tag_list &tags);

private:
    bool orTagMatchesTagList(const std::string &currentOrTag, const tag_list &tags);

    tag_list orTags;

    static Regex & csvTagNotationRegex();
};

class CUCUMBER_CPP_EXPORT AndTagExpression : public TagExpression {
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

#endif /* CUKE_TAG_HPP_ */
