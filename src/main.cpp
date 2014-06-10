#include <cucumber-cpp/internal/CukeEngineImpl.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>
#include <cucumber-cpp/internal/connectors/wire/WireProtocol.hpp>
#include <iostream>
#include <boost/program_options.hpp>
using boost::program_options::value;

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
    
    boost::program_options::options_description optionDescription("Allowed options");
    optionDescription.add_options()
        ("help", "this help message")
        ("port", value<int>(), "listening port of wireserver")
        ;
    boost::program_options::variables_map optionVariableMap;
    boost::program_options::store( boost::program_options::parse_command_line(argc, argv, optionDescription), optionVariableMap);
    boost::program_options::notify(optionVariableMap); 

    if (optionVariableMap.count("help")) {
        std::cout << optionDescription << std::endl;
    }

    int port = 3902;
    if (optionVariableMap.count("port")) {
       port = optionVariableMap["port"].as<int>();
    }

    try {
        acceptWireProtocol(port);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
