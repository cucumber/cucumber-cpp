#ifndef CUKE_WIRESERVER_HPP_
#define CUKE_WIRESERVER_HPP_

#include "ProtocolHandler.hpp"

#include <string>

#include <boost/asio.hpp>

namespace cucumber {
namespace internal {

using namespace boost::asio;
using namespace boost::asio::ip;

/**
 * Socket server that calls a protocol handler line by line
 */
class SocketServer {
public:
    /**
     * Type definition for TCP port
     */
    typedef unsigned short port_type;

    /**
      * Constructor for DI
      */
    SocketServer(const ProtocolHandler *protocolHandler);

    /**
     * Bind and listen to a TCP port
     */
    void listen(const port_type port);

    /**
     * Port number that this server is currently listening on.
     *
     * @throw boost::system::system_error when not listening on any TCP port or
     *        the port cannot be determined.
     */
    port_type listenPort() const;

    /**
     * Accept one connection
     */
    void acceptOnce();

    ~SocketServer() {}; // Forbid inheritance

private:
    const ProtocolHandler *protocolHandler;
    io_service ios;
    tcp::acceptor acceptor;

    void processStream(tcp::iostream &stream);
};

}
}

#endif /* CUKE_WIRESERVER_HPP_ */
