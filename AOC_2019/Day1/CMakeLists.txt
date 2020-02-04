file(COPY  ${PROJECT_SOURCE_DIR}/Day1/InputDay1.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable( TestDay1 Day1.cc)
target_link_libraries(TestDay1 gtest_main)
add_test(AllTestsInDay1 TestDay1)
