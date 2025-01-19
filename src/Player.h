#pragma once
#include <stddef.h>
#include <string>
#include <nlohmann/json.hpp>

namespace server {

struct Player {
    std::string id;
    size_t x;
    size_t y;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Player, id, x, y)
};

}