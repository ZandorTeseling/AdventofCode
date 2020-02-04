file(COPY  ${PROJECT_SOURCE_DIR}/Day9/InputDay9.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable( TestDay9 Day9.cc)
target_link_libraries(TestDay9 gtest_main)
add_test(AllTestsInDay9 TestDay9)
