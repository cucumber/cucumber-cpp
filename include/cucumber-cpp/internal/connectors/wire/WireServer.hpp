#ifndef CUKE_WIRESERVER_HPP_
#define CUKE_WIRESERVER_HPP_

#include "ProtocolHandler.hpp"

#include <string>

#include <boost/asio.hpp>

namespace cucumber {
namespace internal {

/**
 * Socket server that calls a protocol handler line by line
 */
class SocketServer {
public:
    /**
      * Constructor for DI
      */
    SocketServer(const ProtocolHandler *protocolHandler);

    /**
     * Accept one connection
     */
    virtual void acceptOnce() = 0;

protected:
    const ProtocolHandler *protocolHandler;
    boost::asio::io_service ios;

    template <typename Protocol, typename Service>
    void doListen(boost::asio::basic_socket_acceptor<Protocol, Service>& acceptor,
            const typename Protocol::endpoint& endpoint);
    template <typename Protocol, typename Service>
    void doAcceptOnce(boost::asio::basic_socket_acceptor<Protocol, Service>& acceptor);
    void processStream(std::iostream &stream);
};

/**
 * Socket server that calls a protocol handler line by line
 */
class TCPSocketServer : public SocketServer {
public:
    /**
     * Type definition for TCP port
     */
    typedef unsigned short port_type;

    /**
      * Constructor for DI
      */
    TCPSocketServer(const ProtocolHandler *protocolHandler);

    /**
     * Bind and listen to a TCP port
     */
    void listen(const port_type port);

    /**
     * Bind and listen to a TCP port on the given endpoint
     */
    void listen(const boost::asio::ip::tcp::endpoint endpoint);

    /**
     * Endpoint (IP address and port number) that this server is currently
     * listening on.
     *
     * @throw boost::system::system_error when not listening on any socket or
     *        the endpoint cannot be determined.
     */
    boost::asio::ip::tcp::endpoint listenEndpoint() const;

    virtual void acceptOnce();

private:
    boost::asio::ip::tcp::acceptor acceptor;
};

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
/**
 * Socket server that calls a protocol handler line by line
 */
class UnixSocketServer : public SocketServer {
public:
    /**
      * Constructor for DI
      */
    UnixSocketServer(const ProtocolHandler *protocolHandler);

    /**
     * Bind and listen on a local stream socket
     */
    void listen(const std::string& unixPath);

    /**
     * Port number that this server is currently listening on.
     *
     * @throw boost::system::system_error when not listening on any socket or
     *        the endpoint cannot be determined.
     */
    boost::asio::local::stream_protocol::endpoint listenEndpoint() const;

    virtual void acceptOnce();

    ~UnixSocketServer();

private:
    boost::asio::local::stream_protocol::acceptor acceptor;
};
#endif

}
}

#endif /* CUKE_WIRESERVER_HPP_ */
