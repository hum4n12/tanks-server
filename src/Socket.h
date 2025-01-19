#pragma once
#include <netinet/in.h>
#include <future>

namespace server {

class Socket {
public:
    explicit Socket(__socket_type type, int port);
    ~Socket();
    std::future<std::string> listen();
    bool send(const std::string& data);
private:
    int socketDescriptor;
    sockaddr_in serverAddress;
    sockaddr clientAddress;
    socklen_t clientAddrLen{0};
    std::unique_ptr<std::thread> listenThread;
    std::promise<std::string> listenPromise;
};

}