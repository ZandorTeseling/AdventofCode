file(COPY  ${PROJECT_SOURCE_DIR}/Day7/InputDay7.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable( TestDay7 Day7.cc)
target_link_libraries(TestDay7 gtest_main)
add_test(AllTestsInDay7 TestDay7)
