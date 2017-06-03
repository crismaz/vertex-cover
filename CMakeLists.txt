cmake_minimum_required(VERSION 3.7)
project(vertex_cover)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp BipartiteGraph.cpp BipartiteGraph.h Graph.cpp Graph.h)
add_executable(vertex_cover ${SOURCE_FILES})
add_executable(test_gen gen.cpp)

target_compile_options(vertex_cover PRIVATE -O2)
