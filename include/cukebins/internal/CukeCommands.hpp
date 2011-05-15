#ifndef CUKEBINS_CUKECOMMANDS_HPP_
#define CUKEBINS_CUKECOMMANDS_HPP_

#include "ContextManager.hpp"
#include "Scenario.hpp"
#include "Table.hpp"
#include "step/StepManager.hpp"
#include "hook/HookRegistrar.hpp"

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
    const std::string snippetText(const std::string stepKeyword, const std::string stepName) const;
    MatchResult stepMatches(const std::string description);
    InvokeResult invoke(step_id_type id, const InvokeArgs * pArgs);

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
