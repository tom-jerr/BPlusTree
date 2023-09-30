#include <gtest/gtest.h>

#include <cstdint>

#include "../include/CLBPlusTree.h"
#include "../include/CLSerialization.h"

TEST(SerializationTree, serializationtest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  for (int i = 1; i <= 50; ++i) {
    tree->tree_insert(i, i);
  }
  // tree->show_bplustree();
  // std::cout << '\n';

  Serialization s;
  std::string filename = "test.txt";

  s.serialize(*tree, filename);
  auto* new_tree = new BPlusTree<int, uint64_t>();

  new_tree = s.deserialize<int, uint64_t>(filename);
  // new_tree->show_bplustree();
  // new_tree->tree_insert(100, 100);
  // new_tree->show_bplustree();
  std::vector<int> ans = new_tree->bfs();
  ASSERT_EQ(ans, tree->bfs());
  delete tree;
}
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}