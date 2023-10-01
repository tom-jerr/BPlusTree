add_executable(CONCURRENTTEST ConcurrentTest.cpp)
target_include_directories(CONCURRENTTEST PRIVATE ${CMAKE_SOURCE_DIR}/include)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)
