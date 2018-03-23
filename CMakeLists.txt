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
    include/errors.h
    include/Entity.h
    include/Factory.h
    include/IFactory.h
    include/IJob.h
    include/Step.h
    include/job_depots/IJobDepot.h
    include/job_depots/IMultiJobDepotFactory.h
    include/job_depots/ISingleJobDepotFactory.h
    include/queues/IQueue.h
    include/queues/IJobInputs.h
    include/queues/IJobOutputs.h
    src/Engine.cpp include/Engine.h
    src/Job.cpp include/Job.h
    src/JobFactory.cpp include/JobFactory.h
    src/queues/JobInputFactory.cpp include/queues/JobInputFactory.h
    src/queues/JobOutputFactory.cpp include/queues/JobOutputFactory.h
    src/QueueFactory.cpp include/QueueFactory.h
    src/job_depots/AsynchronousJobDepot.cpp include/job_depots/AsynchronousJobDepot.h
    src/job_depots/JobDepot.cpp include/job_depots/JobDepot.h
    src/job_depots/MultiJobDepotFactory.cpp include/job_depots/MultiJobDepotFactory.h
    src/job_depots/SingleJobDepotFactory.cpp include/job_depots/SingleJobDepotFactory.h
    src/job_depots/SynchronousJobDepot.cpp include/job_depots/SynchronousJobDepot.h
    src/queues/Inputs.cpp include/queues/Inputs.h
    src/queues/JobInputs.cpp include/queues/JobInputs.h
    src/queues/JobOutputs.cpp include/queues/JobOutputs.h
    src/queues/Outputs.cpp include/queues/Outputs.h
    src/queues/Queue.cpp include/queues/Queue.h)
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
