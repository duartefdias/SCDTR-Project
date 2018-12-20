#include <iostream> //SYNC_TCP_CLIENT.CPP
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

int main() {
    io_service io;
    boost::system::error_code ec;
    char buf[20000];

    tcp::resolver r(io); //find endpoints from address
    tcp::resolver::query q("127.0.0.1", "123");
    tcp::resolver::iterator server = r.resolve(q);
    tcp::socket s(io);
    
    std::cout << "Ready to send messages: " << std::endl;

    for (;;) {
        
        std::cin.getline(buf,128);
        s.connect(*server, ec); //connect and wait

        write(s,buffer(buf,strlen(buf)+1), ec);

        if(ec) break;
        size_t n = s.read_some(buffer(buf,20000), ec);
        if (ec) break;
        std::cout << "Server response: " << buf << std::endl;
        memset(buf, 0, sizeof(buf));
    }
}
