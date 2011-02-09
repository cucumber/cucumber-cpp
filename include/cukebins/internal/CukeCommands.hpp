#ifndef CUKEBINS_CUKECOMMANDS_HPP_
#define CUKEBINS_CUKECOMMANDS_HPP_

#include "StepManager.hpp"
#include "ContextManager.hpp"

#include <map>
#include <string>
#include <sstream>

#include <boost/shared_ptr.hpp>

namespace cukebins {
namespace internal {

using boost::shared_ptr;

class CukeCommands {
public:
    void beginScenario();
    void endScenario();
    //virtual void snippetText() = 0;
    MatchResult stepMatches(const std::string description);
    InvokeResult invoke(step_id_type id, shared_ptr<command_args_type> args);

private:
    StepManager stepManager;
    ContextManager contextManager;
};

}
}

#endif /* CUKEBINS_CUKECOMMANDS_HPP_ */
