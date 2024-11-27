cmake_minimum_required(VERSION 3.20)
project(MCTS)

set(CMAKE_CXX_STANDARD 20)

# OpenMP
find_package(OpenMP)

if(OpenMP_CXX_FOUND)
    message("-- OpenMP was found")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 ${OpenMP_CXX_FLAGS}")
else()
    message("-- OpenMP not found")
    set(CMAKE_CXX_FLAGS "-O3")
endif()

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

# MPI
find_package(MPI REQUIRED)

if(MPI_CXX_FOUND)
    message("-- MPI was found")
    include_directories(${MPI_INCLUDE_PATH})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MPI_CXX_COMPILE_FLAGS}")
else()
    message("-- MPI not found")
endif()

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Add executable
add_executable(MCTS_EXEC ${SOURCES})

# Link libraries for OpenMP
if(OpenMP_CXX_FOUND)
    target_link_libraries(MCTS_EXEC PUBLIC OpenMP::OpenMP_CXX)
endif()

# Link libraries for MPI
if(MPI_CXX_FOUND)
    target_link_libraries(MCTS_EXEC PUBLIC ${MPI_CXX_LIBRARIES})
endif()
