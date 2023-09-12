#include <cstdint>
#include <iostream>

#include "../include/CLBPlusTree.h"
#include "../include/CLSerialization.h"

int main(int /*argc*/, char** /*argv*/) {
  auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, 1);
  tree->tree_insert(2, 2);
  tree->tree_insert(3, 3);
  std::cout << "tree before serialize:\n";
  tree->show_bplustree();

  Serialization s;
  std::string path = "1.txt";
  off_t len = s.serialize(*tree, path);
  std::cout << "serialized content length:\t" << len << "\n";

  auto* tree_deserialize = s.deserialize<int, uint64_t>(path.c_str());
  std::cout << "tree after deserialize:\n";
  tree_deserialize->show_bplustree();

  return 0;
}