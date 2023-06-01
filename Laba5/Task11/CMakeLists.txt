cmake_minimum_required(VERSION 3.22.1)
project(t11)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(.)
# find_package(nlohmann_json 3.11.2 REQUIRED)

add_executable(t11
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
        ../memory/memory_holder.h
        ../memory/memory_holder.cpp
        ../memory/memory.h
        ../memory/memory.cpp
        ../task3/list_memory.cpp
        ../task3/list_memory.h
        ../task4/boundary_tags_allocator.cpp
        ../task4/boundary_tags_allocator.h
        ../task5/memory_buddy_system.cpp
        ../task5/memory_buddy_system.h
        associative_container.h
        binary_search_tree.h
        binary_search_tree.cpp
        main.cpp)
target_link_libraries(t11 PRIVATE nlohmann_json::nlohmann_json)