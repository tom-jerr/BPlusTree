
#include <iostream>

#include "../include/CLBPlusTree.h"
// Insert Node to Tree

// insert <key, value> to leaf node
template <typename K, typename T>
bool LeafNode<K, T>::insert_leaf(K key, T value) {
  int i = 0;
  if (this->keys.empty()) {
    this->keys.push_back(key);
    this->data.push_back(value);
    return true;
  }
  for (; (key > this->keys[i]) && (i < this->keys.size()); ++i) {
  }
  this->keys.insert(this->keys.begin() + i, key);
  this->data.insert(this->data.begin() + i, value);
  return true;
}

// insert <p_node, key> to inner node
template <typename K, typename T>
bool InnerNode<K, T>::insert_inner(std::vector<Node<K, T>*> p_node, K key) {
  int i = 0;
  for (; (key > this->keys[i]) && (i < this->keys.size()); ++i) {
  }
  this->keys.insert(this->keys.begin() + i, key);
  // 将孩子指针修改；其父亲指针指向原来节点
  this->child.erase(this->child.begin() + i);
  this->child.insert(this->child.begin() + i, p_node.begin(), p_node.end());
  for (auto* node : p_node) {
    node->parent = this;
  }

  return true;
}

// when keys of leaf node attend to 2*degree-1; it will split
template <typename K, typename T>
std::tuple<Node<K, T>*, Node<K, T>*, K> LeafNode<K, T>::split_leaf() {
  auto* p_node = new LeafNode<K, T>(prev, this);
  p_node->parent = this->parent;

  int mid = this->keys.size() / 2;
  // 将一半的keys给另一个新节点
  p_node->keys = std::vector<K>(this->keys.begin(), this->keys.begin() + mid);
  p_node->data = std::vector<T>(this->data.begin(), this->data.begin() + mid);

  this->keys.erase(this->keys.begin(), this->keys.begin() + mid);
  this->data.erase(this->data.begin(), this->data.begin() + mid);

  return std::make_tuple(p_node, this, this->keys[0]);
}

template <typename K, typename T>
std::tuple<Node<K, T>*, Node<K, T>*, K> InnerNode<K, T>::split_inner() {
  auto* p_node = new InnerNode<K, T>();
  p_node->parent = this->parent;

  int mid = this->keys.size() / 2;
  copy(this->keys.begin(), this->keys.begin() + mid,
       back_inserter(p_node->keys));
  copy(this->child.begin(), this->child.begin() + mid + 1,
       back_inserter(p_node->child));

  // 将父亲节点进行赋值
  for (auto* ch : p_node->child) {
    ch->parent = p_node;
  }
  K key = this->keys[mid];
  this->keys.erase(this->keys.begin(), this->keys.begin() + mid + 1);
  this->child.erase(this->child.begin(), this->child.begin() + mid + 1);

  return std::make_tuple(p_node, this, key);
}

template <typename K, typename T>
void BPlusTree<K, T>::insert_in_parent(
    std::tuple<Node<K, T>*, Node<K, T>*, K> result) {
  int key = std::get<2>(result);
  Node<K, T>* left = std::get<0>(result);
  Node<K, T>* right = std::get<1>(result);
  // old_node是根节点
  if (right->parent == nullptr) {
    Node<K, T>* new_root = new InnerNode<K, T>();
    this->depth_ += 1;
    new_root->keys.push_back(key);
    static_cast<InnerNode<K, T>*>(new_root)->child = {left, right};
    // static_cast<InnerNode<K, T>*>(new_root)->child.push_back(new_node);
    this->root_ = new_root;
    left->parent = this->root_;
    right->parent = this->root_;
    return;
  }

  Node<K, T>* parent = right->parent;
  bool op_inner =
      static_cast<InnerNode<K, T>*>(parent)->insert_inner({left, right}, key);
  if (!op_inner) {
    std::cout << "failed to insert inner"
              << "\n";
  }
  if (parent->keys.size() > this->maxcap_) {
    std::tuple<Node<K, T>*, Node<K, T>*, K> new_inner =
        static_cast<InnerNode<K, T>*>(parent)->split_inner();
    insert_in_parent(new_inner);
  }
}

template <typename K, typename T>
void BPlusTree<K, T>::tree_insert(K key, T data) {
  Node<K, T>* p_node = this->root_;
  bool op_leaf = false;
  // 如果此时叶子节点没有元素，直接插入<key, value>
  if (p_node->keys.empty()) {
    op_leaf = static_cast<LeafNode<K, T>*>(p_node)->insert_leaf(key, data);
    if (!op_leaf) {
      std::cout << "failed to insert leaf"
                << "\n";
    }
    return;
  }
  p_node = find_leaf(key);

  // 此时为叶子节点
  op_leaf = static_cast<LeafNode<K, T>*>(p_node)->insert_leaf(key, data);
  if (!op_leaf) {
    std::cout << "failed to insert leaf"
              << "\n";
  }
  if (p_node->keys.size() > this->maxcap_) {
    std::tuple<Node<K, T>*, Node<K, T>*, K> new_node =
        static_cast<LeafNode<K, T>*>(p_node)->split_leaf();
    insert_in_parent(new_node);
  }
}