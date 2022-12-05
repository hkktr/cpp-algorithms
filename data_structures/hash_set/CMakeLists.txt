include_directories(${CMAKE_SOURCE_DIR}/utilities)
include_directories(${CMAKE_SOURCE_DIR}/data_structures/doubly_linked_list)
include_directories(${CMAKE_SOURCE_DIR}/data_structures/dynamic_array)

add_executable(hash_set_unittest hash_set_unittest.cc)
target_link_libraries(hash_set_unittest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(hash_set_unittest)