file(COPY  ${PROJECT_SOURCE_DIR}/Day8/InputDay8.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable( TestDay8 Day8.cc)
target_link_libraries(TestDay8 gtest_main)
add_test(AllTestsInDay8 TestDay8)
