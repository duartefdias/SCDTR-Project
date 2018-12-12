#include "../headers/networkFunctions.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
//#include <boost/asio.hpp>

/*using namespace boost::asio;
using ip::tcp;*/

    /*io_service& io;
    tcp::acceptor acceptor;
    void start_accept() {
        session* new_sess = new session(io);
        acceptor.async_accept(new_sess->socket(),
        boost::bind(&server::handle_accept, this, new_sess,_1));
    }
    void handle_accept(session* sess, const error_code& ec) {
        if (!ec) sess->start(); else delete sess;
            start_accept();
    }
    public:
        server(io_service& io, short port)
        : io(io), acceptor(io, tcp::endpoint(tcp::v4(), port)) {
            start_accept();
    }*/