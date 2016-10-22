#ifndef CUKE_WIRESERVER_HPP_
#define CUKE_WIRESERVER_HPP_

#include "ProtocolHandler.hpp"

#include <string>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

namespace cucumber {
namespace internal {

using namespace boost::asio;
using namespace boost::asio::ip;
#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
using namespace boost::asio::local;
#endif

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

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
    /**
     * Bind and listen on a local strea
     */
    void listen(const std::string& unixPath);
#endif

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

    ~SocketServer(); // Forbid inheritance

private:
    void removeUnixSocket();

private:
    const ProtocolHandler *protocolHandler;
    io_service ios;
    boost::variant<
        boost::blank
      , boost::shared_ptr<tcp::acceptor>
#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
      , boost::shared_ptr<stream_protocol::acceptor>
#endif
      >  acceptor;
    std::string socketToRemove;
};

}
}

#endif /* CUKE_WIRESERVER_HPP_ */
