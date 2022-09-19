include_directories(${CMAKE_SOURCE_DIR}/utilities)
include_directories(${CMAKE_SOURCE_DIR}/data_structures/deque)

add_executable(queue_unittest queue_unittest.cc)
target_link_libraries(queue_unittest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(queue_unittest)
