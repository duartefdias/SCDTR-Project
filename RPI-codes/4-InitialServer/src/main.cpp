#include <iostream> //SYNC_TCP_SERVER.CPP
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <chrono>

#include "../headers/i2cFunctions.h"
#include "../headers/data.h"
//#include "../headers/server.h"

using namespace std;
using namespace boost::asio;
using ip::tcp;

const string SERVER_IP_ADDRESS = "192.168.0.120";
const int SERVER_PORT = 123;
#define SLAVE_ADDR 0x0

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}


//TCP_CONNECTION
class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_service& io_service)
  {
    return pointer(new tcp_connection(io_service));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    message_ = make_daytime_string();

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

private:
  tcp_connection(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

  tcp::socket socket_;
  std::string message_;
};


// TCP SERVER CLASS
class tcp_server
{
public:
  tcp_server(boost::asio::io_service& io_service)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), 13))
  {
    start_accept();
  }

private:
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(acceptor_.io_service());

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
      start_accept();
    }
  }

  tcp::acceptor acceptor_;
};

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//void respondToClient()

void i2cFunction(I2cFunctions i2c, Data* database){
    i2c.readLoop(database);
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

    // Create Networking "thread"
    // Listen to client requests, fetch requested data and respond
    for (;;) {
        tcp::socket s(io); //create new listening socket
        a.accept(s); //wait client to connect
        
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

    i2cThread.join();

    return 0;
}
