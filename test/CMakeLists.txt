add_executable(TEST InsertTest.cpp StructureTest.cpp main.cpp)
target_include_directories(TEST PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(TEST PUBLIC BPLUSTREE gtest_main)
add_test(
  NAME TEST
  COMMAND TEST
)