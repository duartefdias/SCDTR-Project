#include <iostream> //SYNC_TCP_SERVER.CPP
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <chrono>

#include "../headers/i2cFunctions.h"
#include "../headers/data.h"
#include "../headers/server.h"

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

int main() {

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

    // Create Networking threadss
    // Listen to client requests, fetch requested data and respond
    io_service io;
    server s(io, 123, &database);
    io.run();

    //clientResponseThread.join();
    i2cThread.join();

    return 0;
}
