file(GLOB SRC_FILES "*.cpp" "*.h")

add_library(src_lib ${SRC_FILES})

target_include_directories(src_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(src_lib nlohmann_json::nlohmann_json)