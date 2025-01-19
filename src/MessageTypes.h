#pragma once
#include <string_view>

namespace server::messages {
    static constexpr std::string_view REGISTER{"REGISTER"};
    static constexpr std::string_view UPDATE{"UPDATE"};
    static constexpr std::string_view GET{"GET"};
}