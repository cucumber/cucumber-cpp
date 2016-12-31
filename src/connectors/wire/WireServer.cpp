#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>
#include <boost/filesystem/operations.hpp>

namespace cucumber {
namespace internal {

using namespace boost::asio::ip;
#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
using namespace boost::asio::local;
#endif

SocketServer::SocketServer(const ProtocolHandler *protocolHandler) :
    protocolHandler(protocolHandler),
    ios() {
}

template <typename Protocol, typename Service>
void SocketServer::doListen(basic_socket_acceptor<Protocol, Service>& acceptor,
        const typename Protocol::endpoint& endpoint) {
    if (acceptor.is_open())
        throw boost::system::system_error(boost::asio::error::already_open);
    acceptor.open(endpoint.protocol());
    acceptor.set_option(typename Protocol::acceptor::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen(1);
}

template <typename Protocol, typename Service>
void SocketServer::doAcceptOnce(basic_socket_acceptor<Protocol, Service>& acceptor) {
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

TCPSocketServer::TCPSocketServer(const ProtocolHandler *protocolHandler) :
    SocketServer(protocolHandler),
    acceptor(ios) {
}

void TCPSocketServer::listen(const port_type port) {
    doListen(acceptor, tcp::endpoint(tcp::v4(), port));
    acceptor.set_option(tcp::no_delay(true));
}

tcp::endpoint TCPSocketServer::listenEndpoint() const {
    return acceptor.local_endpoint();
}

void TCPSocketServer::acceptOnce() {
    doAcceptOnce(acceptor);
}

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
UnixSocketServer::UnixSocketServer(const ProtocolHandler *protocolHandler) :
    SocketServer(protocolHandler),
    acceptor(ios) {
}

void UnixSocketServer::listen(const std::string& unixPath) {
    if (boost::filesystem::status(unixPath).type() == boost::filesystem::socket_file)
        boost::filesystem::remove(unixPath);

    doListen(acceptor, stream_protocol::endpoint(unixPath));
}

stream_protocol::endpoint UnixSocketServer::listenEndpoint() const {
    return acceptor.local_endpoint();
}

void UnixSocketServer::acceptOnce() {
    doAcceptOnce(acceptor);
}

UnixSocketServer::~UnixSocketServer() {
    if (!acceptor.is_open())
        return;
    std::string path = acceptor.local_endpoint().path();
    // NOTE: this will fail if this path got deleted manually or represents an abstract-namespace socket
    boost::filesystem::remove(path);
}
#endif

}
}
