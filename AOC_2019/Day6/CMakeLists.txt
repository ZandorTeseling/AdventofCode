file(COPY  ${PROJECT_SOURCE_DIR}/Day6/InputDay6.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable( TestDay6 Day6.cc)
target_link_libraries(TestDay6 gtest_main)
add_test(AllTestsInDay6 TestDay6)
