#include <cucumber-cpp/internal/connectors/wire/WireProtocol.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireProtocolCommands.hpp>

#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
#include <sstream>

using json = nlohmann::json;

namespace cucumber {
namespace internal {

/*
 * Responses
 */

void SuccessResponse::accept(WireResponseVisitor& visitor) const {
    visitor.visit(*this);
}

FailureResponse::FailureResponse(const std::string& message, const std::string& exceptionType) :
    message(message),
    exceptionType(exceptionType) {
}

const std::string FailureResponse::getMessage() const {
    return message;
}

const std::string FailureResponse::getExceptionType() const {
    return exceptionType;
}

void FailureResponse::accept(WireResponseVisitor& visitor) const {
    visitor.visit(*this);
}

PendingResponse::PendingResponse(const std::string& message) :
    message(message) {
}

const std::string PendingResponse::getMessage() const {
    return message;
}

void PendingResponse::accept(WireResponseVisitor& visitor) const {
    visitor.visit(*this);
}

StepMatchesResponse::StepMatchesResponse(const std::vector<StepMatch>& matchingSteps) :
    matchingSteps(matchingSteps) {
}

const std::vector<StepMatch>& StepMatchesResponse::getMatchingSteps() const {
    return matchingSteps;
}

void StepMatchesResponse::accept(WireResponseVisitor& visitor) const {
    visitor.visit(*this);
}

SnippetTextResponse::SnippetTextResponse(const std::string& stepSnippet) :
    stepSnippet(stepSnippet) {
}

const std::string SnippetTextResponse::getStepSnippet() const {
    return stepSnippet;
}

void SnippetTextResponse::accept(WireResponseVisitor& visitor) const {
    visitor.visit(*this);
}

/*
 * Command decoders
 */

namespace {
typedef std::shared_ptr<WireCommand> (*CommandDecoder)(const json& jsonArgs);

CukeEngine::tags_type getScenarioTags(const json& jsonArgs) {
    CukeEngine::tags_type tags;
    if (!jsonArgs.is_null()) {
        const auto& jsonTags = jsonArgs.at("tags");
        for (const auto& tag : jsonTags) {
            tags.push_back(tag.get<std::string>());
        }
    }
    return tags;
}

std::shared_ptr<WireCommand> BeginScenarioDecoder(const json& jsonArgs) {
    return std::make_shared<BeginScenarioCommand>(getScenarioTags(jsonArgs));
}

std::shared_ptr<WireCommand> EndScenarioDecoder(const json& jsonArgs) {
    return std::make_shared<EndScenarioCommand>(getScenarioTags(jsonArgs));
}

std::shared_ptr<WireCommand> StepMatchesDecoder(const json& jsonArgs) {
    const std::string& nameToMatch = jsonArgs.at("name_to_match");
    return std::make_shared<StepMatchesCommand>(nameToMatch);
}

void fillTableArg(const json& jsonTableArg, CukeEngine::invoke_table_type& tableArg) {
    const std::size_t rows = jsonTableArg.size();
    if (rows > 0) {
        const std::size_t columns = jsonTableArg[0].get<std::vector<std::string>>().size();
        tableArg.resize(rows);
        for (std::size_t i = 0; i < rows; ++i) {
            const auto& jsonRow(jsonTableArg[i].get<std::vector<std::string>>());
            if (jsonRow.size() == columns) {
                for (std::size_t j = 0; j < columns; ++j) {
                    tableArg[i].push_back(jsonRow[j]);
                }
            } else {
                // TODO: Invalid row
            }
        }
    } else {
        // TODO: Invalid table (no column specified)
    }
}

void fillInvokeArgs(
    const json& invokeParams,
    CukeEngine::invoke_args_type& args,
    CukeEngine::invoke_table_type& tableArg
) {
    const auto& jsonArgs = invokeParams.at("args");
    for (const auto& arg : jsonArgs) {
        if (arg.is_string()) {
            args.push_back(arg.get<std::string>());
        } else if (arg.is_array()) {
            fillTableArg(arg, tableArg);
        }
    }
}

std::shared_ptr<WireCommand> InvokeDecoder(const json& jsonArgs) {
    const auto& invokeParams = jsonArgs.get<json::object_t>();

    CukeEngine::invoke_args_type args;
    CukeEngine::invoke_table_type tableArg;
    const std::string& id = invokeParams.at("id");
    fillInvokeArgs(invokeParams, args, tableArg);
    return std::make_shared<InvokeCommand>(id, args, tableArg);
}

std::shared_ptr<WireCommand> SnippetTextDecoder(const json& jsonArgs) {
    const auto& snippetTextArgs = jsonArgs.get<json::object_t>();
    const std::string& stepKeyword = snippetTextArgs.at("step_keyword");
    const std::string& stepName = snippetTextArgs.at("step_name");
    const std::string& multilineArgClass = snippetTextArgs.at("multiline_arg_class");
    return std::make_shared<SnippetTextCommand>(stepKeyword, stepName, multilineArgClass);
}
}

static const std::map<std::string, CommandDecoder> commandDecodersMap = {
    {"begin_scenario", BeginScenarioDecoder},
    {"end_scenario", EndScenarioDecoder},
    {"step_matches", StepMatchesDecoder},
    {"invoke", InvokeDecoder},
    {"snippet_text", SnippetTextDecoder},
};

std::shared_ptr<WireCommand> JsonWireMessageCodec::decode(const std::string& request) const {
    try {
        json jsonRequest = json::parse(request);
        const auto& jsonCommand = jsonRequest.at(0);

        const auto& commandDecoder = commandDecodersMap.find(jsonCommand.get<std::string>());
        if (commandDecoder != commandDecodersMap.end() && commandDecoder->second) {
            json jsonArgs;
            if (jsonRequest.size() > 1) {
                jsonArgs = jsonRequest.at(1);
            }
            return commandDecoder->second(jsonArgs);
        }
    } catch (...) {
        // LOG Error decoding wire protocol command
    }
    return std::make_shared<FailingCommand>();
}

namespace {

class WireResponseEncoder : public WireResponseVisitor {
private:
    json jsonOutput = json::array();

