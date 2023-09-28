#include <cstdint>
// #include <iostream>
#include <chrono>

#include "../include/CLBPlusTree.h"

#define DGREE_18 493
#define DGREE_20 400
#define TENMILLION 10000000

void TENMILLIONTEST(int degree) {
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
  std::cout << degree << " B+ tree insert time cost:\t" << cost << "ms" << '\n';

  begin = std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch())
              .count();
  for (int i = 1; i <= TENMILLION; ++i) {
    tree->search(i);
  }
  end = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count();
  cost = end - begin;
  std::cout << "search time cost:\t" << cost << "ms" << '\n';

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
  std::cout << "delete time cost:\t" << cost << "ms" << '\n';
}

int main(int /*argc*/, char** /*argv*/) {
  TENMILLIONTEST(DGREE_18);
  TENMILLIONTEST(DGREE_20);

  return 0;
}