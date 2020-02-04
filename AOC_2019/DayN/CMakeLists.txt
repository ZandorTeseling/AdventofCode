file(COPY  ${PROJECT_SOURCE_DIR}/Day__/InputDay__.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable( TestDay_ Day_.cc)
target_link_libraries(TestDay_ gtest_main)
add_test(AllTestsInDay_ TestDay_)
