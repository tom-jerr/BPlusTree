add_library(BPLUSTREE STATIC CLBPlusTree.cpp CLBPTInsert.cpp CLBPTDelete.cpp CLBPTSearch.cpp)
target_include_directories(BPLUSTREE PRIVATE ${CMAKE_SOURCE_DIR}/include)