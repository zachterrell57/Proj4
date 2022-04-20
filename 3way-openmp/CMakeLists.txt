cmake_minimum_required(VERSION 2.8)

# ALL OF THE SPECIFIED NAMES IN THIS CMAKE ARE REQUIRED.
# Set the project name variable to assignment3.
# THIS IS REQUIRED
project(test_hw2)
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werror")

include_directories(
                          "${CMAKE_SOURCE_DIR}"
                          "${CMAKE_SOURCE_DIR}/include"
                          )

add_library(openmp_min_ascii src/openmp_min_ascii.c)

add_executable(analysis src/analysis.c)

target_link_libraries(analysis openmp_min_ascii)