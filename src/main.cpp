#include "Socket.h"

#include <iostream>
#include <thread>
#include <csignal>
#include <nlohmann/json.hpp>

std::atomic<bool> stop(false);

// Funkcja obsługująca sygnał SIGTERM
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
         future.wait_for(100ms);

    }
    
    return 0;
}