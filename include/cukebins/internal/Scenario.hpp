#ifndef CUKEBINS_SCENARIO_HPP_
#define CUKEBINS_SCENARIO_HPP_

#include "hook/Tag.hpp"

namespace cukebins {
namespace internal {

class Scenario {
public:
    Scenario(const TagExpression::tag_list *pTags);

    const TagExpression::tag_list & getTags();
private:
    shared_ptr<const TagExpression::tag_list> pTags;
};

}
}

#endif /* CUKEBINS_SCENARIO_HPP_ */
