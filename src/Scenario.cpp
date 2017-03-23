#include <cucumber-cpp/internal/Scenario.hpp>
#include <boost/make_shared.hpp>

namespace cucumber {
namespace internal {

Scenario::Scenario(const TagExpression::tag_list& tags) :
    tags(tags) {
};

const TagExpression::tag_list & Scenario::getTags() {
    return tags;
}

}
}
