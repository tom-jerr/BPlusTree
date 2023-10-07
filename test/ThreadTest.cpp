#include <fcntl.h>

#include <chrono>
#include <cstdint>
#include <fstream>
#include <ios>
#include <thread>
#include <vector>

#include "../include/CLBPlusTree.h"

#define TENMILLION 10000000
#define OPEN_CODE 00700

void thread_insert_base(int begin, int end, BPlusTree<int, uint64_t>* tree) {
  for (int i = begin; i <= end; ++i) {
    tree->tree_insert(i, i);
  }
}

void insert_tenmillion(int thread_num) {
  auto* tree = new BPlusTree<int, uint64_t>(400);
  std::vector<std::thread> threads;
  for (int i = 0; i < thread_num - 1; ++i) {
    threads.push_back(std::thread(
        thread_insert_base, i * TENMILLION / thread_num + 1,
        i * TENMILLION / thread_num + TENMILLION / thread_num, tree));
  }
  threads.push_back(std::thread(thread_insert_base,
                                (thread_num - 1) * TENMILLION / thread_num + 1,
                                TENMILLION, tree));
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
  std::cout << thread_num << " multi B+ Tree insert time cost:\t" << cost
            << "ms" << '\n';
  std::fstream f;
  f.open("../../doc/analysis/thread_test.txt", std::ios::out | std::ios::app);
  std::fstream data;
  data.open("../../doc/analysis/thread_data.txt",
            std::ios::out | std::ios::app);
  f << thread_num << " B+ Tree insert time cost:\t" << cost << "ms" << '\n';
  data << cost << ",";
  f.close();
  data.close();
  delete tree;
}

void thread_test() {
  for (int i = 30; i <= 100; i++) {
    insert_tenmillion(i);
  }
}

int main(int /*argc*/, char** /*argv*/) {
  thread_test();
  return 0;
}