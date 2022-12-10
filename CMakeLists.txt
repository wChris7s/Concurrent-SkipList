cmake_minimum_required(VERSION 3.24)
project(Concurrent_SkipList)

set(CMAKE_CXX_STANDARD 23)
set(PROJECT_HEADERS src/node.h src/SkipList.h)
set(PROJECT_SOURCE src/main.cpp src/node.cpp src/SkipList.cpp src/generate_data.h)

add_executable(Concurrent_SkipList ${PROJECT_SOURCE} ${PROJECT_HEADERS})
