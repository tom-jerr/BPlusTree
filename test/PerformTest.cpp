#include <cstdint>
// #include <iostream>
#include <chrono>

#include "../include/CLBPlusTree.h"

#define TENTHOUSAND 10000
#define HUNTHOUSAND 100000
#define MILLION 1000000
#define TENMILLION 10000000
void TENSOUSANDTEST() {
  auto* tree = new BPlusTree<int, uint64_t>();

  uint64_t begin = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
  for (int i = 1; i <= TENTHOUSAND; ++i) {
    tree->tree_insert(i, i);
  }
  uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
  uint64_t cost = end - begin;
  std::cout << "time cost:\t" << cost << "ms" << '\n';
  // tree->show_bplustree();
}

void HUNTHOUSANDTEST() {
  auto* tree = new BPlusTree<int, uint64_t>();

  uint64_t begin = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
  for (int i = 1; i <= HUNTHOUSAND; ++i) {
    tree->tree_insert(i, i);
  }
  uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
  uint64_t cost = end - begin;
  std::cout << "time cost:\t" << cost << "ms" << '\n';
  // tree->show_bplustree();
}

void MILLIONTEST() {
  auto* tree = new BPlusTree<int, uint64_t>();

  uint64_t begin = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
  for (int i = 1; i <= MILLION; ++i) {
    tree->tree_insert(i, i);
  }
  uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
  uint64_t cost = end - begin;
  std::cout << "time cost:\t" << cost << "ms" << '\n';
  // tree->show_bplustree();
}

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
  std::cout << "insert time cost:\t" << cost << "s" << '\n';
  // tree->show_bplustree();
  begin = std::chrono::duration_cast<std::chrono::seconds>(
              std::chrono::system_clock::now().time_since_epoch())
              .count();
  for (int i = 1; i <= TENMILLION; ++i) {
    tree->search(i);
  }
  end = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count();
  cost = end - begin;
  std::cout << "search time cost:\t" << cost << "s" << '\n';

  begin = std::chrono::duration_cast<std::chrono::seconds>(
              std::chrono::system_clock::now().time_since_epoch())
              .count();
  for (int i = 1; i <= TENMILLION; ++i) {
    tree->tree_delete(i);
  }
  end = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count();
  cost = end - begin;
  std::cout << "delete time cost:\t" << cost << "s" << '\n';
}
void ten_million_search() {
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
  std::cout << "insert time cost:\t" << cost << "s" << '\n';
  tree->show_bplustree();
  begin = std::chrono::duration_cast<std::chrono::seconds>(
              std::chrono::system_clock::now().time_since_epoch())
              .count();
  for (int i = 1; i <= TENMILLION; ++i) {
    tree->search(i);
  }
  end = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count();
  cost = end - begin;
  std::cout << "search time cost:\t" << cost << "s" << '\n';
  delete tree;
}
int main(int /*argc*/, char** /*argv*/) {
  // TENSOUSANDTEST();
  // HUNTHOUSANDTEST();
  // MILLIONTEST();
  TENMILLIONTEST();
  // ten_million_search();
  return 0;
}