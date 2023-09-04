#include <gtest/gtest.h>

#include <cstdint>
#include <iostream>

#include "../include/BPlusTree.h"
#include "../src/BPlusTreeStruct.cpp"
void struct_test() {
  auto* node1 = new Node<int, uint64_t>();
  std::cout << sizeof(node1->degree) << "\n";

  auto* leaf1 = new LeafNode<int, uint64_t>();
  std::cout << sizeof(leaf1->degree) << "\n";

  auto* inner1 = new InnerNode<int, uint64_t>();
  std::cout << sizeof(inner1->degree) << "\n";

  auto* node2 = new LeafNode<int, uint64_t>();
  static_cast<LeafNode<int, uint64_t>*>(node2)->data.push_back(1);
  static_cast<LeafNode<int, uint64_t>*>(node2)->keys.push_back(2);
  for (auto v : static_cast<LeafNode<int, uint64_t>*>(node2)->data) {
    std::cout << v << " ";
  }
  std::cout << " \n";

  inner1->child.push_back(node2);
  std::cout << "inner1 child\n";
  for (auto* v : inner1->child) {
    std::cout << "key\t" << v->keys[0] << " \n";
    std::cout << "data\t" << dynamic_cast<LeafNode<int, uint64_t>*>(v)->data[0]
              << " \n";
  }
}

void search_test() { BPlusTree<int, uint64_t> tree; }

TEST(STRUCTTest, INTELEMENTTest) {
  // auto* node2 = new LeafNode<int, uint64_t>();
  // Node<int, uint64_t>* node3 = new LeafNode<int, uint64_t>();
  // static_cast<LeafNode<int, uint64_t>*>(node2)->data.push_back(1);
  // static_cast<LeafNode<int, uint64_t>*>(node2)->keys.push_back(2);
  // uint64_t data0 = static_cast<LeafNode<int, uint64_t>*>(node2)->data[0];
  // int key0 = static_cast<LeafNode<int, uint64_t>*>(node2)->keys[0];
  // EXPECT_EQ(data0, 1);
  // EXPECT_EQ(key0, 2);
  // EXPECT_TRUE(typeid(node3->getclasstype()) != typeid(LeafNode<int,
  // uint64_t>));

  auto* tree = new BPlusTree<int, uint64_t>();
  tree->tree_insert(1, static_cast<uint64_t>(1));
  tree->show_bplustree();
}
