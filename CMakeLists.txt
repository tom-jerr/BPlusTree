cmake_minimum_required(VERSION 3.20.1)
project(BPlusTree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DDEBUG -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -pthread")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(third_party/googletest)