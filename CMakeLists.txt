cmake_minimum_required(VERSION 3.7)
project(mimo)
set(CMAKE_CXX_STANDARD 14)


# Options

option(MIMO_BUILD_TESTS "Enable/Disable unit tests for the mimo library." on)


# Add workflow library

set(WORKFLOW_BUILD_TESTS off CACHE BOOL "Disable workflow library unit tests.")
add_subdirectory(lib/workflow)


# Add mimo library

add_library(mimo
        include/Entity.h
        include/Step.h
        include/queues/InputQueue.h
        include/queues/OutputQueue.h
        src/queues/Queue.cpp include/queues/Queue.h
        src/queues/QueueChannel.cpp include/queues/QueueChannel.h)
target_include_directories(mimo PRIVATE
        include)
target_link_libraries(mimo
        workflow)


# Add examples

if (MIMO_BUILD_TESTS)
    add_executable(linear examples/linear.cpp
            include/entities/Integer.h)
    target_include_directories(linear PRIVATE
            include)
    target_link_libraries(linear
            mimo
            workflow)

    add_executable(join examples/join.cpp)
    target_include_directories(join PRIVATE
            include)
    target_link_libraries(join
            mimo
            workflow)

    add_executable(fork examples/fork.cpp)
    target_include_directories(fork PRIVATE
            include)
    target_link_libraries(fork
            mimo
            workflow)

    add_executable(split examples/split.cpp)
    target_include_directories(split PRIVATE
            include)
    target_link_libraries(split
            mimo
            workflow)

    enable_testing()
    add_subdirectory(tests)
endif()
