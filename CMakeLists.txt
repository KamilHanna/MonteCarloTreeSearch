cmake_minimum_required(VERSION 3.20)
project(MCTS)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP)

if(OpenMP_CXX_FOUND)
    message("-- OpenMP was found")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 ${OpenMP_CXX_FLAGS}")
else()
    message("-- OpenMP not found")
    set(CMAKE_CXX_FLAGS "-O3")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(PSO_EXEC ${SOURCES})

if(OpenMP_CXX_FOUND)
    target_link_libraries(MCTS_EXEC PUBLIC OpenMP::OpenMP_CXX)
endif()