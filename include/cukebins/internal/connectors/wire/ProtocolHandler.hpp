#ifndef CUKEBINS_PROTOCOLHANDLER_HPP_
#define CUKEBINS_PROTOCOLHANDLER_HPP_

#include <string>

namespace cukebins {
namespace internal {

/**
 * Protocol that reads one command for each input line.
 */
class ProtocolHandler {
public:
    virtual std::string handle(const std::string &request) const = 0;
    virtual ~ProtocolHandler() {};
};

}
}

#endif /* CUKEBINS_PROTOCOLHANDLER_HPP_ */
