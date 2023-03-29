cmake_minimum_required(VERSION 3.22.1)

project(batya)
# find_package(nlohmann_json 3.2.0 REQUIRED)

CPMAddPackage(
    NAME nlohmann_json
    GITHUB_REPOSITORY nlohmann/json
    VERSION 3.9.1)

include_directories(.)
add_executable(batya
    main.cpp
    logger_builder_concrete.cpp
    logger_concrete.cpp
    logger.cpp
    logger_builder.cpp
    logger.h
    logger_builder.h
    logger_builder_concrete.h
    logger_concrete.h
)

target_link_libraries(batya PRIVATE nlohmann_json::nlohmann_json)