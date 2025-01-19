#include "GameManager.h"
#include "MessageTypes.h"
#include <nlohmann/json.hpp>
#include <iostream>

namespace server {
using json = nlohmann::json;

GameManager::GameManager() : udpSocket(SOCK_DGRAM, 8081) {
    
}

void GameManager::registerPlayer(const std::string &message) {

    try {
        json parsedPlayerData = json::parse(message);
        Player newPlayer;
        std::string id {generateId()};
        newPlayer.id = id;
        newPlayer.x = parsedPlayerData["x"];
        newPlayer.y = parsedPlayerData["y"];
        players.emplace(std::move(id), newPlayer);
    } catch (const json::parse_error& e) { 
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
    }
}

void GameManager::listen() {
    auto future = udpSocket.listen();
    while (!stopListen) {
        future.wait();
        auto data = future.get();
        processMessage(data);
    }
}

std::string GameManager::generateId() {
    static int id = 0;
    return std::to_string(++id);
}

void GameManager::processMessage(const std::string &message) {   
    try {
        json parsedMessage = json::parse(message);
        const std::string type{parsedMessage["type"]};

        if (type == messages::UPDATE) {
            update(parsedMessage);
        } else if (type == messages::GET) {
            get();
        }
        
    } catch (const json::parse_error& e) { 
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
    }
}

void GameManager::update(const json& message)  {
    const std::string playerId = message["id"];
    if (!players.contains(playerId)) {
        return;
    }

    Player& player = players[playerId];
    player.x = message["x"];
    player.y = message["y"];
}

void GameManager::get() {
    json data = players;
    udpSocket.send(data.dump());
}

}