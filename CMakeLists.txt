cmake_minimum_required(VERSION 3.7)
project(vertex_cover)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp BipartiteGraph.cpp BipartiteGraph.h)
add_executable(vertex_cover ${SOURCE_FILES})
