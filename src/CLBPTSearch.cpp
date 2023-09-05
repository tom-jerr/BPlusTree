#include <cstddef>
#include <iostream>

#include "../include/CLBPlusTree.h"
// Search

template <typename K, typename T>
T BPlusTree<K, T>::search(K key) {
  Node<K, T>* leaf = this->find_leaf(key);
  int index = leaf->key_index(key);
  if (index == -1) {
    std::cout << "key " << key << " not found" << std::endl;
    return NULL;
  }
  return static_cast<LeafNode<K, T>*>(leaf)->data[index];
}

template <typename K, typename T>
std::vector<T> BPlusTree<K, T>::search_range(K begin, K end) {
  std::vector<T> vec;
  Node<K, T>* leaf = this->find_leaf(begin);
  int position = leaf->find_last_pos(begin);
  position = leaf->find_last_pos(begin);
  // TODO(lzy): String 比较是否可行
  if (leaf->keys[position] < begin) {
    leaf = leaf->next;
    if (leaf == nullptr) {
      return vec;
    }
    // 进入新节点position从第一个开始计算
    position = 0;
  }

  // 当前节点中符合条件的值
  for (int i = position; i < leaf->keys.size() && leaf->keys[i] <= end; ++i) {
    // 提前结束遍历过程
    if (leaf->keys[i] > end) {
      return vec;
    }
    vec.push_back(leaf->data[i]);
  }

  // 后面节点中符合条件的值
  while (leaf->keys[leaf->keys.size() - 1] <= end && leaf->next != nullptr) {
    leaf = leaf->next;
    // 提前结束遍历过程
    if (leaf->keys[0] > end) {
      return vec;
    }
    for (int i = 0; i < leaf->keys.size() && leaf->keys[i] <= end; ++i) {
      // 提前结束遍历过程
      if (leaf->keys[i] > end) {
        return vec;
      }
      vec.push_back(leaf->data[i]);
    }
  }
  return vec;
}