#include <cstdint>
#include <iostream>
#include <thread>

#include "../include/CLBPlusTree.h"
#include "../include/CLSerialization.h"

BPlusTree<int, uint64_t>* tree = new BPlusTree<int, uint64_t>();
int i = 0;
void serialization_test() {
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

  std::cout << "insert <4,4>\n";
  tree_deserialize->tree_insert(4, 4);
  tree_deserialize->show_bplustree();
  for (int i = 1; i <= 4; ++i) {
    std::cout << "delete " << i << "\n";
    tree_deserialize->tree_delete(i);
    tree_deserialize->show_bplustree();
  }
}
void search_1() {
  int data = tree->search(1);
  printf("%d key : %d\n", 1, data);
}
void search_2() {
  int data = tree->search(2);
  printf("%d key : %d\n", 2, data);
}

void insert_1() {
  tree->tree_insert(1, 1);
  tree->tree_insert(2, 2);
  tree->show_bplustree();
  tree->tree_delete(1);
  tree->show_bplustree();
}

void insert_2() {
  tree->tree_insert(3, 3);
  tree->show_bplustree();
  tree->tree_insert(4, 4);
  tree->show_bplustree();
  tree->tree_delete(4);
  tree->show_bplustree();
}

void delete_1() {
  tree->tree_delete(1);
  tree->show_bplustree();
}
void delete_2() {
  tree->tree_delete(2);
  tree->show_bplustree();
}
void latch_search_test() {
  tree->tree_insert(1, 1);
  tree->tree_insert(2, 2);
  tree->tree_insert(3, 3);
  std::cout << "tree before serialize:\n";
  tree->show_bplustree();

  std::thread th1(search_1);
  std::thread th3(search_1);
  std::thread th2(search_2);
  th1.join();
  th2.join();
  th3.join();
}

void latch_insert_test() {
  tree->show_bplustree();

  std::thread th1(insert_1);
  std::thread th3(insert_2);
  th1.join();
  th3.join();
  // std::thread th2(search_2);
  // th2.join();
}
void delete_latch_test() {
  tree->show_bplustree();

  std::thread th1(insert_1);
  std::thread th3(insert_2);
  th1.join();
  th3.join();
  // std::thread th2(delete_1);
  // th2.join();
  // std::thread th4(delete_2);
  // th4.join();
}
void delete_test() {
  tree->tree_insert(1, 1);
  tree->tree_insert(2, 2);
  tree->tree_insert(3, 3);
  tree->tree_insert(4, 4);
  tree->tree_insert(5, 5);
  std::cout << "tree before serialize:\n";
  tree->show_bplustree();
  for (int i = 1; i <= 5; ++i) {
    tree->tree_delete(i);
  }
  delete tree;
}
int main(int /*argc*/, char** /*argv*/) {
  serialization_test();
  // latch_search_test();
  // delete_test();

  // latch_insert_test();
  // delete_latch_test();
  return 0;
}