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
