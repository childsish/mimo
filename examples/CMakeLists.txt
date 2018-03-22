add_executable(linear linear.cpp
        entities/Integer.h
        steps/Print.h
        steps/Range.h)
target_include_directories(linear PRIVATE
        include)
target_link_libraries(linear
        mimo
        workflow)

add_executable(join join.cpp)
target_include_directories(join PRIVATE
        include)
target_link_libraries(join
        mimo
        workflow)

add_executable(fork fork.cpp)
target_include_directories(fork PRIVATE
        include)
target_link_libraries(fork
        mimo
        workflow)

add_executable(split split.cpp)
target_include_directories(split PRIVATE
        include)
target_link_libraries(split
        mimo
        workflow)
