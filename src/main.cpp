#include <cucumber-cpp/internal/CukeEngineImpl.hpp>
#include <cucumber-cpp/internal/CukeExport.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireProtocol.hpp>
#include <iostream>
#include <memory>
#include <tclap/CmdLine.h>

namespace {

void acceptWireProtocol(
    const std::string& host, int port, const std::string& unixPath, bool verbose
) {
    using namespace ::cucumber::internal;
    CukeEngineImpl cukeEngine;
    JsonWireMessageCodec wireCodec;
    WireProtocolHandler protocolHandler(wireCodec, cukeEngine);
    std::unique_ptr<SocketServer> server;
#if defined(ASIO_HAS_LOCAL_SOCKETS)
    if (!unixPath.empty()) {
        UnixSocketServer* const unixServer = new UnixSocketServer(&protocolHandler);
        server.reset(unixServer);
        unixServer->listen(unixPath);
        if (verbose)
            std::clog << "Listening on socket " << unixServer->listenEndpoint() << std::endl;
    } else
#else
    // Prevent warning about unused parameter
    static_cast<void>(unixPath);
#endif
    {
        TCPSocketServer* const tcpServer = new TCPSocketServer(&protocolHandler);
        server.reset(tcpServer);
        tcpServer->listen(asio::ip::tcp::endpoint(asio::ip::address::from_string(host), port));
        if (verbose)
            std::clog << "Listening on " << tcpServer->listenEndpoint() << std::endl;
    }
    server->acceptOnce();
}

}

int CUCUMBER_CPP_EXPORT main(int argc, char** argv) {
    TCLAP::CmdLine cmd("C++ Cucumber wireserver", ' ', CUKE_VERSION);

    TCLAP::SwitchArg verboseArg("v", "verbose", "Verbose output", cmd, false);
    TCLAP::ValueArg<std::string> listenArg(
        "l", "listen", "Listening address of wireserver", false, "127.0.0.1", "string"
    );
    cmd.add(listenArg);
    TCLAP::ValueArg<int> portArg(
        "p",
        "port",
        "Listening port of wireserver, use '0' (zero) to select an ephemeral port",
        false,
        3902,
        "int"
    );
    cmd.add(portArg);

#if defined(ASIO_HAS_LOCAL_SOCKETS)
    TCLAP::ValueArg<std::string> unixArg(
        "u",
        "unix",
        "Listening unix socket of wireserver (disables listening on port)",
        false,
        "",
        "string"
    );
    cmd.add(unixArg);
#endif

    cmd.parse(argc, argv);

    std::string unixPath;
    std::string listenHost = listenArg.getValue();
    int port = portArg.getValue();
#if defined(ASIO_HAS_LOCAL_SOCKETS)
    unixPath = unixArg.getValue();
#endif

    bool verbose = verboseArg.getValue();

    try {
        acceptWireProtocol(listenHost, port, unixPath, verbose);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
