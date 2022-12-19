include_directories(${CMAKE_SOURCE_DIR}/utilities)
include_directories(${CMAKE_SOURCE_DIR}/data_structures/dynamic_array)
include_directories(${CMAKE_SOURCE_DIR}/data_structures/binary_heap)

add_executable(priority_queue_unittest priority_queue_unittest.cc)
target_link_libraries(priority_queue_unittest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(priority_queue_unittest)
