file(COPY  ${PROJECT_SOURCE_DIR}/Day4/InputDay4.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable( TestDay4 Day4.cc)
target_link_libraries(TestDay4 gtest_main)
add_test(AllTestsInDay4 TestDay4)
