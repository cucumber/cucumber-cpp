#ifndef CUKEBINS_FAKEDRIVER_HPP_
#define CUKEBINS_FAKEDRIVER_HPP_

#include <cukebins/internal/CukeCommands.hpp>

namespace cukebins {
namespace internal {

class CukeCommands : public AbstractCommands {
public:
    void beginScenario() {
    }

protected:
    const InvokeResult invokeNoArgs(step_id_type id) {
        InvokeResult result;
        result.success = true;
        return result;
    }
};

}
}

#endif /* CUKEBINS_FAKEDRIVER_HPP_ */