    void success(const json* detail = nullptr) {
        output("success", detail);
    }

    void fail(const json* detail = nullptr) {
        output("fail", detail);
    }

    void output(const std::string& responseType, const json* detail = nullptr) {
        jsonOutput.push_back(responseType);
        if (detail == nullptr || detail->is_null()) {
            return;
        }
        jsonOutput.push_back(*detail);
    }

public:
    std::string encode(const WireResponse& response) {
        jsonOutput.clear();
        response.accept(*this);
        return jsonOutput.dump();
    }

    void visit(const SuccessResponse& /*response*/) override {
        success();
    }

    void visit(const FailureResponse& response) override {
        json detailObject;
        if (!response.getMessage().empty()) {
            detailObject["message"] = response.getMessage();
        }
        if (!response.getExceptionType().empty()) {
            detailObject["exception"] = response.getExceptionType();
        }
        if (detailObject.empty()) {
            fail();
        } else {
            const json detail(detailObject);
            fail(&detail);
        }
    }

    void visit(const PendingResponse& response) override {
        json jsonReponse(response.getMessage());
        output("pending", &jsonReponse);
    }

    void visit(const StepMatchesResponse& response) override {
        json jsonMatches = json::array();
        for (const StepMatch& m : response.getMatchingSteps()) {
            json jsonM;
            jsonM["id"] = m.id;
            json jsonArgs = json::array();
            for (const StepMatchArg& ma : m.args) {
                json jsonMa;
                jsonMa["val"] = ma.value;
                jsonMa["pos"] = static_cast<int64_t>(ma.position);
                jsonArgs.push_back(jsonMa);
            }
            jsonM["args"] = jsonArgs;
            if (!m.source.empty()) {
                jsonM["source"] = m.source;
            }
            if (!m.regexp.empty()) {
                jsonM["regexp"] = m.regexp;
            }
            jsonMatches.push_back(jsonM);
        }
        json jsonReponse(jsonMatches);
        output("success", &jsonReponse);
    }

    void visit(const SnippetTextResponse& response) override {
        json jsonReponse(response.getStepSnippet());
        success(&jsonReponse);
    }
};

}

const std::string JsonWireMessageCodec::encode(const WireResponse& response) const {
    try {
        WireResponseEncoder encoder;
        return encoder.encode(response);
    } catch (...) {
        throw WireMessageCodecException("Error decoding wire protocol response");
    }
}

WireProtocolHandler::WireProtocolHandler(const WireMessageCodec& codec, CukeEngine& engine) :
    codec(codec),
    engine(engine) {
}

std::string WireProtocolHandler::handle(const std::string& request) const {
    std::string response;
    // LOG request
    try {
        std::shared_ptr<const WireCommand> command = codec.decode(request);
        std::shared_ptr<const WireResponse> wireResponse = command->run(engine);
        response = codec.encode(*wireResponse);
    } catch (...) {
        response = "[\"fail\"]";
    }
    // LOG response
    return response;
}

} // namespace internal
} // namespace cucumber
