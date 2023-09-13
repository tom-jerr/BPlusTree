#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include <sys/types.h>

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#define MAX_DEGREE 3  // 结点的最大容纳数量+1
// 声明Inner Node
template <typename K, typename T>
class InternalNode;

// Leaf Node和Inner Node的父类
template <typename K, typename T>
class Node {
 public:
  Node<K, T>* parent;   // 父亲节点
  std::vector<K> keys;  // 关键字数组
  bool is_leaf;         // 判断是否为叶子节点

  Node();
  virtual ~Node();

  // find_last_pos: 寻找不小于key的最小的pos
  int find_last_pos(K key);
  int find_first_big_pos(K key);

  // 寻找key再keys数组中的索引
  int key_index(K key);
};

// Leaf Node
template <typename K, typename T>
class LeafNode : public Node<K, T> {
 public:
  std::vector<T> data;   // 存放数据
  LeafNode<K, T>* prev;  // 指向前一个leaf node
  LeafNode<K, T>* next;  // 指向后一个leaf node

  // 构造和析构函数
  LeafNode(LeafNode<K, T>* prev_node = nullptr,
           LeafNode<K, T>* next_node = nullptr);
  ~LeafNode();

  // 插入叶子节点&超过阈值后分裂节点
  bool insert_leaf(K key, T value);
  std::tuple<Node<K, T>*, Node<K, T>*, K> split_leaf();
  // 删除叶子节点
  bool delete_leaf(K key);
};

// Inner Node
template <typename K, typename T>
class InternalNode : public Node<K, T> {
 public:
  std::vector<Node<K, T>*> child;  // 指向孩子节点(inner node or leaf node)

  InternalNode();
  ~InternalNode();
  int child_index(K key);

  bool insert_inner(std::vector<Node<K, T>*> p_node, K key);
  std::tuple<Node<K, T>*, Node<K, T>*, K> split_inner();

  bool delete_inner(K key);
};

// B Plus Tree
template <typename K, typename T>
class BPlusTree {
  friend class Serialization;
  friend Node<K, T>* search_pre_node(Node<K, T>* node);

 private:
  Node<K, T>* root_;
  int depth_{1};
  int maxcap_;  // 结点最大容纳数量
  int mincap_;  // 结点最小容纳数量

  void clear();

 public:
  BPlusTree();
  ~BPlusTree();

  // 查找、插入和删除函数
  T search(K key);
  std::vector<T> search_range(K begin, K end);
  void tree_insert(K key, T data);
  void tree_delete(K key, Node<K, T>* node = nullptr);

  // 辅助函数
  Node<K, T>* get_root();
  int get_depth();
  Node<K, T>* find_leaf(int key);
  // 将新建结点插入树中
  void insert_in_parent(std::tuple<Node<K, T>*, Node<K, T>*, K> result);
  // 显示整个B+Tree的键值
  void show_bplustree();
  // 层次遍历整个B+Tree
  std::vector<K> bfs();

  // 删除辅助函数
  // 对叶子结点的操作
  void borrow_right_leaf(Node<K, T>* node, Node<K, T>* next);
  void borrow_left_leaf(Node<K, T>* node, Node<K, T>* prev);
  void merge_right_leaf(Node<K, T>* node, Node<K, T>* next);
  void merge_left_leaf(Node<K, T>* node, Node<K, T>* prev);
  // 对内部结点的操作
  void borrow_right_inner(int pos, Node<K, T>* node, Node<K, T>* next);
  void borrow_left_inner(int pos, Node<K, T>* node, Node<K, T>* prev);
  void merge_right_inner(int pos, Node<K, T>* node, Node<K, T>* next);
  void merge_left_inner(int pos, Node<K, T>* node, Node<K, T>* prev);
};

template <typename K, typename T>
Node<K, T>::Node() : parent(nullptr) {}

template <typename K, typename T>
Node<K, T>::~Node() = default;

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
LeafNode<K, T>::~LeafNode() = default;
// {
//   if (this->prev) {
//     delete prev;
//   }
//   if (this->prev) {
//     delete next;
//   }
//   prev = nullptr;
//   next = nullptr;
// }

