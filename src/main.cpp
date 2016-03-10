#include <cucumber-cpp/internal/CukeEngineImpl.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireProtocol.hpp>
#include <iostream>
#include <boost/program_options.hpp>

namespace {

void acceptWireProtocol(int port, bool verbose) {
    using namespace ::cucumber::internal;
    CukeEngineImpl cukeEngine;
    JsonSpiritWireMessageCodec wireCodec;
    WireProtocolHandler protocolHandler(&wireCodec, &cukeEngine);
    SocketServer server(&protocolHandler);
    server.listen(port);
    if (verbose)
        std::clog << "Listening on port " << port << std::endl;
    server.acceptOnce();
}

}

int main(int argc, char **argv) {
    using boost::program_options::value;
    boost::program_options::options_description optionDescription("Allowed options");
    optionDescription.add_options()
        ("help,h", "help for cucumber-cpp")
        ("verbose,v", "verbose output")
        ("port,p", value<int>(), "listening port of wireserver")
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

    bool verbose = false;
    if (optionVariableMap.count("verbose")) {
        verbose = true;
    }

    try {
        acceptWireProtocol(port, verbose);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
