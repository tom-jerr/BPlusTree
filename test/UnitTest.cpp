#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "../include/CLBPlusTree.h"

class InsertTree : public ::testing::Test {
 public:
  BPlusTree<int, uint64_t>* tree;

  void SetUp() override { tree = new BPlusTree<int, uint64_t>(); }
  void TearDown() override { delete tree; }
};
TEST_F(InsertTree, leafnodetest) {
  // auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, 1);
  // //tree->show_bplustree();
  std::vector<int> ans;
  ans.push_back(1);
  ASSERT_EQ(tree->bfs(), ans) << "The root key is not 1\n";
}

TEST_F(InsertTree, InternalNodetest) {
  tree->tree_insert(1, 1);
  tree->tree_insert(2, static_cast<uint64_t>(2));
  std::vector<int> ans;
  ans.push_back(1);
  ans.push_back(2);
  // tree->show_bplustree();
  ASSERT_EQ(tree->bfs(), ans) << "The root keys are not 1 or 2\n";
}

TEST_F(InsertTree, splitleaftest) {
  tree->tree_insert(1, 1);
  tree->tree_insert(2, 2);
  tree->tree_insert(3, 3);
  // tree->show_bplustree();
  std::vector<int> ans = {2, 1, 2, 3};
  ASSERT_EQ(tree->bfs(), ans) << "Leaf node split failed\n";
}
TEST_F(InsertTree, splitinternaltest) {
  tree->tree_insert(1, 1);
  tree->tree_insert(2, 2);
  tree->tree_insert(3, 3);
  tree->tree_insert(4, static_cast<uint64_t>(4));
  tree->tree_insert(5, static_cast<uint64_t>(5));
  // tree->show_bplustree();
  std::vector<int> ans = {3, 2, 4, 1, 2, 3, 4, 5};
  ASSERT_EQ(tree->bfs(), ans) << "Internal node split failed\n";
}

class SearchTree : public ::testing::Test {
 public:
  BPlusTree<int, uint64_t>* tree;

  void SetUp() override {
    tree = new BPlusTree<int, uint64_t>();
    for (int i = 1; i <= 100; ++i) {
      tree->tree_insert(i, i);
    }
  }
  void TearDown() override { delete tree; }
};
TEST_F(SearchTree, searchtest) {
  // //tree->show_bplustree();
  for (int i = 1; i <= 100; ++i) {
    uint64_t data = tree->search(i);
    ASSERT_EQ(data, i);
  }
}

TEST_F(SearchTree, searchrangetest) {
  std::vector<uint64_t> res = tree->search_range(1, 20);
  std::vector<uint64_t> ans;
  for (int i = 1; i <= 20; ++i) {
    ans.push_back(i);
  }
  ASSERT_EQ(res, ans);
}

class DeleteTree : public ::testing::Test {
 public:
  BPlusTree<int, uint64_t>* tree;

  void SetUp() override {
    tree = new BPlusTree<int, uint64_t>();
    std::vector<int> insert_data = {0, 50, 100, 75, 30, 40, 80, 45, 20};
    for (int i : insert_data) {
      tree->tree_insert(i, i);
    }
  }
  void TearDown() override { delete tree; }
};

TEST_F(DeleteTree, structtest) {
  // tree->show_bplustree();
  std::vector<int> ans = {50, 30, 40, 75, 80, 0,  20,
                          30, 40, 45, 50, 75, 80, 100};
  ASSERT_EQ(tree->bfs(), ans);
}

TEST_F(DeleteTree, deleteLeaftest) {
  tree->tree_delete(20);
  std::vector<int> ans = {50, 30, 40, 75, 80, 0, 30, 40, 45, 50, 75, 80, 100};
  ASSERT_EQ(tree->bfs(), ans);
  // tree->show_bplustree();
}

TEST_F(DeleteTree, deleteInternaltest) {
  tree->tree_delete(20);
  tree->tree_delete(80);
  // tree->show_bplustree();
  std::vector<int> ans = {50, 30, 40, 75, 100, 0, 30, 40, 45, 50, 75, 100};
  ASSERT_EQ(tree->bfs(), ans);
}

TEST_F(DeleteTree, borrowRightLeaftest) {
  tree->tree_delete(20);
  tree->tree_delete(80);
  tree->tree_delete(30);
  std::vector<int> ans = {50, 40, 45, 75, 100, 0, 40, 45, 50, 75, 100};
  ASSERT_EQ(tree->bfs(), ans);
  // tree->show_bplustree();
}

