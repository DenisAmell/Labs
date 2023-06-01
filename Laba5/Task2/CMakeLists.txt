cmake_minimum_required(VERSION 3.22.1)
project(t2)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(.)
# find_package(nlohmann_json 3.11.2 REQUIRED)

add_executable(t2
        ../logger/log2/logger.cpp
        ../logger/log2/logger.h
        ../logger/log2/logger_builder.cpp
        ../logger/log2/logger_builder.h
        ../logger/log2/logger_builder_concrete.cpp
        ../logger/log2/logger_builder_concrete.h
        ../logger/log2/logger_concrete.cpp
        ../logger/log2/logger_concrete.h
        allocator.cpp
        allocator.h
        memory_denis.h
        main.cpp)
target_link_libraries(t2 PRIVATE nlohmann_json::nlohmann_json)