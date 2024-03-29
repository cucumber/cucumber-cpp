#ifndef CUKE_CUKEENGINE_IMPL_HPP_
#define CUKE_CUKEENGINE_IMPL_HPP_

#include "CukeEngine.hpp"
#include <cucumber-cpp/internal/CukeExport.hpp>
#include "CukeCommands.hpp"

namespace cucumber {
namespace internal {

/**
 * Default Engine Implementation
 *
 * Currently it is a wrapper around CukeCommands. It will have its own
 * implementation when feature #31 is complete.
 */
class CUCUMBER_CPP_EXPORT CukeEngineImpl : public CukeEngine {
private:
    CukeCommands cukeCommands;

public:
    std::vector<StepMatch> stepMatches(const std::string& name) const override;
    void beginScenario(const tags_type& tags) override;
    void invokeStep(
        const std::string& id, const invoke_args_type& args, const invoke_table_type& tableArg
    ) override;
    void endScenario(const tags_type& tags) override;
    std::string snippetText(
        const std::string& keyword, const std::string& name, const std::string& multilineArgClass
    ) const override;
};

}
}

#endif /* CUKE_CUKEENGINE_IMPL_HPP_ */
