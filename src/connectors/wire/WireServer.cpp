#include <cucumber-cpp/internal/connectors/wire/WireServer.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>

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

SocketServer::~SocketServer()
{
    removeUnixSocket();
}

void SocketServer::listen(const port_type port) {
    removeUnixSocket();

    boost::shared_ptr<tcp::acceptor> acceptor(
            boost::make_shared<tcp::acceptor>(boost::ref(ios), tcp::endpoint(tcp::v4(), port)));
    acceptor->set_option(tcp::no_delay(true));
    acceptor->listen(1);
    this->acceptor = acceptor;
}

SocketServer::port_type SocketServer::listenPort() const {
    const boost::shared_ptr<tcp::acceptor>* const acceptorPtr
        = boost::get< const boost::shared_ptr<tcp::acceptor> >(&this->acceptor);
    if (!acceptorPtr || !*acceptorPtr)
    {
        // Force an exception to get thrown if we're not listening on a TCP port.
        // Will be something like "bad file descriptor" on *nix
        io_service local_ios;
        return tcp::acceptor(local_ios).local_endpoint().port();
    }

    return (*acceptorPtr)->local_endpoint().port();
}

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
void SocketServer::listen(const std::string& unixPath) {
    removeUnixSocket();

    if (boost::filesystem::status(unixPath).type() == boost::filesystem::socket_file)
        boost::filesystem::remove(unixPath);

    boost::shared_ptr<stream_protocol::acceptor> acceptor(
            boost::make_shared<stream_protocol::acceptor>(boost::ref(ios), stream_protocol::endpoint(unixPath)));
    socketToRemove = unixPath;
    acceptor->listen(1);
    this->acceptor = acceptor;
}
#endif

void SocketServer::removeUnixSocket() {
    if (!socketToRemove.empty())
        boost::filesystem::remove(socketToRemove);
    socketToRemove.clear();
}

namespace {
    void processStream(std::iostream &stream, const ProtocolHandler *protocolHandler) {
        std::string request;
        while (getline(stream, request)) {
            stream << protocolHandler->handle(request) << std::endl << std::flush;
        }
    }

    class AcceptOnce
    {
    public:
        typedef void result_type;

        AcceptOnce(const ProtocolHandler *protocolHandler) :
            protocolHandler(protocolHandler) {
        }

        void operator()(const boost::blank&) const {}

        template <typename Protocol, typename Service>
        void operator()(const boost::shared_ptr<basic_socket_acceptor<Protocol, Service> >& acceptor) const {
            typename Protocol::iostream stream;
            acceptor->accept(*stream.rdbuf());
            processStream(stream, protocolHandler);
        }

    private:
        const ProtocolHandler *protocolHandler;
    };
}

void SocketServer::acceptOnce() {
    boost::apply_visitor(AcceptOnce(protocolHandler), acceptor);
}

}
}
