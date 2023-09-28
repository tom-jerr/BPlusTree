#include <fcntl.h>

#include <chrono>
#include <cstdint>
#include <fstream>
#include <ios>
#include <map>
#include <utility>

#include "../include/CLBPlusTree.h"

#define TENMILLION 10000000
#define TEST_DEGREE20 400
#define TEST_DEGREE16 404
#define TEST_DEGREE18 398
#define TEST_ROUND 10

void rb_tree_test() {
  uint64_t begin_rb = std::chrono::duration_cast<std::chrono::milliseconds>(
                          std::chrono::system_clock::now().time_since_epoch())
                          .count();
  std::map<int, uint64_t> rbtree;
  for (int i = 1; i <= TENMILLION; ++i) {
    rbtree.insert(std::make_pair(i, i));
  }
  uint64_t end_rb = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch())
                        .count();
  uint64_t cost_rb = end_rb - begin_rb;
  std::cout << "RB Tree insert time cost:\t" << cost_rb << "ms" << '\n';
  std::fstream f;
  f.open("../../doc/analysis/compare_test.txt", std::ios::out | std::ios::app);
  f << " RB Tree insert time cost:\t" << cost_rb << "ms" << '\n';
  f.close();

  std::fstream data;
  data.open("../../doc/analysis/rb_test_data.txt",
            std::ios::out | std::ios::app);
  data << cost_rb << ',';
  data.close();
  rbtree.clear();
}

void b_plus_tree_test(int degree) {
  auto* tree20 = new BPlusTree<int, uint64_t>(degree);

  uint64_t begin = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
  for (int i = 1; i <= TENMILLION; ++i) {
    tree20->tree_insert(i, i);
  }
  uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
  uint64_t cost = end - begin;
  std::cout << "B+ Tree degree " << degree << " insert time cost:\t" << cost
            << "ms" << '\n';
  std::fstream f;
  f.open("../../doc/analysis/compare_test.txt", std::ios::out | std::ios::app);
  f << degree << " degree B+ Tree insert time cost:\t" << cost << "ms" << '\n';
  f.close();

  std::fstream data;
  std::fstream data1;
  std::fstream data2;
  switch (degree) {
    case TEST_DEGREE16:
      data.open("../../doc/analysis/bplustree_test_data16.txt",
                std::ios::out | std::ios::app);
      data << cost << ',';
      data.close();
      break;
    case TEST_DEGREE18:
      data1.open("../../doc/analysis/bplustree_test_data18.txt",
                 std::ios::out | std::ios::app);
      data1 << cost << ',';
      data1.close();
      break;
    case TEST_DEGREE20:
      data2.open("../../doc/analysis/bplustree_test_data20.txt",
                 std::ios::out | std::ios::app);
      data2 << cost << ',';
      data2.close();
      break;
    default:
      break;
  };

  delete tree20;
}

void compare_test() {
  rb_tree_test();
  b_plus_tree_test(TEST_DEGREE16);
  b_plus_tree_test(TEST_DEGREE18);
  b_plus_tree_test(TEST_DEGREE20);
}

int main(int /*argc*/, char** /*argv*/) {
  for (int i = 0; i < TEST_ROUND; ++i) {
    std::cout << "round " << i + 1 << '\n';
    std::fstream f;
    f.open("../../doc/analysis/compare_test.txt",
           std::ios::out | std::ios::app);
    f << "round " << i + 1 << '\n';
    f.close();
    // B+树和红黑树比较测试
    compare_test();

    f.open("../../doc/analysis/compare_test.txt",
           std::ios::out | std::ios::app);
    f << '\n';
    f.close();
    std::cout << '\n';
  }
  return 0;
}