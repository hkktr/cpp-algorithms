add_executable(array_unittest array_unittest.cc)
target_link_libraries(array_unittest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(array_unittest)