TEST_F(DeleteTree, borrowLeftLeaftest) {
  tree->tree_delete(20);
  tree->tree_delete(80);
  tree->tree_delete(40);
  std::vector<int> ans = {50, 30, 45, 75, 100, 0, 30, 45, 50, 75, 100};
  ASSERT_EQ(tree->bfs(), ans);
  // tree->show_bplustree();
}

TEST_F(DeleteTree, mergeRightLeaftest) {
  tree->tree_delete(20);
  tree->tree_delete(80);
  tree->tree_delete(40);
  tree->tree_delete(0);
  std::vector<int> ans = {50, 45, 75, 100, 30, 45, 50, 75, 100};
  ASSERT_EQ(tree->bfs(), ans);
  // tree->show_bplustree();
}
TEST_F(DeleteTree, mergeLeftLeaftest) {
  tree->tree_delete(20);
  tree->tree_delete(80);
  tree->tree_delete(40);
  tree->tree_delete(0);
  tree->tree_delete(100);
  std::vector<int> ans = {50, 45, 75, 30, 45, 50, 75};
  ASSERT_EQ(tree->bfs(), ans);
  // tree->show_bplustree();
}

TEST_F(DeleteTree, mergeRightInternaltest) {
  tree->tree_delete(20);
  tree->tree_delete(80);
  tree->tree_delete(40);
  tree->tree_delete(0);
  tree->tree_delete(100);
  tree->tree_insert(60, 60);
  tree->tree_delete(50);
  // tree->show_bplustree();
  tree->tree_delete(30);
  // tree->show_bplustree();
  std::vector<int> ans = {60, 75, 45, 60, 75};
  ASSERT_EQ(tree->bfs(), ans);
}

TEST_F(DeleteTree, mergeLeftInternaltest) {
  tree->tree_delete(20);
  tree->tree_delete(80);
  tree->tree_delete(40);
  tree->tree_delete(0);
  tree->tree_delete(100);
  tree->tree_insert(60, 60);
  tree->tree_delete(50);
  tree->tree_delete(30);
  tree->tree_insert(80, 80);
  tree->tree_insert(90, 90);
  tree->tree_delete(80);
  // tree->show_bplustree();
  tree->tree_delete(90);
  // tree->show_bplustree();
  std::vector<int> ans = {60, 75, 45, 60, 75};
  ASSERT_EQ(tree->bfs(), ans);
  // tree->show_bplustree();
}

TEST_F(DeleteTree, borrowRightInternaltest) {
  tree->tree_delete(20);
  tree->tree_delete(80);
  tree->tree_delete(40);
  tree->tree_delete(0);
  tree->tree_delete(100);
  tree->tree_insert(60, 60);
  tree->tree_delete(50);
  tree->tree_delete(30);
  tree->tree_insert(80, 80);
  tree->tree_insert(90, 90);
  tree->tree_delete(80);
  tree->tree_delete(90);
  tree->tree_insert(80, 80);
  tree->tree_insert(90, 90);
  tree->tree_insert(100, 100);
  // tree->show_bplustree();
  tree->tree_delete(45);
  std::vector<int> ans = {80, 75, 90, 60, 75, 80, 90, 100};
  ASSERT_EQ(tree->bfs(), ans);
  // tree->show_bplustree();
}

TEST_F(DeleteTree, borrowLeftInternaltest) {
  tree->tree_delete(20);
  tree->tree_delete(80);
  tree->tree_delete(40);
  tree->tree_delete(0);
  tree->tree_delete(100);
  tree->tree_insert(60, 60);
  tree->tree_delete(50);
  tree->tree_delete(30);
  tree->tree_insert(80, 80);
  tree->tree_insert(90, 90);
  tree->tree_delete(80);
  tree->tree_delete(90);
  tree->tree_insert(80, 80);
  tree->tree_insert(90, 90);
  tree->tree_insert(100, 100);
  tree->tree_delete(45);
  tree->tree_insert(40, 40);
  tree->tree_insert(50, 50);

  tree->tree_delete(100);
  // tree->show_bplustree();
  tree->tree_delete(90);
  // tree->show_bplustree();
  std::vector<int> ans = {75, 50, 80, 40, 50, 60, 75, 80};
  ASSERT_EQ(tree->bfs(), ans);
}

TEST_F(DeleteTree, deleteAlltest) {
  std::vector<int> delete_data = {0, 50, 100, 75, 30, 40, 80, 45, 20};
  for (int i : delete_data) {
    // tree->show_bplustree();
    tree->tree_delete(i);
  }
  // tree->show_bplustree();
  std::vector<int> ans = {};
  ASSERT_EQ(tree->bfs(), ans);
}
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