// Inner Node
template <typename K, typename T>
InternalNode<K, T>::InternalNode() : Node<K, T>() {
  this->is_leaf = false;
}

// Inner Node
template <typename K, typename T>
InternalNode<K, T>::~InternalNode() = default;

template <typename K, typename T>
int InternalNode<K, T>::child_index(K key) {
  // for (int i = 0; i < this->keys.size(); i++) {
  //   if (key < this->keys[i]) {
  //     return i;
  //   }
  // }
  // return this->keys.size();
  int index = this->find_first_big_pos(key);
  if (key >= this->keys[index]) {
    return this->keys.size();
  }
  return index;
}

template <typename K, typename T>
BPlusTree<K, T>::BPlusTree()
    : root_(nullptr), maxcap_(MAX_DEGREE - 1), mincap_(maxcap_ / 2) {
  if (!root_) {
    root_ = new LeafNode<K, T>();
  }
}

template <typename K, typename T>
BPlusTree<K, T>::~BPlusTree() {
  this->clear();
  // std::queue<Node<K, T>*> q;
  // q.push(root_);
  // while (!q.empty()) {
  //   Node<K, T>* tmp = q.front();
  //   q.pop();

  //   if (!tmp->is_leaf) {
  //     for (int i = 0; i < static_cast<InternalNode<K,
  //     T>*>(tmp)->child.size();
  //          ++i) {
  //       q.push(static_cast<InternalNode<K, T>*>(tmp)->child[i]);
  //     }
  //   }
  //   delete tmp;
  // }
  // this->root_ = nullptr;
};

template <typename K, typename T>
void BPlusTree<K, T>::clear() {
  std::queue<Node<K, T>*> q;
  q.push(root_);
  while (!q.empty()) {
    Node<K, T>* tmp = q.front();
    q.pop();

    if (!tmp->is_leaf) {
      for (int i = 0; i < static_cast<InternalNode<K, T>*>(tmp)->child.size();
           ++i) {
        q.push(static_cast<InternalNode<K, T>*>(tmp)->child[i]);
      }
    }
    delete tmp;
  }
  this->root_ = nullptr;
}
// 帮助函数

// 返回根节点
template <typename K, typename T>
Node<K, T>* BPlusTree<K, T>::get_root() {
  return this->root_;
}

template <typename K, typename T>
int BPlusTree<K, T>::get_depth() {
  return this->depth_;
}
// 寻找叶子节点
template <typename K, typename T>
Node<K, T>* BPlusTree<K, T>::find_leaf(int key) {
  if (this->root_->is_leaf) {
    return this->root_;
  }
  Node<K, T>* p_node = this->root_;
  while (!p_node->is_leaf) {
    int flag = 0;
    int size = p_node->keys.size();
    for (int i = 0; i < size; ++i) {
      if (p_node->keys[i] > key) {
        p_node = static_cast<InternalNode<K, T>*>(p_node)->child[i];
        flag = 1;
        break;
      }
      if (p_node->keys[i] == key) {
        p_node = static_cast<InternalNode<K, T>*>(p_node)->child[i + 1];
        flag = 1;
        break;
      }
    }
    // 没找到，直接指向最右侧的孩子节点
    if (!flag) {
      p_node =
          static_cast<InternalNode<K, T>*>(p_node)->child[p_node->keys.size()];
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
        for (int j = 0;
             j < (static_cast<InternalNode<K, T>*>(q[i]))->child.size(); ++j) {
          q.push_back(static_cast<InternalNode<K, T>*>(q[i])->child[j]);
        }
      }
      std::cout << "[";
      int nk = 0;
      int size = q[i]->keys.size() - 1;
      for (nk = 0; nk < size; ++nk) {
        std::cout << q[i]->keys[nk] << ",";
      }
      std::cout << q[i]->keys[nk] << "] ";
    }
    q.erase(q.begin(), q.begin() + size);
    std::cout << "\n";
  }
}

