file(COPY  ${PROJECT_SOURCE_DIR}/Day5/InputDay5.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable( TestDay5 Day5.cc)
target_link_libraries(TestDay5 gtest_main)
add_test(AllTestsInDay5 TestDay5)
