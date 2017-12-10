cmake_minimum_required(VERSION 3.7)
project(test_graph)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(googletest)


add_executable(test_mimo
        ../include/Entity.h
        ../include/entities/Integer.h
        test_Inputs.cpp
        test_Outputs.cpp
        test_Queue.cpp
        test_QueueChannel.cpp)
target_include_directories(test_mimo PRIVATE
    ${gtest_SOURCE_DIR}/include
    ../include
)
target_link_libraries(test_mimo
    gtest_main
    mimo)
add_test(test_mimo test_units)
