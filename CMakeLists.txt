cmake_minimum_required(VERSION 3.10)
project(cpprich VERSION 1.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)
add_subdirectory(src)