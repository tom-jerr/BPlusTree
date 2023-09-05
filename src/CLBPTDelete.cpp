#include <iostream>

#include "../include/CLBPlusTree.h"
// Delete
// 每次改变叶子节点的key，其父亲节点的key也需要进行对应改变

// 叶子节点删除key
template <typename K, typename T>
bool LeafNode<K, T>::delete_leaf(K key) {
  int index = this->key_index(key);
  if (index == -1) {
    std::cout << "Key " << key << " not found! Exiting ..." << std::endl;
    return false;
  }

  this->keys.erase(this->keys.begin() + index);
  this->data.erase(this->data.begin() + index);
  if (this->parent) {
    int child_index =
        static_cast<InnerNode<K, T>*>(this->parent)->child_index(key);
    // 父节点的key删除后改变（key的index比child的index小1）
    if (child_index) {
      this->parent->keys[child_index - 1] = this->keys.front();
    }
  }
  return true;
}

// 内部节点删除key
template <typename K, typename T>
bool InnerNode<K, T>::delete_inner(K key) {
  int index = this->key_index(key);
  if (index != -1) {
    Node<K, T>* left_leaf = this->child[index + 1];
    while (!left_leaf->is_leaf) {
      left_leaf = static_cast<InnerNode<K, T>*>(left_leaf)->child.front();
    }
    this->keys[index] = left_leaf->keys.front();
  }
  return true;
}

// BPlusTree借用节点

// 从右边叶子节点借第一个一个数据
template <typename K, typename T>
void BPlusTree<K, T>::borrow_right_leaf(Node<K, T>* node, Node<K, T>* next) {
  node->keys.push_back(next->keys.front());
  next->keys.erase(next->keys.begin());
  static_cast<LeafNode<K, T>*>(node)->data.push_back(
      static_cast<LeafNode<K, T>*>(next)->data.front());
  static_cast<LeafNode<K, T>*>(next)->data.erase(
      static_cast<LeafNode<K, T>*>(next)->data.begin());

  for (int i = 0; i < static_cast<InnerNode<K, T>*>(node->parent)->child.size();
       ++i) {
    if (static_cast<InnerNode<K, T>*>(node->parent)->child[i] == next) {
      node->parent->keys[i - 1] = next->keys.front();
      break;
    }
  }
}

// 从左边叶子节点中借最后一个数据; node父节点对应的key需要改变
template <typename K, typename T>
void BPlusTree<K, T>::borrow_left_leaf(Node<K, T>* node, Node<K, T>* prev) {
  node->keys.insert(node->keys.begin(), prev->keys.back());
  prev->keys.erase(prev->keys.end() - 1);
  static_cast<LeafNode<K, T>*>(node)->data.insert(
      static_cast<LeafNode<K, T>*>(node)->data.begin(),
      static_cast<LeafNode<K, T>*>(prev)->data.back());
  static_cast<LeafNode<K, T>*>(prev)->data.erase(
      static_cast<LeafNode<K, T>*>(prev)->data.begin());

  for (int i = 0; i < static_cast<InnerNode<K, T>*>(node->parent)->child.size();
       ++i) {
    if (static_cast<InnerNode<K, T>*>(node->parent)->child[i] == node) {
      node->parent->keys[i - 1] = node->keys.front();
      break;
    }
  }
}

// 借用inner node; pos是node在父节点的child中的index
// 插入node节点的父节点的key，父节点得到next的第一个key
// node.key = next.keys.begin() 使得满足node.key < node.parent.key, after
// node.key >= node.parent.key
template <typename K, typename T>
void BPlusTree<K, T>::borrow_right_inner(int pos, Node<K, T>* node,
                                         Node<K, T>* next) {
  node->keys.insert(node->keys.end(), node->parent->keys[pos]);
  node->parent->keys[pos] = next->keys.front();
  next->keys.erase(next->keys.begin());

  static_cast<InnerNode<K, T>*>(node)->child.insert(
      static_cast<InnerNode<K, T>*>(node)->child.end(),
      static_cast<InnerNode<K, T>*>(next)->child.front());
  static_cast<InnerNode<K, T>*>(next)->child.erase(
      static_cast<InnerNode<K, T>*>(next)->child.begin());
  // 改变父亲节点
  static_cast<InnerNode<K, T>*>(node)->child.back()->parent = node;
}

