include_directories(${CMAKE_SOURCE_DIR}/utilities)
include_directories(${CMAKE_SOURCE_DIR}/data_structures/doubly_linked_list)
include_directories(${CMAKE_SOURCE_DIR}/data_structures/dynamic_array)

add_executable(hash_map_unittest hash_map_unittest.cc)
target_link_libraries(hash_map_unittest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(hash_map_unittest)