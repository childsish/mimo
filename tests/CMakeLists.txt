cmake_minimum_required(VERSION 3.7)
project(test_graph)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(googletest)


add_executable(test_mimo
    ../include/Entity.h
    ../include/entities/Integer.h
    test_Job.cpp
    test_JobFactory.cpp
    test_JobInputs.cpp
    test_JobOutputs.cpp
    test_Queue.cpp
    test_StepFactory.cpp
    mocks/MockJobInputsFactory.h
    mocks/MockJobOutputsFactory.h
    mocks/MockJobInputs.h
    mocks/MockJobOutputs.h
    mocks/MockQueue.h
    mocks/MockQueueFactory.h
    mocks/MockStep.h)
target_include_directories(test_mimo PRIVATE
    ${gtest_SOURCE_DIR}/include
    ${gmock_SOURCE_DIR}/include
    ../include
    mocks
)
target_link_libraries(test_mimo
    gtest_main
    gmock
    mimo
    workflow)
add_test(test_mimo test_units)
