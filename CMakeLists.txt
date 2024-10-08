cmake_minimum_required(VERSION 3.10)

project(server)

add_subdirectory(fmt)

add_executable(server 
    src/main.cpp
    src/server.cpp
    src/logger.cpp
)

target_link_libraries(server fmt::fmt)
