#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>
#include <filesystem>

namespace cucumber {
namespace internal {

SocketServer::SocketServer(const ProtocolHandler* protocolHandler) :
    protocolHandler(protocolHandler),
    ios() {
}

template<typename Protocol>
void SocketServer::doListen(
    asio::basic_socket_acceptor<Protocol>& acceptor, const typename Protocol::endpoint& endpoint
) {
    if (acceptor.is_open())
        throw std::system_error(asio::error::already_open);
    acceptor.open(endpoint.protocol());
    acceptor.set_option(typename Protocol::acceptor::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen(1);
}

template<typename Protocol>
void SocketServer::doAcceptOnce(asio::basic_socket_acceptor<Protocol>& acceptor) {
    typename Protocol::iostream stream;
    acceptor.accept(*stream.rdbuf());
    processStream(stream);
}

void SocketServer::processStream(std::iostream& stream) {
    std::string request;
    while (getline(stream, request)) {
        stream << protocolHandler->handle(request) << std::endl << std::flush;
    }
}

TCPSocketServer::TCPSocketServer(const ProtocolHandler* protocolHandler) :
    SocketServer(protocolHandler),
    acceptor(ios) {
}

void TCPSocketServer::listen(const port_type port) {
    listen(asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
}

void TCPSocketServer::listen(const asio::ip::tcp::endpoint endpoint) {
    doListen(acceptor, endpoint);
    acceptor.set_option(asio::ip::tcp::no_delay(true));
}

asio::ip::tcp::endpoint TCPSocketServer::listenEndpoint() const {
    return acceptor.local_endpoint();
}

void TCPSocketServer::acceptOnce() {
    doAcceptOnce(acceptor);
}

#if defined(ASIO_HAS_LOCAL_SOCKETS)
UnixSocketServer::UnixSocketServer(const ProtocolHandler* protocolHandler) :
    SocketServer(protocolHandler),
    acceptor(ios) {
}

void UnixSocketServer::listen(const std::string& unixPath) {
    if (std::filesystem::status(unixPath).type() == std::filesystem::file_type::socket)
        std::filesystem::remove(unixPath);

    doListen(acceptor, asio::local::stream_protocol::endpoint(unixPath));
}

asio::local::stream_protocol::endpoint UnixSocketServer::listenEndpoint() const {
    return acceptor.local_endpoint();
}

void UnixSocketServer::acceptOnce() {
    doAcceptOnce(acceptor);
}

UnixSocketServer::~UnixSocketServer() {
    if (!acceptor.is_open())
        return;
    std::string path = acceptor.local_endpoint().path();
    // NOTE: this will fail if this path got deleted manually or represents an abstract-namespace
    // socket
    std::filesystem::remove(path);
}
#endif

}
}
