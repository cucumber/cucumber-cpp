#ifndef WIRESERVER_HPP_
#define WIRESERVER_HPP_

#include "cukebins.hpp"
#include "internal/connectors/wire/WireServer.hpp"
#include <iostream>

namespace cukebins {

void acceptWireProtocol(int port) {
    cukebins::internal::SocketServer<cukebins::internal::WireProtocol> server(port);
    server.accept();
}
}

int main(int argc, char** argv) {
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
