file(COPY  ${PROJECT_SOURCE_DIR}/Day2/InputDay2.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable( TestDay2 Day2.cc)
target_link_libraries(TestDay2 gtest_main)
add_test(AllTestsInDay2 TestDay2)
