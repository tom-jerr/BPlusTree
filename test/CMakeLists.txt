add_executable(BPLUSTREETEST TestStruct.cpp TestInsert.cpp main.cpp)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
target_include_directories(BPLUSTREETEST PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(BPLUSTREETEST PUBLIC BPLUSTREE gtest_main)
add_test(
  NAME BPLUSTREETEST
  COMMAND BPLUSTREETEST
)