// 插入node节点的父节点的key，父节点得到prev的最后一个key
// node.parent.key = prev.keys.back() 使得满足prev.key < node.parent.key, after
// node.key >= node.parent.key
template <typename K, typename T>
void BPlusTree<K, T>::borrow_left_inner(int pos, Node<K, T>* node,
                                        Node<K, T>* prev) {
  node->keys.insert(node->keys.begin(), node->parent->keys[pos - 1]);
  node->parent->keys[pos - 1] = prev->keys.back();
  prev->keys.erase(prev->keys.end() - 1);

  static_cast<InnerNode<K, T>*>(node)->child.insert(
      static_cast<InnerNode<K, T>*>(node)->child.end(),
      static_cast<InnerNode<K, T>*>(prev)->child.back());
  static_cast<InnerNode<K, T>*>(prev)->child.erase(
      static_cast<InnerNode<K, T>*>(prev)->child.end() - 1);
  // 改变父亲节点
  static_cast<InnerNode<K, T>*>(node)->child.front()->parent = node;
}

// 节点合并
// 合并叶子节点后，叶子节点父节点对应的key和child也需要删除
template <typename K, typename T>
void BPlusTree<K, T>::merge_right_leaf(Node<K, T>* node, Node<K, T>* next) {
  node->keys.insert(node->keys.end(), next->keys.begin(), next->keys.end());
  static_cast<LeafNode<K, T>*>(node)->data.insert(
      static_cast<LeafNode<K, T>*>(node)->data.end(),
      static_cast<LeafNode<K, T>*>(next)->data.begin(),
      static_cast<LeafNode<K, T>*>(next)->data.end());
  // 链表拆下next节点
  static_cast<LeafNode<K, T>*>(node)->next =
      static_cast<LeafNode<K, T>*>(next)->next;
  if (static_cast<LeafNode<K, T>*>(node)->next) {
    static_cast<LeafNode<K, T>*>(next)->next->prev =
        static_cast<LeafNode<K, T>*>(node);
  }
  static_cast<LeafNode<K, T>*>(node)->next = nullptr;
  static_cast<LeafNode<K, T>*>(node)->prev = nullptr;

  // 处理next父节点中需要删除的key和child
  for (int i = 0; i < static_cast<InnerNode<K, T>*>(next->parent)->child.size();
       ++i) {
    if (static_cast<InnerNode<K, T>*>(next->parent)->child[i] == next) {
      next->parent->keys.erase(next->parent->keys.begin() + i - 1);
      static_cast<InnerNode<K, T>*>(next->parent)
          ->child.erase(
              static_cast<InnerNode<K, T>*>(next->parent)->child.begin() + i);
      break;
    }
  }
}

// 将node合并到prev中
template <typename K, typename T>
void BPlusTree<K, T>::merge_left_leaf(Node<K, T>* node, Node<K, T>* prev) {
  prev->keys.insert(prev->keys.end(), node->keys.begin(), node->keys.end());
  static_cast<LeafNode<K, T>*>(prev)->data.insert(
      static_cast<LeafNode<K, T>*>(prev)->data.end(),
      static_cast<LeafNode<K, T>*>(node)->data.begin(),
      static_cast<LeafNode<K, T>*>(node)->data.end());

  static_cast<LeafNode<K, T>*>(prev)->next =
      static_cast<LeafNode<K, T>*>(node)->next;
  if (static_cast<LeafNode<K, T>*>(prev)->next) {
    static_cast<LeafNode<K, T>*>(prev)->next->prev =
        static_cast<LeafNode<K, T>*>(node);
  }
  // 处理node父节点中需要删除的key和child
  for (int i = 0; i < static_cast<InnerNode<K, T>*>(node->parent)->child.size();
       ++i) {
    if (static_cast<InnerNode<K, T>*>(node->parent)->child[i] == node) {
      node->parent->keys.erase(node->parent->keys.begin() + i - 1);
      static_cast<InnerNode<K, T>*>(node->parent)
          ->child.erase(
              static_cast<InnerNode<K, T>*>(node->parent)->child.begin() + i);
      break;
    }
  }
}

// 两个inner合并时，需要把父节点中两个子节点中间的key也插入
// 父节点需要删除对应的key和child
template <typename K, typename T>
void BPlusTree<K, T>::merge_right_inner(int pos, Node<K, T>* node,
                                        Node<K, T>* next) {
  node->keys.insert(node->keys.end(), node->parent->keys[pos]);
  node->parent->keys.erase(node->parent->keys.begin() + pos);
  static_cast<InnerNode<K, T>*>(node->parent)
      ->child.erase(static_cast<InnerNode<K, T>*>(node->parent)->child.begin() +
                    pos + 1);

  // 插入合并的值
  node->keys.insert(node->keys.end(), next->keys.begin(), next->keys.end());
  static_cast<InnerNode<K, T>*>(node)->child.insert(
      static_cast<InnerNode<K, T>*>(node)->child.end(),
      static_cast<InnerNode<K, T>*>(next)->child.begin(),
      static_cast<InnerNode<K, T>*>(next)->child.end());
  for (Node<K, T>* child : static_cast<InnerNode<K, T>*>(node)->child) {
    child->parent = node;
  }
}

