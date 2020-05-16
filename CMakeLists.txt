cmake_minimum_required(VERSION 3.0.0)
project(k_tree VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(copy_move_test copy_move_test.cpp)
add_executable(clear_test clear_test.cpp)
add_test(copy_move_test copy_move_test)
add_test(clear_test clear_test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)