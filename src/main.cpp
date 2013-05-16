#include <cucumber-cpp/internal/CukeEngineImpl.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireProtocol.hpp>
#include <iostream>

namespace {

void acceptWireProtocol(int port) {
    using namespace ::cucumber::internal;
    CukeEngineImpl cukeEngine;
    JsonSpiritWireMessageCodec wireCodec;
    WireProtocolHandler protocolHandler(&wireCodec, &cukeEngine);
    SocketServer server(&protocolHandler);
    server.listen(port);
    server.acceptOnce();
}

}

int main(int argc, char **argv) {
    try {
        int port = 3902;
        if (argc > 1) {
            std::string firstArg(argv[1]);
            port = ::cucumber::internal::fromString<int>(firstArg);
        }
        acceptWireProtocol(port);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
