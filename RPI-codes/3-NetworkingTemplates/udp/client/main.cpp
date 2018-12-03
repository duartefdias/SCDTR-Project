#include <iostream> //SYNC_UDP_CLIENT
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::udp;
using query = udp::resolver::query;

int main() {
    io_service io;

    udp::resolver r(io);
    query q(udp::v4(),"127.0.0.1","123");
    udp::endpoint ep = *r.resolve(q);
    udp::socket s(io);

    s.open(udp::v4());

    char buf[128] = {0};
    
    std::cout << "Endpoint: " << ep << std::endl;
    std::cout << "Initiating while loop" << std::endl;

    do {

        std::cin.getline(buf,128);
        
        s.send_to(buffer(buf,1), ep); //send 1st char

        size_t n = s.receive_from(buffer(buf, 128),ep);

        std::cout.write(buf, n-1);
    } while( buf[0] != 'q' );
}
