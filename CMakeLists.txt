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
    include/mimo/IInputs.h
    include/mimo/IOutputs.h
    include/mimo/Entity.h
    include/mimo/Step.h
    src/Factory.h
    src/IFactory.h
    src/IForwardIterator.h
    src/IJob.h
    src/Job.cpp src/Job.h
    src/JobComparator.cpp src/JobComparator.h
    src/JobFactory.h
    src/depots/IJobDepot.h
    src/depots/IJobDepotFactory.h
    src/depots/IMultiJobDepot.h
    src/depots/ISingleJobDepot.h
    src/depots/MultiJobDepot.cpp src/depots/MultiJobDepot.h
    src/depots/MultiJobDepotFactory.h
    src/depots/SingleJobDepot.cpp src/depots/SingleJobDepot.h
    src/depots/SingleJobDepotFactory.h
    src/queues/IQueue.h
    src/queues/IQueueBundle.h
    src/queues/Queue.cpp src/queues/Queue.h
    src/queues/QueueFactory.h
    src/queues/QueueBundle.cpp src/queues/QueueBundle.h
    src/queues/QueueBundleFactory.h
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
