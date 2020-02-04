file(COPY  ${PROJECT_SOURCE_DIR}/Day3/InputDay3.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable( TestDay3 Day3.cc)
target_link_libraries(TestDay3 gtest_main)
add_test(AllTestsInDay3 TestDay3)
