#include <cstdint>
// #include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "../include/CLBPlusTree.h"

#define DGREE_18 493
#define DGREE_20 400
#define TENMILLION 10000000
#define STRIDE 10000

void TENMILLIONTEST(int degree) {
  std::cout << "single thread B+ tree \n";
  auto* tree = new BPlusTree<int, uint64_t>(degree);

  uint64_t begin = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
  for (int i = 1; i <= TENMILLION; ++i) {
    tree->tree_insert(i, i);
  }
  uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
  uint64_t cost = end - begin;
  std::cout << "insert time cost:\t" << cost << "ms" << '\n';

  // begin = std::chrono::duration_cast<std::chrono::milliseconds>(
  //             std::chrono::system_clock::now().time_since_epoch())
  //             .count();

  // for (int i = 1; i <= TENMILLION; ++i) {
  //   tree->search(i);
  // }

  // end = std::chrono::duration_cast<std::chrono::milliseconds>(
  //           std::chrono::system_clock::now().time_since_epoch())
  //           .count();
  // cost = end - begin;
  // std::cout << "search time cost:\t" << cost << "ms" << '\n';

  // begin = std::chrono::duration_cast<std::chrono::seconds>(
  //             std::chrono::system_clock::now().time_since_epoch())
  //             .count();
  // for (int i = 1; i <= 10000000; ++i) {
  //   tree->tree_delete(i);
  // }

  // end = std::chrono::duration_cast<std::chrono::seconds>(
  //           std::chrono::system_clock::now().time_since_epoch())
  //           .count();
  // cost = end - begin;
  // std::cout << "delete time cost:\t" << cost << "ms" << '\n';
}

void thread_insert_base(int begin, int end, BPlusTree<int, uint64_t>* tree) {
  for (int i = begin; i <= end; ++i) {
    tree->tree_insert(i, i);
  }
}

void thread_search(int begin, int end, BPlusTree<int, uint64_t>* tree) {
  for (int i = begin; i <= end; ++i) {
    tree->search(i);
  }
}

void thread_delete_base(int begin, int end, BPlusTree<int, uint64_t>* tree) {
  for (int i = begin; i <= end; ++i) {
    tree->tree_delete(i);
  }
}

void thread_test() {
  std::cout << "multithread\n";
  auto* tree = new BPlusTree<int, uint64_t>(DGREE_20);
  std::vector<std::thread> threads;
  for (int i = 0; i < 1000; ++i) {
    threads.push_back(std::thread(thread_insert_base, i * 10000 + 1,
                                  i * 10000 + 10000, tree));
  }
  uint64_t begin = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
  for (auto& thread : threads) {
    thread.join();
  }
  uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
  uint64_t cost = end - begin;
  std::cout << "insert time cost:\t" << cost << "ms" << '\n';

  // begin = std::chrono::duration_cast<std::chrono::milliseconds>(
  //             std::chrono::system_clock::now().time_since_epoch())
  //             .count();
  // for (int i = 0; i < 100; ++i) {
  //   std::thread t1(thread_search, i * STRIDE + 1, i * STRIDE + STRIDE - 1,
  //                  tree);
  //   t1.join();
  // }
  // end = std::chrono::duration_cast<std::chrono::milliseconds>(
  //           std::chrono::system_clock::now().time_since_epoch())
  //           .count();
  // cost = end - begin;
  // std::cout << "search time cost:\t" << cost << "ms" << '\n';

  // begin = std::chrono::duration_cast<std::chrono::milliseconds>(
  //             std::chrono::system_clock::now().time_since_epoch())
  //             .count();
  // for (int i = 0; i < 100; ++i) {
  //   std::thread t1(thread_delete_base, i * STRIDE + 1, i * STRIDE + STRIDE -
  //   1,
  //                  tree);
  //   t1.join();
  // }
  // end = std::chrono::duration_cast<std::chrono::milliseconds>(
  //           std::chrono::system_clock::now().time_since_epoch())
  //           .count();
  // cost = end - begin;
  // std::cout << "delete time cost:\t" << cost << "ms" << '\n';
}

int main(int /*argc*/, char** /*argv*/) {
  // TENMILLIONTEST(DGREE_18);
  TENMILLIONTEST(DGREE_20);
  thread_test();
  return 0;
}