template <typename K, typename T>
std::vector<K> BPlusTree<K, T>::bfs() {
  std::vector<K> bpt_keys;
  std::queue<Node<K, T>*> q;
  q.push(root_);
  while (!q.empty()) {
    Node<K, T>* tmp = q.front();
    q.pop();
    int size = tmp->keys.size();
    for (int i = 0; i < size; ++i) {
      bpt_keys.push_back(tmp->keys[i]);
    }
    if (!tmp->is_leaf) {
      for (int i = 0; i < static_cast<InternalNode<K, T>*>(tmp)->child.size();
           ++i) {
        q.push(static_cast<InternalNode<K, T>*>(tmp)->child[i]);
      }
    }
  }
  return bpt_keys;
}

// Search帮助函数

/* 在节点中寻找大于key的最小键值的位置*/
template <typename K, typename T>
int Node<K, T>::find_last_pos(K key) {
  int low = 0;
  int high = this->keys.size() - 1;
  int mid = 0;
  while (low < high) {
    mid = (low + high + 1) / 2;
    if (this->keys[mid] > key) {
      high = mid - 1;
    } else {
      low = mid;
    }
  }
  return high;
}

template <typename K, typename T>
int Node<K, T>::find_first_big_pos(K key) {
  int low = 0;
  int high = this->keys.size() - 1;
  int mid = 0;
  while (low < high) {
    mid = (low + high) / 2;
    if (this->keys[mid] < key) {
      low = mid + 1;
    } else {
      high = mid;
    }
  }
  return high;
}

// 寻找key所在的位置
template <typename K, typename T>
int Node<K, T>::key_index(K key) {
  int index = this->find_first_big_pos(key);
  if (this->keys[index] == key) {
    return index;
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

  if (leaf->keys[position] < begin) {
    leaf = static_cast<LeafNode<K, T>*>(leaf)->next;
    if (leaf == nullptr) {
      return vec;
    }
    // 进入新节点position从第一个开始计算
    position = 0;
  }
  int leaf_size = leaf->keys.size();
  // 当前节点中符合条件的值
  for (int i = position; i < leaf_size && leaf->keys[i] <= end; ++i) {
    // 提前结束遍历过程
    if (leaf->keys[i] > end) {
      return vec;
    }
    vec.push_back(static_cast<LeafNode<K, T>*>(leaf)->data[i]);
  }

  // 后面节点中符合条件的值
  while (leaf->keys[leaf->keys.size() - 1] <= end &&
         static_cast<LeafNode<K, T>*>(leaf)->next != nullptr) {
    leaf = static_cast<LeafNode<K, T>*>(leaf)->next;
    // 提前结束遍历过程
    if (leaf->keys[0] > end) {
      return vec;
    }
    int leaf_size = leaf->keys.size();
    for (int i = 0; i < leaf_size && leaf->keys[i] <= end; ++i) {
      // 提前结束遍历过程
      if (leaf->keys[i] > end) {
        return vec;
      }
      vec.push_back(static_cast<LeafNode<K, T>*>(leaf)->data[i]);
    }
  }
  return vec;
}

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
  i = this->find_first_big_pos(key);
  if (this->keys[i] < key) {
    i++;
  }
  this->keys.insert(this->keys.begin() + i, key);
  this->data.insert(this->data.begin() + i, value);
  return true;
}

