#ifndef CUKEBINS_WIREPROTOCOL_HPP_
#define CUKEBINS_WIREPROTOCOL_HPP_

#include <json_spirit/json_spirit_reader_template.h>
#include <json_spirit/json_spirit_writer_template.h>
#include <cukebins/internal/StepManager.hpp>
#include <cukebins/internal/CukeCommands.hpp>

#include <string>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/assign/list_of.hpp>

namespace cukebins {
namespace internal {

using json_spirit::mValue;
using json_spirit::mArray;
using json_spirit::mObject;
using std::make_pair;

class NetworkProtocol {
public:
    virtual void processStream(std::iostream &stream) = 0;
};

class JSONCommand {
protected:
    CukeCommands commands;
public:
    static mValue success_response() {
        mArray a;
        a.push_back("success");
        return mValue(a);
    }
    static mValue fail_response() {
        mArray a;
        a.push_back("fail");
        return mValue(a);
    }

    virtual mValue run(mValue &jsonArgs) = 0;
};

class BeginScenarioCommand : public JSONCommand {
public:
    mValue run(mValue &jsonArgs) {
        if (jsonArgs.is_null()) {
            commands.beginScenario();
            return success_response();
        } else {
            return fail_response();
        }
    }
};

class EndScenarioCommand : public JSONCommand {
public:
    mValue run(mValue &jsonArgs) {
        if (jsonArgs.is_null()) {
            commands.endScenario();
            return success_response();
        } else {
            return fail_response();
        }
    }
};

class StepMatchesCommand : public JSONCommand {
public:
    mValue run(mValue &jsonArgs) {
        std::string matcher = getStepMatchesMatcher(jsonArgs);
        mValue v;
        return formatResponse(commands.stepMatches(matcher));
    }
private:
    std::string getStepMatchesMatcher(mValue &jsonArgs) {
        mObject args = jsonArgs.get_obj();
        std::string matcher(args["name_to_match"].get_str());
        return matcher;
    }

    mValue formatResponse(MatchResult matchResult) {
        mValue response = success_response();
        mArray matches;
        MatchResult::results_type resultSet = matchResult.getResultSet();
        for (MatchResult::results_type::iterator i = resultSet.begin(); i != resultSet.end(); ++i) {
            mObject match;
            match.insert(make_pair("id", mValue(toString(i->id))));
            mArray matchArgs;
            for (Match::subexpressions_type::iterator j = i->subExpressions.begin(); j != i->subExpressions.end(); ++j) {
                mObject arg;
                arg.insert(make_pair("val", mValue(j->value)));
                arg.insert(make_pair("pos", mValue(j->position)));
                matchArgs.push_back(arg);
            }
            match.insert(make_pair("args", mValue(matchArgs)));
            matches.push_back(match);
        }
        response.get_array().push_back(matches);
        return response;
    }

};

class InvokeCommand : public JSONCommand {
public:
    mValue run(mValue &jsonArgs) {
        StepInfo::id_type id = getInvokeId(jsonArgs);
        shared_ptr<AbstractCommands::args_type> args(getInvokeArgs(jsonArgs));
        return formatResponse(commands.invoke(id, args));
    }
private:
    StepInfo::id_type getInvokeId(mValue &jsonArgs) {
        std::string idString(jsonArgs.get_obj()["id"].get_str());
        return fromString<StepInfo::id_type> (idString);
    }

    AbstractCommands::args_type *getInvokeArgs(mValue &jsonArgs) {
        AbstractCommands::args_type *args = new AbstractCommands::args_type;
        mArray &argsArray = jsonArgs.get_obj()["args"].get_array();
        for (mArray::const_iterator i = argsArray.begin(); i != argsArray.end(); ++i) {
            std::string arg = i->get_str();
            args->push_back(arg);
        }
        return args;
    }

    mValue formatResponse(InvokeResult result) {
        if (result.success) {
            return success_response();
        } else {
            return fail_response();
        }
    }
};

class WireProtocol : public NetworkProtocol {
private:
    CukeCommands commands;
    static std::map<std::string, boost::shared_ptr<JSONCommand> > jsonCommands;
public:
    void processStream(std::iostream &stream) {
        std::string request;
        while (stream) {
            processOneRequest(stream);
        }
    }

    void processOneRequest(std::iostream &stream) {
        mValue decodedRequestValue;
        read_stream(stream, decodedRequestValue);
        if (decodedRequestValue.is_null()) {
            return;
        }

        mValue response;
        try {
            response = invokeCommand(decodedRequestValue);
        } catch (...) {
            response = JSONCommand::fail_response();
        }
        json_spirit::write_stream(response, stream, false);

        stream << std::endl << std::flush;
    }
private:
    mValue invokeCommand(mValue &decodedRequestValue) {
        mArray decodedRequest = decodedRequestValue.get_array();
        if (decodedRequest.size() > 0) {
            std::string commandName = decodedRequest[0].get_str();
            JSONCommand *requestedCommand = jsonCommands[commandName].get();
            if (requestedCommand != 0) {
                mValue args;
                if (decodedRequest.size() > 1) {
                    args = decodedRequest[1];
                }
                return requestedCommand->run(args);
            }
        }
        return JSONCommand::fail_response();
    }
};

std::map<std::string, boost::shared_ptr<JSONCommand> > WireProtocol::jsonCommands = boost::assign::map_list_of(
        "begin_scenario", shared_ptr<JSONCommand> (new BeginScenarioCommand))("end_scenario", shared_ptr<JSONCommand> (
        new EndScenarioCommand))("step_matches", shared_ptr<JSONCommand> (new StepMatchesCommand))("invoke",
        shared_ptr<JSONCommand> (new InvokeCommand));

}
}

#endif /* CUKEBINS_WIREPROTOCOL_HPP_ */
