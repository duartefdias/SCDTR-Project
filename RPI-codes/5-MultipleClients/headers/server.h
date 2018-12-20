#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>


#include "../headers/data.h"

using namespace boost::asio;
using boost::system::error_code;

class session {
    ip::tcp::socket s;
    enum { max_len = 1024 };
    char data[max_len];
    Data* db;

    // Sends response to client
    void hread(const error_code& ec, size_t sz) {
        if (!ec){
            async_write(s, buffer(db->processRequest(data), sz), boost::bind(&session::hwrite, this, _1));
        }
        else delete this;
    }

    // Happens after the write
    void hwrite(const error_code& ec) {
        if (!ec) {
            start();
        }
        else delete this; 
    }

    public:
    session(io_service& io, Data* database) : s(io) {db = database;}
    ip::tcp::socket& socket() {return s;}

    void start() {
        s.async_read_some(buffer(data,max_len), boost::bind(&session::hread, this, _1, _2));
    }
};

class server {
    io_service& io;
    ip::tcp::acceptor acc;
    Data* db;

    void start_accept(Data* database) {
        session* new_sess = new session(io, database);
        acc.async_accept(new_sess->socket(), boost::bind(&server::haccept, this, new_sess,_1)); 
    }

    void haccept(session* sess, const error_code& ec) {
        if (!ec) sess->start();
        else delete sess;
        start_accept(database); 
    }

    public:
    server(io_service& io, short port, Data* database)
    : io(io), acc(io, ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), port)) {
        db = database;
        start_accept(db); 
    }
};