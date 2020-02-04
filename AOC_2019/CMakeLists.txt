project(ADVENT)
cmake_minimum_required( VERSION 2.8)

set( LIBS_PATH "${ADVENT_SOURCE_DIR}/Libs" )
set( GTEST_LIBRARY ${LIBS_PATH}/gtest )
set( GTEST_MAIN_LIBRARY ${LIBS_PATH}/gtest )
set( GTEST_INCLUDE_DIR ${LIBS_PATH}/gtest )

enable_testing()
find_package(GTest REQUIRED)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -fvisibility-inlines-hidden -fno-omit-frame-pointer -pipe -feliminate-unused-debug-types -fPIC -std=c++11" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fPIC" )
add_definitions(-DLOG4CPP_FIX_ERROR_COLLISION)

add_subdirectory( ${PROJECT_SOURCE_DIR}/Libs/gtest)
add_subdirectory( ${PROJECT_SOURCE_DIR}/Day1)
add_subdirectory( ${PROJECT_SOURCE_DIR}/Day2)
add_subdirectory( ${PROJECT_SOURCE_DIR}/Day3)
add_subdirectory( ${PROJECT_SOURCE_DIR}/Day4)
add_subdirectory( ${PROJECT_SOURCE_DIR}/Day5)
add_subdirectory( ${PROJECT_SOURCE_DIR}/Day6)
add_subdirectory( ${PROJECT_SOURCE_DIR}/Day7)
add_subdirectory( ${PROJECT_SOURCE_DIR}/Day8)
add_subdirectory( ${PROJECT_SOURCE_DIR}/Day9)
#add_subdirectory( ${PROJECT_SOURCE_DIR}/Day10)
#add_subdirectory( ${PROJECT_SOURCE_DIR}/Day11)
#add_subdirectory( ${PROJECT_SOURCE_DIR}/Day12)


