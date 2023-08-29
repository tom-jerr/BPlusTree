#ifndef BPLUSTREE_H
#define BPLUSTREE_H

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

  KeyType* keys;                         // 关键字数组
  int count;                             // 当前关键字的数量
  int degree;                            // degree*2=key数
  InnerNode<KeyType, DataType>* parent;  // 父亲节点

  explicit Node(int m);
  virtual ~Node();
  virtual Node<K, T>* getclasstype();

  // 寻找key对应的位置
  // find_last_pos: 寻找不小于key的最小的pos
  int find_last_pos(KeyType key);
};

// Leaf Node
template <typename K, typename T>
class LeafNode : public Node<K, T> {
 public:
  using KeyType = K;
  using DataType = T;

  DataType* data;
  // 叶子结点使用双向链表连接
  LeafNode* prev;
  LeafNode* next;

  LeafNode(int m);
  virtual ~LeafNode();
  virtual LeafNode<K, T>* getclasstype();

  bool insert_leaf(KeyType key, DataType value);
  LeafNode* split_leaf(LeafNode* p_node);

  void print_leaf();
};

// Inner Node
template <typename K, typename T>
class InnerNode : public Node<K, T> {
 public:
  using KeyType = K;
  using DataType = T;

  Node<KeyType, DataType>** child;

  InnerNode(int m);
  virtual ~InnerNode();
  virtual InnerNode<K, T>* getclasstype();

  bool insert_inner(InnerNode* p_node, DataType value);
  InnerNode* split_inner(InnerNode* p_node, KeyType key);

  void print_inner();
};

// B Plus Tree
template <typename K, typename T>
class BPlusTree {
 public:
  using KeyType = K;
  using DataType = T;

 protected:
  Node<KeyType, DataType>* root_;

 public:
  BPlusTree();
  virtual ~BPlusTree();

  DataType search(KeyType key);
  std::vector<DataType> search_range(KeyType begin, KeyType end);
  bool tree_insert(KeyType key, DataType data);
  bool tree_delete(KeyType key);
};
#endif  // BPLUSTREE_H