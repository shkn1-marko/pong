cmake_minimum_required(VERSION 3.15)
project(Pong)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glad CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)

    add_executable(PongClient
        client/main.cpp
        client/game.cpp
    )

    target_include_directories(PongClient PRIVATE shared)
    target_link_libraries(PongClient PRIVATE glfw glad::glad glm::glm)
    target_compile_definitions(PongClient PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")
endif()

if (UNIX)
    add_executable(PongServer
        server/main.cpp
    )

    target_include_directories(PongServer PRIVATE shared)
endif()