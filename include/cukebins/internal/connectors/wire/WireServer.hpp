#ifndef CUKEBINS_WIRESERVER_HPP_
#define CUKEBINS_WIRESERVER_HPP_

#include "WireProtocol.hpp"

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <string>
#include <fstream>

namespace cukebins {
namespace internal {

using std::string;
using std::iostream;

using namespace boost::asio;
using namespace boost::asio::ip;

template<class T>
class SocketServer {
    T protocol;
    tcp::endpoint endpoint;
    io_service ios;
    tcp::acceptor acceptor;

public:
    SocketServer(const short port) :
        endpoint(tcp::v4(), port), acceptor(ios) {
        acceptor.open(endpoint.protocol());
        acceptor.set_option(tcp::acceptor::reuse_address(true));
        acceptor.bind(endpoint);
        acceptor.listen(1);
    }

    void accept() {
        tcp::iostream stream;
        acceptor.accept(*stream.rdbuf());
        protocol.processStream(stream);
    }
};

}
}

#endif /* CUKEBINS_WIRESERVER_HPP_ */