template <typename K, typename T>
void BPlusTree<K, T>::merge_left_inner(int pos, Node<K, T>* node,
                                       Node<K, T>* prev) {
  prev->keys.insert(prev->keys.end(), node->parent->keys[pos - 1]);
  node->parent->keys.erase(node->parent->keys.begin() + pos - 1);
  static_cast<InnerNode<K, T>*>(node->parent)
      ->child.erase(static_cast<InnerNode<K, T>*>(node->parent)->child.begin() +
                    pos);

  // 插入合并的值
  prev->keys.insert(prev->keys.end(), node->keys.begin(), node->keys.end());
  static_cast<InnerNode<K, T>*>(prev)->child.insert(
      static_cast<InnerNode<K, T>*>(prev)->child.end(),
      static_cast<InnerNode<K, T>*>(node)->child.begin(),
      static_cast<InnerNode<K, T>*>(node)->child.end());
  for (Node<K, T>* child : static_cast<InnerNode<K, T>*>(prev)->child) {
    child->parent = prev;
  }
}

// 整个树的删除键过程
template <typename K, typename T>
void BPlusTree<K, T>::tree_delete(K key, Node<K, T>* node) {
  if (node == nullptr) {
    node = this->find_leaf(key);
  }
  // 正常删除叶子节点上的Key和inner node上的key
  if (node->is_leaf) {
    bool op_leaf = static_cast<LeafNode<K, T>*>(node)->delete_leaf(key);
    if (!op_leaf) {
      std::cout << "failed to delete leaf"
                << "\n";
    }
  } else {
    bool op_inner = static_cast<InnerNode<K, T>*>(node)->delete_inner(key);
    if (!op_inner) {
      std::cout << "failed to delete inner"
                << "\n";
    }
  }

  // 如果此时的Key大小小于m/2，进行节点的借出和合并
  if (node->keys.size() < this->mincap_) {
    // 唯一出口点，当node是root时结束；否则继续进行删除直到进行到根节点
    if (node == this->root_) {
      if (this->root_->keys.empty() &&
          !static_cast<InnerNode<K, T>*>(this->root_)->child.empty()) {
        this->root_ = static_cast<InnerNode<K, T>*>(this->root_)->child[0];
        this->root_->parent = nullptr;
        this->depth_ -= 1;
      }
      return;
    }
    // 如果node是叶子节点，判断是否需要进行节点合并和借出节点
    if (node->is_leaf) {
      Node<K, T>* next = static_cast<LeafNode<K, T>*>(node)->next;
      Node<K, T>* prev = static_cast<LeafNode<K, T>*>(node)->prev;
      // 如果此时节点>=mincap_，可以借出一个节点
      if (next && next->parent == node->parent &&
          next->keys.size() > this->mincap_) {
        this->borrow_right_leaf(node, next);
      } else if (prev && prev->parent == node->parent &&
                 prev->keys.size() > this->mincap_) {
        this->borrow_left_leaf(node, prev);
      }
      // 如果此时节点<=mincap_，只能进行两个节点的合并
      else if (next && next->parent == node->parent &&
               next->keys.size() <= this->mincap_) {
        this->merge_right_leaf(node, next);
      } else if (prev && prev->parent == node->parent &&
                 prev->keys.size() <= this->mincap_) {
        this->merge_left_leaf(node, prev);
      }
    }

    else {
      int pos = -1;
      for (int i = 0;
           i < static_cast<InnerNode<K, T>*>(node->parent)->child.size(); ++i) {
        if (static_cast<InnerNode<K, T>*>(node->parent)->child[i] == node) {
          pos = i;
          break;
        }
      }

      // 获得前一个Inner node和后一个Inner node
      Node<K, T>* next = nullptr;
      Node<K, T>* prev = nullptr;

      if (static_cast<InnerNode<K, T>*>(node->parent)->child.size() > pos + 1) {
        next = static_cast<InnerNode<K, T>*>(node->parent)->child[pos + 1];
      }

      if (pos) {
        prev = static_cast<InnerNode<K, T>*>(node->parent)->child[pos - 1];
      }

      if (next && next->parent == node->parent &&
          next->keys.size() > this->mincap_) {
        this->borrow_right_inner(pos, node, next);
      } else if (prev && prev->parent == node->parent &&
                 prev->keys.size() > this->mincap_) {
        this->borrow_left_inner(pos, node, prev);
      } else if (next && next->parent == node->parent &&
                 next->keys.size() <= this->mincap_) {
        this->merge_right_inner(pos, node, next);
      } else if (prev && prev->parent == node->parent &&
                 prev->keys.size() <= this->mincap_) {
        this->merge_left_inner(pos, node, prev);
      }
    }

    // 继续向上进行删除直到node为根节点
    if (node->parent) {
      this->tree_delete(key, node->parent);
    }
  }
}
