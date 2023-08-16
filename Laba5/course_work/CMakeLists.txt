cmake_minimum_required(VERSION 3.22.1)
project(cp)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(.)
# find_package(nlohmann_json 3.11.2 REQUIRED)

add_executable(cp
        logger/logger.cpp
        logger/logger.h
        logger/logger_holder.cpp
        logger/logger_holder.h
        logger/logger_builder.cpp
        logger/logger_builder.h
        logger/logger_builder_concrete.cpp
        logger/logger_builder_concrete.h
        logger/logger_concrete.cpp
        logger/logger_concrete.h
        memory/memory_holder.h
        memory/memory_holder.cpp
        memory/memory.h
        memory/memory.cpp
        not_implemented/not_implemented.h
        allocators/list_memory.cpp
        allocators/list_memory.h
        allocators/boundary_tags_allocator.cpp
        allocators/boundary_tags_allocator.h
        allocators/memory_buddy_system.cpp
        allocators/memory_buddy_system.h
        tree/associative_container.h
        tree/binary_search_tree.h
        tree/red_black_tree.h
        database_singleton.h
        commands/command_add_collection.h
        # commands/command_add_data.h
        commands/command_add_pool.h
        commands/command_add_scheme.h
        # commands/command_get_data_between.h
        # commands/command_get_data.h
        # commands/command_remove_collection.h
        # commands/command_remove_data.h
        # commands/command_remove_pool.h
        # commands/command_remove_scheme.h
        # commands/command_update_data.h
        exception/doesnt_exist.h
        commands/command.h
        abstract_handler.h
        allocator_type.h
        comparers.h
        logger_singleton.h
        parse.h
        request_handler_with_command_chain.h
        request_handler_with_command.h
        student_definition.h
        tree_type.h
        pool.h
        scheme.h
        collection.h
        main.cpp)
target_link_libraries(cp PRIVATE nlohmann_json::nlohmann_json)