#include <cucumber-cpp/internal/CukeEngineImpl.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireProtocol.hpp>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/scoped_ptr.hpp>

namespace {

void acceptWireProtocol(int port, const std::string& unixPath, bool verbose) {
    using namespace ::cucumber::internal;
    CukeEngineImpl cukeEngine;
    JsonSpiritWireMessageCodec wireCodec;
    WireProtocolHandler protocolHandler(wireCodec, cukeEngine);
    boost::scoped_ptr<SocketServer> server;
#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
    if (!unixPath.empty())
    {
        UnixSocketServer* const unixServer = new UnixSocketServer(&protocolHandler);
        server.reset(unixServer);
        unixServer->listen(unixPath);
        if (verbose)
            std::clog << "Listening on socket " << unixServer->listenEndpoint() << std::endl;
    }
    else
#endif
    {
        TCPSocketServer* const tcpServer = new TCPSocketServer(&protocolHandler);
        server.reset(tcpServer);
        tcpServer->listen(port);
        if (verbose)
            std::clog << "Listening on port " << tcpServer->listenEndpoint() << std::endl;
    }
    server->acceptOnce();
}

}

int main(int argc, char **argv) {
    using boost::program_options::value;
    boost::program_options::options_description optionDescription("Allowed options");
    optionDescription.add_options()
        ("help,h", "help for cucumber-cpp")
        ("verbose,v", "verbose output")
        ("port,p", value<int>(), "listening port of wireserver, use '0' (zero) to select an ephemeral port")
#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
        ("unix,u", value<std::string>(), "listening unix socket of wireserver (disables listening on port)")
#endif
        ;
    boost::program_options::variables_map optionVariableMap;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, optionDescription), optionVariableMap);
    boost::program_options::notify(optionVariableMap);

    if (optionVariableMap.count("help")) {
        std::cerr << optionDescription << std::endl;
        exit(1);
    }

    int port = 3902;
    if (optionVariableMap.count("port")) {
        port = optionVariableMap["port"].as<int>();
    }

    std::string unixPath;
#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
    if (optionVariableMap.count("unix")) {
        unixPath = optionVariableMap["unix"].as<std::string>();
    }
#endif

    bool verbose = false;
    if (optionVariableMap.count("verbose")) {
        verbose = true;
    }

    try {
        acceptWireProtocol(port, unixPath, verbose);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
