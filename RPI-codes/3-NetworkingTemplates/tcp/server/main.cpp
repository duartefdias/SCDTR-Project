#include <iostream> //SYNC_TCP_SERVER.CPP
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

int main() {
    io_service io;
    boost::system::error_code ec;
    char buf[128];
    tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 123);

    std::cout << "Listening at: " << ep << std::endl;
    tcp::acceptor a(io, ep);

    for (;;) {
        tcp::socket s(io); //create new listening socket
        a.accept(s); //wait client to connect
        
        for(;;) { //got a client
            size_t n = s.read_some(buffer(buf,128), ec);
            if(ec) break;
            std::cout << "Received message: " << buf << std::endl;
            write(s, buffer(buf,n), ec);
            if(ec) break;
        } //kills connection
    }
}
