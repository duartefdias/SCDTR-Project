#include "../headers/networkFunctions.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

void Server::start_accept(io_service& io) {
    new_sess = new session(io);
    acceptor.async_accept(new_sess->socket(),
    boost::bind(&server::handle_accept, this, new_sess,_1));
}

void Server::handle_accept(session* sess, const error_code& ec) {
    if (!ec) sess->start(); else delete sess;
        start_accept();
}

    server(io_service& io, short port)
    : io(io), acceptor(io, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
}