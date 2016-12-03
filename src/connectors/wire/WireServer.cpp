#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>

namespace cucumber {
namespace internal {

SocketServer::SocketServer(const ProtocolHandler *protocolHandler) :
    protocolHandler(protocolHandler),
    ios(),
    acceptor(ios) {
}

void SocketServer::listen(const port_type port) {
    tcp::endpoint endpoint(tcp::v4(), port);
    acceptor.open(endpoint.protocol());
    acceptor.set_option(tcp::acceptor::reuse_address(true));
    acceptor.set_option(tcp::no_delay(true));
    acceptor.bind(endpoint);
    acceptor.listen(1);
}

SocketServer::port_type SocketServer::listenPort() const {
  const tcp::endpoint ep(acceptor.local_endpoint());
  return ep.port();
}

void SocketServer::acceptOnce() {
    tcp::iostream stream;
    acceptor.accept(*stream.rdbuf());
    processStream(stream);
}

void SocketServer::processStream(tcp::iostream &stream) {
    std::string request;
    while (getline(stream, request)) {
        stream << protocolHandler->handle(request) << std::endl << std::flush;
    }
}

}
}
