#pragma once
#include "Player.h"
#include "Socket.h"
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>

namespace server {

class GameManager {
public:
    GameManager();
    void registerPlayer(const std::string& message);
    void listen();
    static std::string generateId();
private:
    void processMessage(const std::string& message);
    void update(const nlohmann::json& message);
    void get();
    Socket udpSocket;
    bool stopListen{false};
    std::unordered_map<std::string, Player> players;
};

}