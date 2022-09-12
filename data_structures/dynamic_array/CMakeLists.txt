include_directories(${CMAKE_SOURCE_DIR}/utilities)

add_executable(dynamic_array_unittest dynamic_array_unittest.cc)
target_link_libraries(dynamic_array_unittest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(dynamic_array_unittest)
