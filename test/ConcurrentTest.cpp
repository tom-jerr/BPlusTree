#include <gtest/gtest.h>
#include <sys/types.h>

#include <cassert>
#include <cstdint>
#include <thread>

#include "../include/CLBPlusTree.h"

class ConcurrentTree : public ::testing::Test {
 public:
  BPlusTree<int, uint64_t>* tree;

  void SetUp() override { tree = new BPlusTree<int, uint64_t>(); }
  void TearDown() override { delete tree; }
};
void thread_insert_base(int begin, int end, BPlusTree<int, uint64_t>* tree) {
  for (int i = begin; i <= end; ++i) {
    tree->tree_insert(i, i);
  }
}

void thread_op1(BPlusTree<int, uint64_t>* tree) {
  thread_insert_base(1, 10, tree);
  for (int i = 1; i <= 5; ++i) {
    uint64_t data = tree->search(i);
    assert(data == i);
  }
  for (int i = 6; i <= 10; ++i) {
    tree->tree_delete(i);
  }
}

void thread_op2(BPlusTree<int, uint64_t>* tree) {
  thread_insert_base(11, 15, tree);
  for (int i = 11; i <= 15; ++i) {
    uint64_t data = tree->search(i);
    assert(data == i);
  }
}

void thread_op3(BPlusTree<int, uint64_t>* tree) {
  thread_insert_base(11, 15, tree);
  for (int i = 11; i <= 15; ++i) {
    uint64_t data = tree->search(i);
    assert(data == i);
  }
}

void thread_op4(BPlusTree<int, uint64_t>* tree) {
  for (int i = 6; i <= 10; ++i) {
    tree->tree_delete(i);
  }
}
void thread_insert_test(BPlusTree<int, uint64_t>* tree) {
  std::thread t1(thread_insert_base, 1, 3, tree);
  std::thread t2(thread_insert_base, 4, 6, tree);
  std::thread t3(thread_insert_base, 7, 10, tree);

  t1.join();
  t2.join();
  t3.join();
}

void thread_op_test(BPlusTree<int, uint64_t>* tree) {
  std::thread t1(thread_op1, tree);
  std::thread t2(thread_op2, tree);

  t1.join();
  t2.join();
}

void thread_op_test2(BPlusTree<int, uint64_t>* tree) {
  std::thread t1(thread_op3, tree);
  std::thread t2(thread_op4, tree);

  t1.join();
  t2.join();
}

TEST(ConcurrentTree, ConcurrentInserttest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  thread_insert_test(tree);
  tree->show_bplustree();
  delete tree;
}

TEST(ConcurrentTree, Concurrenttest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  for (int i = 1; i <= 10; ++i) {
    tree->tree_insert(i, i);
  }
  thread_op_test2(tree);
  tree->show_bplustree();
  delete tree;
}

TEST(ConcurrentTree, Concurrenttest2) {
  auto* tree = new BPlusTree<int, uint64_t>();
  thread_op_test(tree);
  tree->show_bplustree();
  delete tree;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}