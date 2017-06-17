cmake_minimum_required(VERSION 3.7)
project(mimo)

set(CMAKE_CXX_STANDARD 14)


set(GRAPH__DISABLE_TESTS on)
add_subdirectory(../graph ${PROJECT_BINARY_DIR}/lib/graph)

set(SOURCE_FILES
        src/Entity.cpp include/Entity.h
        src/Pipeline.cpp include/Pipeline.h
        src/Queue.cpp include/Queue.h
        src/QueuePusher.cpp include/QueuePusher.h
        src/Stream.cpp include/Stream.h
        src/entities/Integer.cpp include/entities/Integer.h
        include/streams/Capture.h
        include/streams/Print.h
        include/streams/Range.h
        include/streams/Release.h
        include/streams/Sink.h)
add_library(mimo ${SOURCE_FILES})
target_include_directories(mimo PRIVATE
        include
        ${PROJECT_BINARY_DIR}/lib/graph/include)

enable_testing()

add_executable(linear examples/linear.cpp)
target_include_directories(linear PRIVATE
        include)
target_link_libraries(linear mimo)
add_test(linear linear)

add_executable(join examples/join.cpp)
target_include_directories(join PRIVATE
        include)
target_link_libraries(join mimo)
add_test(join join)

add_subdirectory(tests)
