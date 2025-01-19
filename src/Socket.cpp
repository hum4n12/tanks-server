#include "Socket.h"
#include <unistd.h>
#include <iostream>

namespace server{

Socket::Socket(__socket_type type, int port) {
    socketDescriptor = socket(AF_INET, type, 0);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    bind(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
}

Socket::~Socket() {
    close(socketDescriptor);
}

std::future<std::string> Socket::listen() {

    listenPromise = std::promise<std::string>();

    if (!listenThread) {
        listenThread = std::make_unique<std::thread>([this]() {
        try {
            while (true) {
                std::string data(1024, 0);
                ::listen(socketDescriptor, 5);
                int clientSocket = accept(socketDescriptor, nullptr, nullptr);
                size_t dataReceivedSize = recvfrom(clientSocket, data.data(), data.size(), 0, &clientAddress, &clientAddrLen);

                if (dataReceivedSize < 0) {
                    clientAddrLen = 0;
                    std::cerr << "Error while reading data: " << dataReceivedSize << "\n";
                }

                data.resize(dataReceivedSize);
                listenPromise.set_value(std::move(data));
                close(clientSocket);
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in thread: " << e.what() << std::endl;
            listenPromise.set_value("Error occurred");
        }
        });
    }
    
    return listenPromise.get_future();
}
bool Socket::send(const std::string& data) {
    if (clientAddrLen == 0) {
        std::cerr << "Client socket unknown \n";
        return false;
    }

    size_t result = sendto(socketDescriptor, data.data(), data.size(), 0, &clientAddress, clientAddrLen);
    if(result < 0) {
        std::cerr << "Could not sent data: " << result << "\n";
        return false;
    }
    
    return true;
}
}