#include <iostream> //SYNC_TCP_SERVER.CPP
#include <boost/asio.hpp>
#include <string>

#include "../headers/i2cFunctions.h"
#include "../headers/networkFunctions.h"
#include "../headers/data.h"

const string SERVER_IP_ADDRESS = "192.168.0.120";
const string SERVER_PORT = "123";

// Initialize networking module
//Server server(SERVER_IP_ADDRESS, SERVER_PORT);

std::cout << "Hello from main.cpp" << std::endl;

// Initialize i2cReading module

// Initialize database module

// Create Networking thread
// Listen to client requests, fetch requested data and respond

// Create i2cReader thread
// Read values in i2c line
// Store values in database