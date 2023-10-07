#include <gtest/gtest.h>
#include <sys/types.h>

#include <cassert>
#include <cstdint>
#include <thread>
#include <vector>

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

void thread_delete_base(int begin, int end, BPlusTree<int, uint64_t>* tree) {
  for (int i = begin; i <= end; ++i) {
    tree->tree_delete(i);
  }
}

void thread_op1(BPlusTree<int, uint64_t>* tree) {
  thread_insert_base(1, 10, tree);
  for (int i = 1; i <= 5; ++i) {
    uint64_t data = tree->search(i);
    assert(data == i);
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
  std::vector<std::thread> threads;
  for (int i = 0; i < 100; ++i) {
    threads.push_back(
        std::thread(thread_insert_base, i * 100 + 1, i * 100 + 100, tree));
  }

  for (auto& thread : threads) {
    thread.join();
  }
}

void thread_delete_test(BPlusTree<int, uint64_t>* tree) {
  std::vector<std::thread> threads;
  for (int i = 0; i < 3; ++i) {
    threads.push_back(
        std::thread(thread_delete_base, i * 10 + 1, i * 10 + 10, tree));
  }

  for (auto& thread : threads) {
    thread.join();
  }
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
  auto* tree = new BPlusTree<int, uint64_t>(10);
  thread_insert_test(tree);
  // tree->show_bplustree();
  delete tree;
}

TEST(ConcurrentTree, ConcurrentDeletetest) {
  auto* tree = new BPlusTree<int, uint64_t>();
  for (int i = 1; i <= 60; ++i) {
    tree->tree_insert(i, i);
  }
  tree->show_bplustree();
  std::cout << "\n";
  thread_delete_test(tree);
  tree->show_bplustree();
  std::cout << "\n";
  // delete tree;
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