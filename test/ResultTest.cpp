#include <fcntl.h>

#include <chrono>
#include <cstdint>
#include <fstream>
#include <ios>

#include "../include/CLBPlusTree.h"

#define TENMILLION 10000000
#define OPEN_CODE 00700

#define TEST_DEGREE_BEGIN 3
#define TEST_DEGREE_END 500
void insert_tenmillion(int degree) {
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
  std::cout << degree << " degree B+ Tree insert time cost:\t" << cost << "ms"
            << '\n';
  std::fstream f;
  f.open("../../doc/analysis/degree_test.txt", std::ios::out | std::ios::app);
  std::fstream data;
  data.open("../../doc/analysis/degree_data.txt",
            std::ios::out | std::ios::app);
  f << degree << " degree B+ Tree insert time cost:\t" << cost << "ms" << '\n';
  data << cost << ",";
  f.close();
  data.close();
  delete tree;
}

void degree_test() {
  for (int i = TEST_DEGREE_BEGIN; i <= TEST_DEGREE_END; i++) {
    insert_tenmillion(i);
  }
}

int main(int /*argc*/, char** /*argv*/) {
  degree_test();
  // auto* tree = new BPlusTree<int, uint64_t>(5);
  // for (int i = 1; i <= 13; ++i) {
  //   tree->tree_insert(i, i);
  // }
  // tree->show_bplustree();
  return 0;
}