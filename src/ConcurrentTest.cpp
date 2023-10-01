#include <cassert>
#include <cstdint>
#include <thread>

#include "../include/CLBPlusTree.h"

BPlusTree<int, uint64_t>* tree = new BPlusTree<int, uint64_t>();

void thread_insert_base(int begin, int end) {
  for (int i = begin; i <= end; ++i) {
    tree->tree_insert(i, i);
  }
}

void thread_insert_test() {
  std::thread t1(thread_insert_base, 1, 3);
  std::thread t2(thread_insert_base, 4, 6);
  std::thread t3(thread_insert_base, 7, 10);

  t1.join();
  t2.join();
  t3.join();
}

void thread_op1() {
  thread_insert_base(1, 10);
  for (int i = 1; i <= 5; ++i) {
    uint64_t data = tree->search(i);
    assert(data == i);
  }
  for (int i = 6; i <= 10; ++i) {
    tree->tree_delete(i);
  }
}
void thread_op2() {
  thread_insert_base(11, 15);
  for (int i = 11; i <= 15; ++i) {
    uint64_t data = tree->search(i);
    assert(data == i);
  }
}

void thread_op_test() {
  std::thread t1(thread_op1);
  std::thread t2(thread_op2);

  t1.join();
  t2.join();
}

int main() {
  // thread_insert_test();
  thread_op_test();
  tree->show_bplustree();
  return 0;
}