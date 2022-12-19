include_directories(${CMAKE_SOURCE_DIR}/utilities)
include_directories(${CMAKE_SOURCE_DIR}/data_structures/dynamic_array)

add_executable(binary_heap_unittest binary_heap_unittest.cc)
target_link_libraries(binary_heap_unittest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(binary_heap_unittest)