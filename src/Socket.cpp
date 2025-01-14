#include "Socket.h"
#include <unistd.h>

namespace server{

Socket::Socket(__socket_type type, int port) {
    socketDescriptor = socket(AF_INET, type, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
    bind(socketDescriptor, (struct sockaddr*)&address, sizeof(address));
}

Socket::~Socket() {
    close(socketDescriptor);
}

std::future<std::string> Socket::listen() {
    listenThread = std::make_unique<std::thread>([this]() {
        std::string data(1024, 0);
        listenPromise.set_value_at_thread_exit(std::move(data));
        ::listen(socketDescriptor, 5);
        int clientSocket = accept(socketDescriptor, nullptr, nullptr);
        recv(clientSocket, data.data(), data.size(), 0);    
    });
    
    return listenPromise.get_future();
}
}