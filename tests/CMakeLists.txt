cmake_minimum_required(VERSION 3.7)
project(test_graph)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(googletest)


add_executable(test_mimo
    JobTest.cpp
    QueueTest.cpp
    QueueBundleTest.cpp
    mocks/MockFactory.h
    mocks/MockQueue.h
    mocks/MockQueueBundle.h
    mocks/MockStep.h
)
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
)
add_test(test_mimo test_units)
