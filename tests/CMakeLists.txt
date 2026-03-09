cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 REQUIRED)
add_executable(IdealDeterminant ideal_count_determinant.cpp)
target_link_libraries(IdealDeterminant Eigen3::Eigen)

add_executable(UnitTests unit_tests.cpp)
target_include_directories(UnitTests PRIVATE ${CMAKE_SOURCE_DIR}/inc)

target_link_libraries(UnitTests PRIVATE
   gtest
   gtest_main
)
