cmake_minimum_required(VERSION 3.24)
project(Concurrent_SkipList)

set(CMAKE_CXX_STANDARD 23)

add_executable(Concurrent_SkipList src/main.cpp src/skip-list-node.h src/skip-list.h)
