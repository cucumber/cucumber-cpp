#ifndef CUKE_WIREPROTOCOL_HPP_
#define CUKE_WIREPROTOCOL_HPP_

#include "CukeExport.hpp"
#include "ProtocolHandler.hpp"
#include "../../CukeEngine.hpp"

#include <boost/shared_ptr.hpp>

namespace cucumber {
namespace internal {

/*
 * Response messages
 */

class WireResponseVisitor;

class CUCUMBER_CPP_EXPORT WireResponse {
public:
    WireResponse() {};

    virtual void accept(WireResponseVisitor& visitor) const = 0;

    virtual ~WireResponse() {};
};

class CUCUMBER_CPP_EXPORT SuccessResponse : public WireResponse {
public:
    void accept(WireResponseVisitor& visitor) const;
};

class CUCUMBER_CPP_EXPORT FailureResponse : public WireResponse {
private:
    const std::string message, exceptionType;

public:
    FailureResponse(const std::string & message = "", const std::string & exceptionType = "");

    const std::string getMessage() const;
    const std::string getExceptionType() const;

    void accept(WireResponseVisitor& visitor) const;
};

class CUCUMBER_CPP_EXPORT PendingResponse : public WireResponse {
private:
    const std::string message;

public:
    PendingResponse(const std::string & message);

    const std::string getMessage() const;

    void accept(WireResponseVisitor& visitor) const;
};

class CUCUMBER_CPP_EXPORT StepMatchesResponse : public WireResponse {
private:
    const std::vector<StepMatch> matchingSteps;

public:
    StepMatchesResponse(const std::vector<StepMatch> & matchingSteps);
    const std::vector<StepMatch>& getMatchingSteps() const;

    void accept(WireResponseVisitor& visitor) const;
};

class CUCUMBER_CPP_EXPORT SnippetTextResponse : public WireResponse {
private:
    const std::string stepSnippet;

public:
    SnippetTextResponse(const std::string & stepSnippet);

    const std::string getStepSnippet() const;

    void accept(WireResponseVisitor& visitor) const;
};

class CUCUMBER_CPP_EXPORT WireResponseVisitor {
public:
    virtual void visit(const SuccessResponse& response) = 0;
    virtual void visit(const FailureResponse& response) = 0;
    virtual void visit(const PendingResponse& response) = 0;
    virtual void visit(const StepMatchesResponse& response) = 0;
    virtual void visit(const SnippetTextResponse& response) = 0;

    virtual ~WireResponseVisitor() {};
};


/**
 * Wire protocol request command.
 */
class CUCUMBER_CPP_EXPORT WireCommand {
public:
    /**
     * Runs the command on the provided engine
     *
     * @param The engine
     *
     * @return The command response (ownership passed to the caller)
     */
    virtual boost::shared_ptr<WireResponse> run(CukeEngine& engine) const = 0;

    virtual ~WireCommand() {};
};

class CUCUMBER_CPP_EXPORT WireMessageCodecException : public std::exception {
private:
    const char *description;

public:
    WireMessageCodecException(const char *description) :
        description(description) {
    }

    const char* what() const throw() {
        return description;
    }
};


/**
 * Transforms wire messages into commands and responses to messages.
 */
class CUCUMBER_CPP_EXPORT WireMessageCodec {
public:
    /**
     * Decodes a wire message into a command.
     *
     * @param One single message to decode
     *
     * @return The decoded command (ownership passed to the caller)
     *
     * @throws WireMessageCodecException
     */
    virtual boost::shared_ptr<WireCommand> decode(const std::string &request) const = 0;

    /**
     * Encodes a response to wire format.
     *
     * @param Response to encode
     *
     * @return The encoded string
     */
    virtual const std::string encode(const WireResponse& response) const = 0;

    virtual ~WireMessageCodec() {};
};

/**
 * WireMessageCodec implementation with JsonSpirit.
 */
class CUCUMBER_CPP_EXPORT JsonSpiritWireMessageCodec : public WireMessageCodec {
public:
    JsonSpiritWireMessageCodec();
    boost::shared_ptr<WireCommand> decode(const std::string &request) const;
    const std::string encode(const WireResponse& response) const;
};

/**
 * Wire protocol handler, delegating JSON encoding and decoding to a
 * codec object and running commands on a provided engine instance.
 */
class CUCUMBER_CPP_EXPORT WireProtocolHandler : public ProtocolHandler {
private:
    const WireMessageCodec& codec;
    CukeEngine& engine;

public:
    WireProtocolHandler(const WireMessageCodec& codec, CukeEngine& engine);

    std::string handle(const std::string &request) const;
};

}
}

#endif /* CUKE_WIREPROTOCOL_HPP_ */
