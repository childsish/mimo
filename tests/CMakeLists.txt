add_subdirectory(googletest)

add_executable(test_mimo
    JobTest.cpp
    MultiJobDepotTest.cpp
    QueueTest.cpp
    QueueBundleTest.cpp
    SingleJobDepotTest.cpp
    mocks/MockFactory.h
    mocks/MockJob.h
    mocks/MockQueue.h
    mocks/MockQueueBundle.h
    mocks/MockSingleJobDepot.h
    mocks/MockSingleJobDepotFactory.h
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
