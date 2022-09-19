include_directories(${CMAKE_SOURCE_DIR}/utilities)

add_executable(deque_unittest deque_unittest.cc)
target_link_libraries(deque_unittest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(deque_unittest)