// insert <p_node, key> to inner node
template <typename K, typename T>
bool InternalNode<K, T>::insert_inner(std::vector<Node<K, T>*> p_node, K key) {
  int i = 0;
  i = this->find_first_big_pos(key);
  if (this->keys[i] < key) {
    i++;
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

// when keys of leaf node attend to ; it will split
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
std::tuple<Node<K, T>*, Node<K, T>*, K> InternalNode<K, T>::split_inner() {
  auto* p_node = new InternalNode<K, T>();
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
    Node<K, T>* new_root = new InternalNode<K, T>();
    this->depth_ += 1;
    new_root->keys.push_back(key);
    static_cast<InternalNode<K, T>*>(new_root)->child = {left, right};
    // static_cast<InternalNode<K, T>*>(new_root)->child.push_back(new_node);
    this->root_ = new_root;
    left->parent = this->root_;
    right->parent = this->root_;
    return;
  }

  Node<K, T>* parent = right->parent;
  bool op_inner = static_cast<InternalNode<K, T>*>(parent)->insert_inner(
      {left, right}, key);
  if (!op_inner) {
    std::cout << "failed to insert inner"
              << "\n";
  }
  if (parent->keys.size() > this->maxcap_) {
    std::tuple<Node<K, T>*, Node<K, T>*, K> new_inner =
        static_cast<InternalNode<K, T>*>(parent)->split_inner();
    insert_in_parent(new_inner);
  }
}

template <typename K, typename T>
void BPlusTree<K, T>::tree_insert(K key, T data) {
  bool op_leaf = false;
  // 如果此时根节点没有元素，直接插入<key, value>
  if (this->root_->keys.empty() && this->root_->is_leaf) {
    op_leaf = static_cast<LeafNode<K, T>*>(this->root_)->insert_leaf(key, data);
    if (!op_leaf) {
      std::cout << "failed to insert leaf"
                << "\n";
    }
    return;
  }
  Node<K, T>* p_node = find_leaf(key);

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
        static_cast<InternalNode<K, T>*>(this->parent)->child_index(key);
    // 父节点的key删除后改变（key的index比child的index小1）
    if (child_index && !this->keys.empty()) {
      this->parent->keys[child_index - 1] = this->keys.front();
    }
  }
  return true;
}

