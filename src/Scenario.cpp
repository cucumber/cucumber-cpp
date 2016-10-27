#include <cucumber-cpp/internal/Scenario.hpp>
#include <boost/make_shared.hpp>

namespace cucumber {
namespace internal {

Scenario::Scenario(const TagExpression::tag_list *pTags) :
    pTags(pTags) {
    if (!pTags) {
        this->pTags = boost::make_shared<const TagExpression::tag_list>();
    }
};

const TagExpression::tag_list & Scenario::getTags() {
    return *(pTags.get());
}

}
}
