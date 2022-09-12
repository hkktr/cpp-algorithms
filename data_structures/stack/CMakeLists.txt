include_directories(${CMAKE_SOURCE_DIR}/utilities)
include_directories(${CMAKE_SOURCE_DIR}/data_structures/dynamic_array)

add_executable(stack_unittest stack_unittest.cc)
target_link_libraries(stack_unittest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(stack_unittest)
