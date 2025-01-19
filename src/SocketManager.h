#pragma once
#include <netinet/in.h>

class SocketManager {
public:
    void init();
    int getTcpSocket();
    int getUdpSocket();

private:
    sockaddr_in serverAddress;
};