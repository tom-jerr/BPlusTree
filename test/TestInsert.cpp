#include <gtest/gtest.h>

#include <cstdint>
// #include <iostream>

#include "../include/CLBPlusTree.h"
#include "../src/CLBPTDelete.cpp"
#include "../src/CLBPTInsert.cpp"
#include "../src/CLBPlusTree.cpp"

TEST(InsertTest, leafnodetest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, static_cast<uint64_t>(1));
  // tree->show_bplustree();
  EXPECT_TRUE(tree->get_root()->is_leaf);
  EXPECT_TRUE(tree->get_root()->keys.size() == 1);
  EXPECT_TRUE(tree->get_root()->keys[0] = 1);
}

TEST(InsertTest, innernodetest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, static_cast<uint64_t>(1));
  tree->tree_insert(2, static_cast<uint64_t>(2));
  // tree->tree_insert(3, static_cast<uint64_t>(3));
  // tree->show_bplustree();
  EXPECT_TRUE(tree->get_root()->is_leaf);
  EXPECT_TRUE(tree->get_root()->keys.size() == 2);
  EXPECT_TRUE(tree->get_root()->keys[0] = 1);
  EXPECT_TRUE(tree->get_root()->keys[1] = 2);
}

TEST(InsertTest, splittest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, static_cast<uint64_t>(1));
  tree->tree_insert(2, static_cast<uint64_t>(2));
  tree->tree_insert(3, static_cast<uint64_t>(3));
  // tree->show_bplustree();
  EXPECT_FALSE(tree->get_root()->is_leaf);
  EXPECT_EQ(tree->get_depth(), 2);
}
TEST(InsertTest, splitnodetest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, static_cast<uint64_t>(1));
  tree->tree_insert(2, static_cast<uint64_t>(2));
  tree->tree_insert(3, static_cast<uint64_t>(3));
  tree->tree_insert(4, static_cast<uint64_t>(4));
  // tree->show_bplustree();
  EXPECT_EQ(tree->get_depth(), 2);
  EXPECT_EQ(tree->get_root()->keys.size(), 2);
  EXPECT_EQ(tree->get_root()->keys[1], 3);
  int key1 = dynamic_cast<InnerNode<int, uint64_t>*>(tree->get_root())
                 ->child[0]
                 ->keys[0];
  int key2 = dynamic_cast<InnerNode<int, uint64_t>*>(tree->get_root())
                 ->child[1]
                 ->keys[0];
  EXPECT_EQ(key1, 1);
  EXPECT_EQ(key2, 2);
}

TEST(InsertTest, splitinnertest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, static_cast<uint64_t>(1));
  tree->tree_insert(2, static_cast<uint64_t>(2));
  tree->tree_insert(3, static_cast<uint64_t>(3));
  tree->tree_insert(4, static_cast<uint64_t>(4));
  tree->tree_insert(5, static_cast<uint64_t>(5));
  // tree->tree_insert(6, static_cast<uint64_t>(6));
  // tree->show_bplustree();
  EXPECT_EQ(tree->get_depth(), 3);
  EXPECT_TRUE(tree->get_root()->keys.size() == 1);
  EXPECT_TRUE(tree->get_root()->keys[0] = 3);
  int key1 = dynamic_cast<InnerNode<int, uint64_t>*>(tree->get_root())
                 ->child[0]
                 ->keys[0];
  int key2 = dynamic_cast<InnerNode<int, uint64_t>*>(tree->get_root())
                 ->child[1]
                 ->keys[0];
  EXPECT_EQ(key1, 2);
  EXPECT_EQ(key2, 4);
  int leafkey1 =
      dynamic_cast<InnerNode<int, uint64_t>*>(
          dynamic_cast<InnerNode<int, uint64_t>*>(tree->get_root())->child[1])
          ->child[0]
          ->keys[0];
  int leafkey2 =
      dynamic_cast<InnerNode<int, uint64_t>*>(
          dynamic_cast<InnerNode<int, uint64_t>*>(tree->get_root())->child[1])
          ->child[1]
          ->keys[1];

  EXPECT_EQ(leafkey1, 3);
  EXPECT_EQ(leafkey2, 5);
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
