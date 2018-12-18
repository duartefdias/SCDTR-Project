#include <iostream> //SYNC_TCP_SERVER.CPP
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <chrono>

#include "../headers/i2cFunctions.h"
#include "../headers/data.h"

using namespace std;
using namespace boost::asio;
using ip::tcp;

const string SERVER_IP_ADDRESS = "192.168.0.120";
const int SERVER_PORT = 123;
#define SLAVE_ADDR 0x0

//void respondToClient()

void i2cFunction(I2cFunctions i2c, Data* database){
    i2c.readLoop(database);
}

void connectToClient(tcp::socket s){
    for(;;) { //got a client
            int n = s.read_some(buffer(buf,128), ec);
            if(ec) break;
            std::cout << "Received message: " << buf << std::endl;
            //cout << "Buffer message sent to client: " << database.processRequest(buf) << endl;
            write(s, buffer(database.processRequest(buf)), ec);
            //write(s, buffer("                                    "), ec);
            //write(s, buffer("\n",n), ec);
            //write(s, buffer(buf,n), ec);
            if(ec) break;
        } //kills connection
}

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
    std::cout << "I2c functions availability: " << i2c.getAvailability() << std::endl;

    // Initialize database module
    Data database (2);

    // Test database module
    std::cout << "Database availability: " << database.getAvailability() << std::endl;

    // Create i2cReader thread
    // Read values in i2c line
    // Store values in database
    std::thread i2cThread(i2cFunction, i2c, &database);

    // Create Networking threads
    // Listen to client requests, fetch requested data and respond
    for (;;) {
        // Create new listening socket
        tcp::socket s(io);

        // Wait for a client to connect
        a.accept(s);
        
        // Create new socket in new thread to allow more clients to connect
        std::thread clientResponseThread(connectToClient, s);
    }

    clientResponseThread.join();
    i2cThread.join();

    return 0;
}