// 内部节点删除key
template <typename K, typename T>
bool InternalNode<K, T>::delete_inner(K key) {
  if (this->keys.empty()) {
    return true;
  }
  int index = this->key_index(key);
  if (index != -1) {
    Node<K, T>* left_leaf = this->child[index + 1];
    while (!left_leaf->is_leaf) {
      left_leaf = static_cast<InternalNode<K, T>*>(left_leaf)->child.front();
    }
    this->keys[index] = left_leaf->keys.front();
  }
  // 如果只有一个children，直接删掉该父节点
  // if (this->child.size() <= 1) {
  // }
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

  for (int i = 0;
       i < static_cast<InternalNode<K, T>*>(node->parent)->child.size(); ++i) {
    if (static_cast<InternalNode<K, T>*>(node->parent)->child[i] == next) {
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

  for (int i = 0;
       i < static_cast<InternalNode<K, T>*>(node->parent)->child.size(); ++i) {
    if (static_cast<InternalNode<K, T>*>(node->parent)->child[i] == node) {
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

  static_cast<InternalNode<K, T>*>(node)->child.insert(
      static_cast<InternalNode<K, T>*>(node)->child.end(),
      static_cast<InternalNode<K, T>*>(next)->child.front());
  static_cast<InternalNode<K, T>*>(next)->child.erase(
      static_cast<InternalNode<K, T>*>(next)->child.begin());
  // 改变父亲节点
  static_cast<InternalNode<K, T>*>(node)->child.back()->parent = node;
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

  static_cast<InternalNode<K, T>*>(node)->child.insert(
      static_cast<InternalNode<K, T>*>(node)->child.begin(),
      static_cast<InternalNode<K, T>*>(prev)->child.back());
  static_cast<InternalNode<K, T>*>(prev)->child.erase(
      static_cast<InternalNode<K, T>*>(prev)->child.end() - 1);
  // 改变父亲节点
  static_cast<InternalNode<K, T>*>(node)->child.front()->parent = node;
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

  // static_cast<LeafNode<K, T>*>(node)->next = nullptr;
  // static_cast<LeafNode<K, T>*>(node)->prev = nullptr;

  // 处理next父节点中需要删除的key和child
  for (int i = 0;
       i < static_cast<InternalNode<K, T>*>(next->parent)->child.size(); ++i) {
    if (static_cast<InternalNode<K, T>*>(next->parent)->child[i] == next) {
      next->parent->keys.erase(next->parent->keys.begin() + i - 1);
      static_cast<InternalNode<K, T>*>(next->parent)
          ->child.erase(
              static_cast<InternalNode<K, T>*>(next->parent)->child.begin() +
              i);

      break;
    }
  }
  delete next;
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
  for (int i = 0;
       i < static_cast<InternalNode<K, T>*>(node->parent)->child.size(); ++i) {
    if (static_cast<InternalNode<K, T>*>(node->parent)->child[i] == node) {
      node->parent->keys.erase(node->parent->keys.begin() + i - 1);
      static_cast<InternalNode<K, T>*>(node->parent)
          ->child.erase(
              static_cast<InternalNode<K, T>*>(node->parent)->child.begin() +
              i);
      // delete node;
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
  static_cast<InternalNode<K, T>*>(node->parent)
      ->child.erase(
          static_cast<InternalNode<K, T>*>(node->parent)->child.begin() + pos +
          1);

  // 插入合并的值
  node->keys.insert(node->keys.end(), next->keys.begin(), next->keys.end());
  static_cast<InternalNode<K, T>*>(node)->child.insert(
      static_cast<InternalNode<K, T>*>(node)->child.end(),
      static_cast<InternalNode<K, T>*>(next)->child.begin(),
      static_cast<InternalNode<K, T>*>(next)->child.end());

  for (Node<K, T>* child : static_cast<InternalNode<K, T>*>(node)->child) {
    child->parent = node;
  }
  delete next;
}

template <typename K, typename T>
void BPlusTree<K, T>::merge_left_inner(int pos, Node<K, T>* node,
                                       Node<K, T>* prev) {
  prev->keys.insert(prev->keys.end(), node->parent->keys[pos - 1]);
  node->parent->keys.erase(node->parent->keys.begin() + pos - 1);
  static_cast<InternalNode<K, T>*>(node->parent)
      ->child.erase(
          static_cast<InternalNode<K, T>*>(node->parent)->child.begin() + pos);

  // 插入合并的值
  prev->keys.insert(prev->keys.end(), node->keys.begin(), node->keys.end());
  static_cast<InternalNode<K, T>*>(prev)->child.insert(
      static_cast<InternalNode<K, T>*>(prev)->child.end(),
      static_cast<InternalNode<K, T>*>(node)->child.begin(),
      static_cast<InternalNode<K, T>*>(node)->child.end());

  for (Node<K, T>* child : static_cast<InternalNode<K, T>*>(prev)->child) {
    child->parent = prev;
  }
  // delete node;
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
    bool op_inner = static_cast<InternalNode<K, T>*>(node)->delete_inner(key);
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
          !static_cast<InternalNode<K, T>*>(this->root_)->child.empty()) {
        Node<K, T>* delete_node = this->root_;
        this->root_ = static_cast<InternalNode<K, T>*>(this->root_)->child[0];
        this->root_->parent = nullptr;
        this->depth_ -= 1;
        delete delete_node;
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
           i < static_cast<InternalNode<K, T>*>(node->parent)->child.size();
           ++i) {
        if (static_cast<InternalNode<K, T>*>(node->parent)->child[i] == node) {
          pos = i;
          break;
        }
      }

      // 获得前一个Inner node和后一个Inner node
      Node<K, T>* next = nullptr;
      Node<K, T>* prev = nullptr;

      if (static_cast<InternalNode<K, T>*>(node->parent)->child.size() >
          pos + 1) {
        next = static_cast<InternalNode<K, T>*>(node->parent)->child[pos + 1];
      }

      if (pos) {
        prev = static_cast<InternalNode<K, T>*>(node->parent)->child[pos - 1];
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
  }
  // 继续向上进行删除直到node为根节点
  if (node->parent) {
    this->tree_delete(key, node->parent);
  }
}

#endif  // BPLUSTREE_H