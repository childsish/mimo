cmake_minimum_required(VERSION 3.7)
project(mimo)
set(CMAKE_CXX_STANDARD 17)


# Options

option(MIMO_BUILD_TESTS "Enable/Disable unit tests for the mimo library." on)


# Add workflow library

set(WORKFLOW_BUILD_TESTS off CACHE BOOL "Disable workflow library unit tests.")
add_subdirectory(lib/workflow)


# Add mimo library

add_library(mimo
    include/mimo/Entity.h
    src/IForwardIterator.h
    src/queues/IQueue.h
    src/queues/IQueueBundle.h
    src/queues/Queue.cpp src/queues/Queue.h
    src/queues/QueueBundle.cpp src/queues/QueueBundle.h
)
target_include_directories(mimo PRIVATE
    src
    include)
target_link_libraries(mimo
    workflow
)


# Add examples

if (MIMO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(examples)
    add_subdirectory(tests)
endif()
