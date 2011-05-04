#ifndef CUKEBINS_CUKECOMMANDS_HPP_
#define CUKEBINS_CUKECOMMANDS_HPP_

#include "step/StepManager.hpp"
#include "hook/HookRegistrar.hpp"
#include "ContextManager.hpp"
#include "Scenario.hpp"

#include <map>
#include <string>
#include <sstream>

#include <boost/shared_ptr.hpp>

namespace cukebins {
namespace internal {

using boost::shared_ptr;

class CukeCommands {
public:
    void beginScenario(const TagExpression::tag_list *tags);
    void endScenario();
    //virtual void snippetText() = 0;
    MatchResult stepMatches(const std::string description);
    InvokeResult invoke(step_id_type id, command_args_type *args);

private:
    StepManager stepManager;
    HookRegistrar hookRegistrar;
    ContextManager contextManager;

private:
    static shared_ptr<Scenario> currentScenario;
};

}
}

#endif /* CUKEBINS_CUKECOMMANDS_HPP_ */
