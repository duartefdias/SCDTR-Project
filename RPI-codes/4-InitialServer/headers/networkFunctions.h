#ifndef NETWORKFUNCTIONS_H
#define NETWORKFUNCTIONS_H

using namespace boost::asio;
using ip::tcp;

class Server {
public:

    Server(string IpAddress, string port);

    void startAccept();
    void handleAccept(session* sess, const error_code& ec);
    void startListener();


private:
    session* new_sess;
    
};

#endif //NETWORKFUNCTIONS_H