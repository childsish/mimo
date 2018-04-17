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
    include/mimo/Entity.h
    include/mimo/Step.h
    src/IFactory.h
    src/IJob.h
    src/errors.h
    src/Engine.cpp include/mimo/Engine.h
    src/Factory.h
    src/Job.cpp src/Job.h
    src/JobFactory.cpp src/JobFactory.h
    src/queues/IQueue.h
    src/queues/IQueueBundle.h
    src/job_depots/IJobDepot.h
    src/job_depots/IMultiJobDepotFactory.h
    src/job_depots/ISingleJobDepotFactory.h
    src/job_depots/AsynchronousJobDepot.cpp src/job_depots/AsynchronousJobDepot.h
    src/job_depots/JobDepot.cpp src/job_depots/JobDepot.h
    src/job_depots/MultiJobDepotFactory.cpp src/job_depots/MultiJobDepotFactory.h
    src/job_depots/SingleJobDepotFactory.cpp src/job_depots/SingleJobDepotFactory.h
    src/job_depots/SynchronousJobDepot.cpp src/job_depots/SynchronousJobDepot.h
    src/queues/Inputs.cpp include/mimo/Inputs.h
    src/queues/IQueueBundleFactory.h
    src/queues/Outputs.cpp include/mimo/Outputs.h
    src/queues/Queue.cpp src/queues/Queue.h
    src/queues/QueueBundle.cpp src/queues/QueueBundle.h
    src/queues/QueueBundleFactory.cpp src/queues/QueueBundleFactory.h
    src/queues/QueueFactory.cpp src/queues/QueueFactory.h
)
target_include_directories(mimo PRIVATE
    include)
target_link_libraries(mimo
    workflow)


# Add examples

if (MIMO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(examples)
    add_subdirectory(tests)
endif()
