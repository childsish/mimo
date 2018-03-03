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
        include/IJobManager.h
        include/Step.h
        include/queues/IQueue.h
        include/queues/IJobInputs.h
        include/queues/IJobOutputs.h
        src/AsynchronousJobManager.cpp include/AsynchronousJobManager.h
        src/Job.cpp include/Job.h
        src/JobFactory.cpp include/JobFactory.h
        src/JobInputFactory.cpp include/JobInputFactory.h
        src/JobOutputFactory.cpp include/JobOutputFactory.h
        src/QueueFactory.cpp include/QueueFactory.h
        src/StepFactory.cpp include/StepFactory.h
        src/queues/Inputs.cpp include/queues/Inputs.h
        src/queues/JobInputs.cpp include/queues/JobInputs.h
        src/queues/JobOutputs.cpp include/queues/JobOutputs.h
        src/queues/Outputs.cpp include/queues/Outputs.h
        src/queues/Queue.cpp include/queues/Queue.h)
target_include_directories(mimo PRIVATE
        interfaces
        include)
target_link_libraries(mimo
        workflow)


# Add examples

if (MIMO_BUILD_TESTS)
    add_executable(linear examples/linear.cpp
            include/entities/Integer.h
            include/steps/Print.h
            include/steps/Range.h)
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
