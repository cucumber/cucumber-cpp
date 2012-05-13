#ifndef WIRESERVER_HPP_
#define WIRESERVER_HPP_

#include "cukebins.hpp"
#include <cukebins/internal/CukeEngineImpl.hpp>
#include <cukebins/internal/connectors/wire/WireServer.hpp>
#include <cukebins/internal/connectors/wire/WireProtocol.hpp>
#include <iostream>

namespace cukebins {

void acceptWireProtocol(int port) {
    using namespace internal;
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
            port = ::cukebins::internal::fromString<int>(firstArg);
        }
        std::clog << "Listening on port " << port << std::endl;
        ::cukebins::acceptWireProtocol(port);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    return 0;
}

#endif /* WIRESERVER_HPP_ */
