cmake_minimum_required(VERSION 3.22.1)
project(t2)

set(CMAKE_CXX_STANDARD 17)
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL] [SYSTEM])

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(.)
# find_package(nlohmann_json 3.11.2 REQUIRED)

add_executable(t3
        ../logger/log2/logger.cpp
        ../logger/log2/logger.h
        ../logger/log2/logger_holder.cpp
        ../logger/log2/logger_holder.h
        ../logger/log2/logger_builder.cpp
        ../logger/log2/logger_builder.h
        ../logger/log2/logger_builder_concrete.cpp
        ../logger/log2/logger_builder_concrete.h
        ../logger/log2/logger_concrete.cpp
        ../logger/log2/logger_concrete.h
        ../memory/memory.h
        ../memory/memory.cpp
        ../not_implemented/not_implemented.h
        list_memory.cpp
        list_memory.h
        main.cpp)
target_link_libraries(t3 PRIVATE nlohmann_json::nlohmann_json)