#include <iostream> //SYNC_TCP_SERVER.CPP
#include <boost/asio.hpp>
#include <string>
#include <thread>

#include "../headers/i2cFunctions.h"
#include "../headers/data.h"

using namespace std;
using namespace boost::asio;
using ip::tcp;

const string SERVER_IP_ADDRESS = "192.168.0.120";
const int SERVER_PORT = 123;

int main() {

    std::cout << "Hello from main.cpp" << std::endl;

    io_service io;
    boost::system::error_code ec;
    char buf[128];
    tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 123);

    std::cout << "Listening at: " << ep << std::endl;
    tcp::acceptor a(io, ep);

    // Initialize i2cReading module
    I2cFunctions i2c;

    // Test i2c module
    int x = i2c.getAvailability();
    std::cout << "I2c functions availability: " << x << std::endl;

    // Initialize database module
    Data* database = new Data();

    // Test database module
    int y = database->getAvailability();
    std::cout << "Database availability: " << y << std::endl;

    // Create i2cReader thread
    // Read values in i2c line
    // Store values in database
    //std::thread i2cThread(&I2cFunctions::readLoop, &i2c);

    // Create Networking thread
    // Listen to client requests, fetch requested data and respond
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

    //i2cThread.join();

    return 0;
}
