#ifndef CUKEBINS_FAKEDRIVER_HPP_
#define CUKEBINS_FAKEDRIVER_HPP_

#include <cukebins/internal/CukeFixture.hpp>
#include <cukebins/internal/CukeCommands.hpp>

#define CUKE_TEST_FULLNAME_(feature_name, step_name) #feature_name "." #step_name
#define CUKE_TEST_NAME_PREFIX_ ""
#define CUKE_STEPCLASS_INHERITANCE_(parent_class)
#define CUKE_INHERITED_CONSTRUCTOR_(parent_class)
#define CUKE_STEPCLASS_TESTBODY_NAME_ TestBody
#define CUKE_STEPCLASS_OTHER_DECLARATIONS_(feature_class, step_name)
#define CUKE_STEPCLASS_OTHER_DEFINITION_(feature_class, step_name, parent_class, parent_id)

namespace cukebins {
namespace internal {

template<class T>
class CukeFixture : public BaseCukeFixture<T> {
};

class CukeCommands : public AbstractCommands {
public:
    void beginScenario() {
    }

protected:
    InvokeResult invokeNoArgs(StepInfo::id_type id) {
        InvokeResult result;
        result.success = true;
        return result;
    }
};

}
}

#endif /* CUKEBINS_FAKEDRIVER_HPP_ */
