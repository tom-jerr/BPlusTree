add_executable(UNITTEST UnitTest.cpp)
add_executable(PERFROMTEST PerformTest.cpp)
add_executable(RESULTTEST ResultTest.cpp)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
target_include_directories(UNITTEST PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(UNITTEST PUBLIC gtest gtest_main)
target_include_directories(PERFROMTEST PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(RESULTTEST PRIVATE ${CMAKE_SOURCE_DIR}/include)
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
add_test(
  NAME UNITTEST
  COMMAND UNITTEST
)