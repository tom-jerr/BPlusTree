set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)

# 单元测试
add_executable(UNITTEST UnitTest.cpp)
add_executable(SERIALIZETEST SerializationTest.cpp)

# 性能测试
add_executable(PERFROMTEST PerformTest.cpp)
# 不同度数的性能测试
add_executable(RESULTTEST ResultTest.cpp)
# 与红黑树对比性能测试
add_executable(COMPARETEST CompareRBTest.cpp)
# 并发B+树测试
add_executable(CONCURRENTTEST ConcurrentTest.cpp)
target_include_directories(CONCURRENTTEST PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_include_directories(UNITTEST PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(SERIALIZETEST PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(PERFROMTEST PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(RESULTTEST PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(COMPARETEST PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 设置编译选项
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

# add gtest
target_link_libraries(UNITTEST PUBLIC gtest gtest_main)
target_link_libraries(SERIALIZETEST PUBLIC gtest gtest_main)
target_link_libraries(CONCURRENTTEST PUBLIC gtest gtest_main)

add_test(
  NAME UNITTEST
  COMMAND UNITTEST
)