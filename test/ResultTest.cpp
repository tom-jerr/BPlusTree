#include <cstdint>
// #include <iostream>
#include <chrono>
#include <map>
#include <utility>

#include "../include/CLBPlusTree.h"

#define TENTHOUSAND 10000
#define HUNTHOUSAND 100000
#define MILLION 1000000
#define TENMILLION 10000000

void TENMILLIONTEST() {
  auto* tree = new BPlusTree<int, uint64_t>();

  uint64_t begin = std::chrono::duration_cast<std::chrono::seconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
  for (int i = 1; i <= TENMILLION; ++i) {
    tree->tree_insert(i, i);
  }
  uint64_t end = std::chrono::duration_cast<std::chrono::seconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
  uint64_t cost = end - begin;
  std::cout << "B+ Tree insert time cost:\t" << cost << "s" << '\n';
  // tree->show_bplustree();
  // begin = std::chrono::duration_cast<std::chrono::seconds>(
  //             std::chrono::system_clock::now().time_since_epoch())
  //             .count();
  // for (int i = 1; i <= TENMILLION; ++i) {
  //   tree->search(i);
  // }
  // end = std::chrono::duration_cast<std::chrono::seconds>(
  //           std::chrono::system_clock::now().time_since_epoch())
  //           .count();
  // cost = end - begin;
  // std::cout << "search time cost:\t" << cost << "s" << '\n';
}

void result_test(int begin, int end, int stride) {
  for (int i = begin; i <= end; i += stride) {
    auto* tree = new BPlusTree<int, uint64_t>();

    uint64_t begin = std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count();
    for (int j = 1; j <= i; ++j) {
      tree->tree_insert(j, j);
    }
    uint64_t end = std::chrono::duration_cast<std::chrono::microseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
    uint64_t cost = end - begin;
    double cost_ms = static_cast<double>(cost) / 1000;
    double cost_s = static_cast<double>(cost) / 1e6;
    std::cout << i << " insert time cost:\t" << cost << "μs" << '\n';
    std::cout << i << " insert time cost:\t" << cost_ms << "ms" << '\n';
    std::cout << i << " insert time cost:\t" << cost_s << "s" << '\n';
  }
}

void ten_million_red_black_tree_test() {
  std::map<int, uint64_t> test_map;
  uint64_t begin = std::chrono::duration_cast<std::chrono::seconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
  for (int i = 1; i <= TENMILLION; ++i) {
    test_map.insert(std::make_pair(i, i));
  }
  uint64_t end = std::chrono::duration_cast<std::chrono::seconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
  uint64_t cost = end - begin;
  std::cout << "R-B Tree insert time cost:\t" << cost << "s" << '\n';
}

int main(int /*argc*/, char** /*argv*/) {
  // result_test(100, 1000, 100);
  // result_test(1000, 10000, 1000);
  // result_test(10000, 100000, 10000);
  // result_test(100000, 1000000, 100000);
  // result_test(1000000, 10000000, 500000);

  // B+Tree test
  // TENMILLIONTEST();
  // Red-Black Tree Test
  // ten_million_red_black_tree_test();
  auto* tree = new BPlusTree<int, uint64_t>();
  for (int i = 1; i <= 20; ++i) {
    tree->tree_insert(i, i);
  }

  for (int i = 1; i <= 20; ++i) {
    tree->tree_delete(i);
  }

  delete tree;
  return 0;
}