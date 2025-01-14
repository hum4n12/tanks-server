#pragma once
#include <netinet/in.h>
#include <future>

namespace server{

class Socket {
public:
    explicit Socket(__socket_type type, int port);
    ~Socket();
    std::future<std::string> listen();

private:
    int socketDescriptor;
    sockaddr_in address;
    std::unique_ptr<std::thread> listenThread;
    std::promise<std::string> listenPromise;
};

}