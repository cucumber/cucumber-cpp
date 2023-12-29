#ifndef CUKE_WIRESERVER_HPP_
#define CUKE_WIRESERVER_HPP_

#include <cucumber-cpp/internal/CukeExport.hpp>
#include "ProtocolHandler.hpp"

#include <string>

#include <asio.hpp>

namespace cucumber {
namespace internal {

/**
 * Socket server that calls a protocol handler line by line
 */
class CUCUMBER_CPP_EXPORT SocketServer {
public:
    /**
     * Constructor for DI
     */
    SocketServer(const ProtocolHandler* protocolHandler);
    virtual ~SocketServer() = default;

    /**
     * Accept one connection
     */
    virtual void acceptOnce() = 0;

protected:
    const ProtocolHandler* protocolHandler;
    asio::io_service ios;

    template<typename Protocol>
    void doListen(
        asio::basic_socket_acceptor<Protocol>& acceptor, const typename Protocol::endpoint& endpoint
    );
    template<typename Protocol>
    void doAcceptOnce(asio::basic_socket_acceptor<Protocol>& acceptor);
    void processStream(std::iostream& stream);
};

/**
 * Socket server that calls a protocol handler line by line
 */
class CUCUMBER_CPP_EXPORT TCPSocketServer : public SocketServer {
public:
    /**
     * Type definition for TCP port
     */
    typedef unsigned short port_type;

    /**
     * Constructor for DI
     */
    TCPSocketServer(const ProtocolHandler* protocolHandler);

    /**
     * Bind and listen to a TCP port
     */
    void listen(const port_type port);

    /**
     * Bind and listen to a TCP port on the given endpoint
     */
    void listen(const asio::ip::tcp::endpoint endpoint);

    /**
     * Endpoint (IP address and port number) that this server is currently
     * listening on.
     *
     * @throw std::system_error when not listening on any socket or
     *        the endpoint cannot be determined.
     */
    asio::ip::tcp::endpoint listenEndpoint() const;

    void acceptOnce() override;

private:
    asio::ip::tcp::acceptor acceptor;
};

#if defined(ASIO_HAS_LOCAL_SOCKETS)
/**
 * Socket server that calls a protocol handler line by line
 */
class CUCUMBER_CPP_EXPORT UnixSocketServer : public SocketServer {
public:
    /**
     * Constructor for DI
     */
    UnixSocketServer(const ProtocolHandler* protocolHandler);

    /**
     * Bind and listen on a local stream socket
     */
    void listen(const std::string& unixPath);

    /**
     * Port number that this server is currently listening on.
     *
     * @throw std::system_error when not listening on any socket or
     *        the endpoint cannot be determined.
     */
    asio::local::stream_protocol::endpoint listenEndpoint() const;

    void acceptOnce() override;

    ~UnixSocketServer() override;

private:
    asio::local::stream_protocol::acceptor acceptor;
};
#endif

}
}

#endif /* CUKE_WIRESERVER_HPP_ */
