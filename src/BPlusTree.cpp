#include "../include/BPlusTree.h"

#include <cstdint>
#include <iostream>

template <typename K, typename T>
Node<K, T>::Node(int m)
    : degree(m), keys(new K[static_cast<int64_t>(count * 2)]) {}

template <typename K, typename T>
Node<K, T>::~Node() {
  delete (keys);
}
template <typename K, typename T>
Node<K, T>* Node<K, T>::getclasstype() {
  return this;
}

// Leaf Node
template <typename K, typename T>
LeafNode<K, T>::LeafNode(int m)
    : prev(nullptr),
      next(nullptr),
      Node<K, T>(m),
      data(new T[this->degree * 2]) {}

template <typename K, typename T>
LeafNode<K, T>::~LeafNode() {
  prev = nullptr;
  next = nullptr;
}
template <typename K, typename T>
LeafNode<K, T>* LeafNode<K, T>::getclasstype() {
  return this;
}

template <typename K, typename T>
void LeafNode<K, T>::print_leaf() {
  for (int i = 0; i < this->count; i++) {
    std::cout << "key:\t" << this->keys[i] << "\tdata\t" << this->data[i]
              << "\n";
  }
}

// Inner Node
template <typename K, typename T>
InnerNode<K, T>::InnerNode(int m)
    : Node<K, T>(m), child(new LeafNode<K, T>*[this->degree * 2 + 1]) {
  for (auto p : child) {
    p = nullptr;
  }
}

template <typename K, typename T>
InnerNode<K, T>* InnerNode<K, T>::getclasstype() {
  return this;
}

template <typename K, typename T>
void InnerNode<K, T>::print_inner() {
  for (int i = 0; i < this->count; i++) {
    std::cout << "key:\t" << this->keys[i] << "\n";
  }
  for (Node<K, T>* p_child : child) {
    if (typeid(*(p_child->getclasstype())) == typeid(LeafNode<K, T>)) {
      p_child->print_leaf();
    } else {
      p_child->print_inner();
    }
  }
}

template <typename K, typename T>
BPlusTree<K, T>::BPlusTree() : root_(nullptr) {}

template <typename K, typename T>
BPlusTree<K, T>::~BPlusTree() = default;

// Search

/* 在节点中寻找大于等于key的最小键值的位置*/
template <typename K, typename T>
int Node<K, T>::find_last_pos(K key) {
  int low = 0;
  int high = this->count - 1;
  int mid = 0;
  while (low < high) {
    mid = (low + high + 1) / 2;
    if (this->keys[mid] >= key) {
      high = mid - 1;
    } else {
      low = mid;
    }
  }
  return high;
}

template <typename K, typename T>
T BPlusTree<K, T>::search(K key) {
  Node<K, T>* pnode = this->root_;
  int position = 0;

  while (typeid(*(pnode->getclasstype())) != typeid(LeafNode<K, T>)) {
    position = pnode->find_last_pos(key);
    // 如果是key = pnode->key[i], 指向pos对应的下一个子节点; 否则,
    // 指向该pos对应的子节点
    if (pnode->keys[position] == key) {
      pnode = static_cast<InnerNode<K, T>*>(pnode)->child[position + 1];
    } else {
      pnode = static_cast<InnerNode<K, T>*>(pnode)->child[position];
    }
  }
  // 此时pnode为 leaf node
  if (pnode->keys[position] == key) {
    return static_cast<LeafNode<K, T>*>(pnode)->data[position];
  }
  return NULL;
}

template <typename K, typename T>
std::vector<T> BPlusTree<K, T>::search_range(K begin, K end) {
  std::vector<T> vec;
  Node<K, T>* pnode = this->root_;
  int position = 0;

  while (typeid(*(pnode->getclasstype())) != typeid(LeafNode<K, T>)) {
    position = pnode->find_last_pos(begin);
    // 如果是key = pnode->key[i], 指向pos对应的下一个子节点; 否则,
    // 指向该pos对应的子节点
    if (pnode->keys[position] == begin) {
      pnode = static_cast<InnerNode<K, T>*>(pnode)->child[position + 1];
    } else {
      pnode = static_cast<InnerNode<K, T>*>(pnode)->child[position];
    }
  }

  // leaf node寻找不小于begin的最小的pos
  position = pnode->find_last_pos(begin);
  // TODO(lzy): String 比较是否可行
  if (pnode->keys[position] < begin) {
    pnode = pnode->next;
    if (pnode == nullptr) {
      return vec;
    }
    // 进入新节点position从第一个开始计算
    position = 0;
  }

  for (int i = position; i < pnode->count && pnode->keys[i] <= end; ++i) {
    // 提前结束遍历过程
    if (pnode->keys[i] > end) {
      return vec;
    }
    vec.push_back(pnode->data[i]);
  }

  while (pnode->keys[pnode->count - 1] <= end && pnode->next != nullptr) {
    pnode = pnode->next;
    // 提前结束遍历过程
    if (pnode->keys[0] > end) {
      return vec;
    }
    for (int i = 0; i < pnode->count && pnode->keys[i] <= end; ++i) {
      // 提前结束遍历过程
      if (pnode->keys[i] > end) {
        return vec;
      }
      vec.push_back(pnode->data[i]);
    }
  }
  return vec;
}
