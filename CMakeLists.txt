cmake_minimum_required(VERSION 3.16)
project(Project4)

set(CMAKE_CXX_STANDARD 11)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Wuninitialized -pedantic-errors -Wconversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
add_executable(Project4 Main.cpp Seat.cpp Seat.h Auditorium.cpp Auditorium.h Node.h)