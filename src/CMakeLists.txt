add_library(BPLUSTREE STATIC BPlusTreeStruct.cpp Insert.cpp Delete.cpp)
target_include_directories(BPLUSTREE PRIVATE ${CMAKE_SOURCE_DIR}/include)