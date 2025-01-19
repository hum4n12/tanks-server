#include "Socket.h"

#include <iostream>
#include <thread>
#include <csignal>
#include <nlohmann/json.hpp>

std::atomic<bool> stop(false);

void signalHandler(int signal) {
    if (signal == SIGTERM) {
        std::cout << "Received SIGTERM. Exiting loop...\n";
        stop = true;
    }
}


int main() {
    using namespace std::literals::chrono_literals;
    std::signal(SIGTERM, signalHandler);
    server::Socket socket{SOCK_STREAM, 8080};
    auto future = socket.listen();

    while (!stop) {
        if (future.valid()) {
            try {
                std::string receivedData = future.get();
                std::cout << "Received: " << receivedData << std::endl;
                future = socket.listen();
            } catch (const std::future_error& e) {
                std::cerr << "Socket error: " << e.what() << "\n";
            }
        }
    }
    
    return 0;
}