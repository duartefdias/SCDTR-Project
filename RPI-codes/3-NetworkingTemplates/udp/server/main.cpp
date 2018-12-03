#include <iostream> //SYNC_UDP_SERVER
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::udp;

int main() {
	
	io_service io;
	
	// Define endpoint address and port
    udp::endpoint ep(ip::address::from_string("127.0.0.1"), 123);

	// Check if it is working
    std::cout << "Receiving at: " << ep << std::endl;
    udp::socket s(io,ep);

	// Initiate buffer to receive data
    char buf[1] = {0};

	// Receive and print data until 'q' is received
    while (buf[0] != 'q' ) {
        
        udp::endpoint client;

        //client endpoint retrieved on receive_from
        s.receive_from(buffer(buf, 1), client);
        
        std::cout << "From " << client << ": " << buf[0] << std::endl;

        if( buf[0] != 'q')
            s.send_to(buffer("Hello World"), client);
        else
            s.send_to(buffer("q"), client);
    }
}
