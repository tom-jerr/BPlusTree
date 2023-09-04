#include <gtest/gtest.h>

#include <cstdint>
// #include <iostream>

#include "../include/BPlusTree.h"
#include "../src/BPlusTreeStruct.cpp"
#include "../src/Delete.cpp"
#include "../src/Insert.cpp"

TEST(InsertTest, leafnodetest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, static_cast<uint64_t>(1));
  tree->show_bplustree();
}

TEST(InsertTest, innernodetest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, static_cast<uint64_t>(1));
  tree->tree_insert(2, static_cast<uint64_t>(2));
  // tree->tree_insert(3, static_cast<uint64_t>(3));
  tree->show_bplustree();
}

// TEST(InsertTest, splittest) {
//   auto* tree = new BPlusTree<int, uint64_t>();
//   tree->tree_insert(1, static_cast<uint64_t>(1));
//   tree->tree_insert(2, static_cast<uint64_t>(2));
//   tree->tree_insert(3, static_cast<uint64_t>(3));
//   tree->show_bplustree();
// }
TEST(InsertTest, splitnodetest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, static_cast<uint64_t>(1));
  tree->tree_insert(2, static_cast<uint64_t>(2));
  tree->tree_insert(3, static_cast<uint64_t>(3));
  tree->tree_insert(4, static_cast<uint64_t>(4));
  tree->show_bplustree();
}

TEST(InsertTest, splitinnertest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, static_cast<uint64_t>(1));
  tree->tree_insert(2, static_cast<uint64_t>(2));
  tree->tree_insert(3, static_cast<uint64_t>(3));
  tree->tree_insert(4, static_cast<uint64_t>(4));
  tree->tree_insert(5, static_cast<uint64_t>(5));
  // tree->tree_insert(6, static_cast<uint64_t>(6));
  tree->show_bplustree();
  tree->tree_delete(1);
  tree->show_bplustree();
}

// TEST(InsertTest, splitinnertest2) {
//   auto* tree = new BPlusTree<int, uint64_t>();
//   tree->tree_insert(1, static_cast<uint64_t>(1));
//   tree->tree_insert(2, static_cast<uint64_t>(2));
//   tree->tree_insert(3, static_cast<uint64_t>(3));
//   tree->tree_insert(4, static_cast<uint64_t>(4));
//   tree->tree_insert(5, static_cast<uint64_t>(5));
//   tree->tree_insert(6, static_cast<uint64_t>(6));
//   tree->show_bplustree();
// }
