cmake_minimum_required(VERSION 3.7)
project(mimo)

set(CMAKE_CXX_STANDARD 14)


set(GRAPH__DISABLE_TESTS on)
add_subdirectory(../graph ${PROJECT_BINARY_DIR}/lib/graph)

set(SOURCE_FILES
        include/Entity.h
        src/Queue.cpp include/Queue.h
        src/QueuePusher.cpp include/QueuePusher.h
        src/Stream.cpp include/Stream.h
        include/streams/Capture.h
        include/streams/Release.h
        include/streams/Sink.h)
add_library(mimo ${SOURCE_FILES})
target_include_directories(mimo PRIVATE
        include)

enable_testing()
add_subdirectory(tests)
