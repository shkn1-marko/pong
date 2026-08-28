cmake_minimum_required(VERSION 3.15)
project(Pong)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(Pong main.cpp)

target_link_libraries(Pong PRIVATE glfw glad::glad glm::glm)