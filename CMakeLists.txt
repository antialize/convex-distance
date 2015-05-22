cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS -std=c++11)
add_executable(test_exe distance.h distance.cpp test.cpp)

enable_testing()
add_test(lines1 test_exe lines1)
add_test(lines2 test_exe lines2)
add_test(lines3 test_exe lines3)
add_test(lines4 test_exe lines4)
add_test(lines5 test_exe lines5)
add_test(point1 test_exe point1)
add_test(point2 test_exe point2)
add_test(point3 test_exe point3)
add_test(tri1 test_exe tri1)
add_test(tri2 test_exe tri2)
add_test(tri3 test_exe tri3)
add_test(gen1 test_exe gen1)


