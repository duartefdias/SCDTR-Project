#include <iostream> //SYNC_UDP_SERVER
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::udp;

int main() {
    io_service io;
    auto ep = udp::endpoint(udp::v4(),10000);

    std::cout << “Receiving at: ” << ep << std::endl;
    udp::socket s(io,ep));

    char buf[1] = {0};

    while (buf[0] != ‘q’ ) {
        
        udp::endpoint client;

        //client endpoint retrieved on receive_from
        s.receive_from(buffer(buf, 1), client);

        if( buf[0] != ‘q’)
            s.send_to(buffer(“Hello World”), client);
        else
            s.send_to(buffer(“q”), client);
    }
}