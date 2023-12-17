#ifndef CUKE_WIREPROTOCOL_COMMANDS_HPP_
#define CUKE_WIREPROTOCOL_COMMANDS_HPP_

#include "WireProtocol.hpp"
#include <memory>

namespace cucumber {
namespace internal {

class ScenarioCommand : public WireCommand {
protected:
    const CukeEngine::tags_type tags;

    ScenarioCommand(const CukeEngine::tags_type& tags);
};

class BeginScenarioCommand : public ScenarioCommand {
public:
    BeginScenarioCommand(const CukeEngine::tags_type& tags);

    std::shared_ptr<WireResponse> run(CukeEngine& engine) const override;
};

class EndScenarioCommand : public ScenarioCommand {
public:
    EndScenarioCommand(const CukeEngine::tags_type& tags);

    std::shared_ptr<WireResponse> run(CukeEngine& engine) const override;
};

class StepMatchesCommand : public WireCommand {
private:
    const std::string stepName;

public:
    StepMatchesCommand(const std::string& stepName);

    std::shared_ptr<WireResponse> run(CukeEngine& engine) const override;
};

class InvokeCommand : public WireCommand {
private:
    const std::string stepId;
    const CukeEngine::invoke_args_type args;
    const CukeEngine::invoke_table_type tableArg;

public:
    InvokeCommand(
        const std::string& stepId,
        const CukeEngine::invoke_args_type& args,
        const CukeEngine::invoke_table_type& tableArg
    );

    std::shared_ptr<WireResponse> run(CukeEngine& engine) const override;
};

class SnippetTextCommand : public WireCommand {
private:
    std::string keyword, name, multilineArgClass;

public:
    SnippetTextCommand(
        const std::string& keyword, const std::string& name, const std::string& multilineArgClass
    );

    std::shared_ptr<WireResponse> run(CukeEngine& engine) const override;
};

class FailingCommand : public WireCommand {
public:
    std::shared_ptr<WireResponse> run(CukeEngine& engine) const override;
};

}
}

#endif /* CUKE_WIREPROTOCOL_COMMANDS_HPP_ */
