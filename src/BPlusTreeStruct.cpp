#include "../include/BPlusTree.h"

#include <cstddef>
#include <iostream>

template <typename K, typename T>
Node<K, T>::Node() : parent(nullptr) {}

template <typename K, typename T>
Node<K, T>::~Node() = default;

template <typename K, typename T>
Node<K, T>* Node<K, T>::getclasstype() {
  return this;
}

// Leaf Node
template <typename K, typename T>
LeafNode<K, T>::LeafNode(LeafNode<K, T>* prev_node, LeafNode<K, T>* next_node)
    : Node<K, T>(), prev(prev_node), next(next_node) {
  this->is_leaf = true;
  if (next_node) {
    next_node->prev = this;
  }

  if (prev_node) {
    prev_node->next = this;
  }
}

template <typename K, typename T>
LeafNode<K, T>::~LeafNode() {
  if (this->prev) {
    delete prev;
  }
  if (this->prev) {
    delete next;
  }
  prev = nullptr;
  next = nullptr;
}

template <typename K, typename T>
LeafNode<K, T>* LeafNode<K, T>::getclasstype() {
  return this;
}

// Inner Node
template <typename K, typename T>
InnerNode<K, T>::InnerNode() : Node<K, T>() {
  this->is_leaf = false;
}

template <typename K, typename T>
InnerNode<K, T>* InnerNode<K, T>::getclasstype() {
  return this;
}

template <typename K, typename T>
int InnerNode<K, T>::child_index(K key) {
  for (int i = 0; i < this->keys.size(); i++) {
    if (key < this->keys[i]) {
      return i;
    }
  }
  return this->keys.size();
}

template <typename K, typename T>
BPlusTree<K, T>::BPlusTree()
    : root_(new LeafNode<K, T>()),
      depth_(0),
      maxcap_(MAX_DEGREE - 1),
      mincap_(maxcap_ / 2) {}

template <typename K, typename T>
BPlusTree<K, T>::~BPlusTree() {
  if (this->root_) {
    delete this->root_;
  }
};

// 帮助函数

// 寻找叶子节点
template <typename K, typename T>
Node<K, T>* BPlusTree<K, T>::find_leaf(int key) {
  Node<K, T>* p_node = this->root_;
  while (!p_node->is_leaf) {
    int flag = 0;
    for (int i = 0; i < p_node->keys.size(); ++i) {
      if (p_node->keys[i] > key) {
        p_node = static_cast<InnerNode<K, T>*>(p_node)->child[i];
        flag = 1;
        break;
      }
      // TODO(lzy): 验证正确性
      if (p_node->keys[i] == key) {
        p_node = static_cast<InnerNode<K, T>*>(p_node)->child[i + 1];
        flag = 1;
        break;
      }
    }
    // 没找到，直接指向最右侧的孩子节点
    if (!flag) {
      p_node =
          static_cast<InnerNode<K, T>*>(p_node)->child[p_node->keys.size()];
    }
  }
  return p_node;
}

// 显示B+树
template <typename K, typename T>
void BPlusTree<K, T>::show_bplustree() {
  if (this->root_->keys.size() == 0) {
    std::cout << "[]"
              << "\n";
    return;
  }
  std::vector<Node<K, T>*> q;
  q.push_back(root_);
  while (!q.empty()) {
    uint64_t size = q.size();
    for (int i = 0; i < size; ++i) {
      if (!q[i]->is_leaf) {
        for (int j = 0; j < (static_cast<InnerNode<K, T>*>(q[i]))->child.size();
             ++j) {
          q.push_back(static_cast<InnerNode<K, T>*>(q[i])->child[j]);
        }
      }
      std::cout << "[";
      int nk = 0;
      for (nk = 0; nk < q[i]->keys.size() - 1; ++nk) {
        std::cout << q[i]->keys[nk] << ",";
      }
      std::cout << q[i]->keys[nk] << "] ";
    }
    q.erase(q.begin(), q.begin() + size);
    std::cout << "\n";
  }
}
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

// 寻找key所在的位置
template <typename K, typename T>
int Node<K, T>::key_index(K key) {
  for (int i = 0; i < this->keys.size(); ++i) {
    if (key == this->keys[i]) {
      return i;
    }
  }
  return -1;
}

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
