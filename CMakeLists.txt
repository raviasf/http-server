cmake_minimum_required(VERSION 3.10)

project(CppHttpServer)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include header directory
include_directories(include)

# Create executable
add_executable(server
    src/main.cpp
    src/server.cpp
)

# Find and link threads
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)
