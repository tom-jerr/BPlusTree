#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include <tuple>
#define MAX_DEGREE 3
#include <vector>

// 声明Inner Node
template <typename K, typename T>
class InnerNode;

// Leaf Node和Inner Node的父类
template <typename K, typename T>
class Node {
 public:
  using KeyType = K;
  using DataType = T;

  int degree{MAX_DEGREE};           // degree*2=key数
  Node<KeyType, DataType>* parent;  // 父亲节点
  std::vector<KeyType> keys;        // 关键字数组
  bool is_leaf;                     // 判断是否为叶子节点

  Node();
  virtual ~Node();
  virtual Node<K, T>* getclasstype();

  // 寻找key对应的位置
  // find_last_pos: 寻找不小于key的最小的pos
  int find_last_pos(KeyType key);
  int key_index(KeyType key);
};

// Leaf Node
template <typename K, typename T>
class LeafNode : public Node<K, T> {
 public:
  using KeyType = K;
  using DataType = T;

  std::vector<DataType> data;  // 存放数据
  // 叶子结点使用双向链表连接
  LeafNode* prev;  // 指向前一个leaf node
  LeafNode* next;  // 指向后一个leaf node

  LeafNode(LeafNode<K, T>* prev_node = nullptr,
           LeafNode<K, T>* next_node = nullptr);
  virtual ~LeafNode();
  virtual LeafNode<K, T>* getclasstype();

  // 插入叶子节点&超过阈值后分裂节点
  bool insert_leaf(KeyType key, DataType value);
  std::tuple<Node<K, T>*, Node<K, T>*, K> split_leaf();

  bool delete_leaf(KeyType key);
};

// Inner Node
template <typename K, typename T>
class InnerNode : public Node<K, T> {
 public:
  using KeyType = K;
  using DataType = T;

  std::vector<Node<KeyType, DataType>*>
      child;  // 指向孩子节点(inner node or leaf node)

  InnerNode();
  virtual InnerNode<K, T>* getclasstype();

  int child_index(KeyType key);

  bool insert_inner(std::vector<Node<K, T>*> p_node, KeyType key);
  std::tuple<Node<K, T>*, Node<K, T>*, K> split_inner();

  bool delete_inner(KeyType key);
};

// B Plus Tree
template <typename K, typename T>
class BPlusTree {
 public:
  using KeyType = K;
  using DataType = T;

 protected:
  Node<KeyType, DataType>* root_;
  int depth_;
  int maxcap_;
  int mincap_;

 public:
  BPlusTree();
  virtual ~BPlusTree();

  DataType search(KeyType key);
  std::vector<DataType> search_range(KeyType begin, KeyType end);
  void insert_in_parent(std::tuple<Node<K, T>*, Node<K, T>*, K> result);
  void tree_insert(KeyType key, DataType data);
  void tree_delete(KeyType key, Node<K, T>* node = nullptr);

  // 辅助函数
  Node<K, T>* find_leaf(int key);
  void show_bplustree();
  // 对叶子节点的操作
  void borrow_right_leaf(Node<K, T>* node, Node<K, T>* next);
  void borrow_left_leaf(Node<K, T>* node, Node<K, T>* prev);
  void merge_right_leaf(Node<K, T>* node, Node<K, T>* next);
  void merge_left_leaf(Node<K, T>* node, Node<K, T>* prev);
  // 对内部节点的操作
  void borrow_right_inner(int pos, Node<K, T>* node, Node<K, T>* next);
  void borrow_left_inner(int pos, Node<K, T>* node, Node<K, T>* prev);
  void merge_right_inner(int pos, Node<K, T>* node, Node<K, T>* next);
  void merge_left_inner(int pos, Node<K, T>* node, Node<K, T>* prev);
};

#endif  // BPLUSTREE_H