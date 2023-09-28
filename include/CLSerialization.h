#ifndef SERIALIZATION_H
#define SERIALIZATION_H
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdexcept>
#include <string>

#include "CLBPlusTree.h"
#define OPEN_CODE 00700
class Serialization {
 public:
  template <typename K, typename T>
  off_t serialize(const BPlusTree<K, T>& tree, const std::string& file);
  template <typename K, typename T>
  off_t serialize(const BPlusTree<K, T>& tree, const char* file);

  template <typename K, typename T>
  BPlusTree<K, T>* deserialize(const std::string& file);
  template <typename K, typename T>
  BPlusTree<K, T>* deserialize(const char* file);

 private:
  // DFS版本
  template <typename K, typename T>
  void write_node(const int& fd, Node<K, T>* node);

  template <typename K, typename T>
  Node<K, T>* read_node(const int& fd, Node<K, T>* parent);

  // BFS版本
  template <typename K, typename T>
  void write_node_bfs(const int& fd, Node<K, T>* node);

  template <typename K, typename T>
  Node<K, T>* read_node_per(const int& fd, Node<K, T>* parent);
  template <typename K, typename T>
  Node<K, T>* read_node_bfs(const int& fd);
};

// 序列化
template <typename K, typename T>
off_t Serialization::serialize(const BPlusTree<K, T>& tree,
                               const std::string& file) {
  return this->serialize(tree, file.c_str());
}

// 序列化
template <typename K, typename T>
off_t Serialization::serialize(const BPlusTree<K, T>& tree, const char* file) {
  int fd = 0;
  fd = open(file, O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, OPEN_CODE);
  if (fd == -1) {
    throw std::runtime_error("open file error\n");
  }
  // 写树相关的属性
  if (write(fd, &tree.depth_, sizeof(int)) == -1) {
    throw std::runtime_error("write depth error\n");
  }
  if (write(fd, &tree.maxcap_, sizeof(int)) == -1) {
    throw std::runtime_error("write maxcap_ error\n");
  }
  if (write(fd, &tree.mincap_, sizeof(int)) == -1) {
    throw std::runtime_error("write mincap_ error\n");
  }
  // 写入根结点，进入递归写子结点过程
  write_node_bfs(fd, tree.root_);

  off_t len = lseek(fd, 0, SEEK_END);
  if (len == -1) {
    throw std::runtime_error("lseek file end error\n");
  }
  if (close(fd) == -1) {
    throw std::runtime_error("write close file error\n");
  }
  return len;
}

// 反序列化
template <typename K, typename T>
BPlusTree<K, T>* Serialization::deserialize(const std::string& file) {
  return this->deserialize<K, T>(file.c_str());
}

template <typename K, typename T>
BPlusTree<K, T>* Serialization::deserialize(const char* file) {
  auto* tree = new BPlusTree<K, T>();
  int fd = 0;
  fd = open(file, O_RDWR | O_CLOEXEC);
  if (fd == -1) {
    throw std::runtime_error("open file error\n");
  }

  // 读出树的属性
  if (read(fd, &tree->depth_, sizeof(int)) == -1) {
    throw std::runtime_error("read depth_ error\n");
  }
  if (read(fd, &tree->maxcap_, sizeof(int)) == -1) {
    throw std::runtime_error("wread maxcap_ error\n");
  }
  if (read(fd, &tree->mincap_, sizeof(int)) == -1) {
    throw std::runtime_error("read mincap_ error\n");
  }

  // 读入根结点
  tree->root_ = read_node_bfs<K, T>(fd);
  if (close(fd) == -1) {
    throw std::runtime_error("read close fd error\n");
  }

  return tree;
}

// 写树中的结点

// 先写is_leaf, keys.size(), keys; 根据is_leaf, 决定写data or child
template <typename K, typename T>
void Serialization::write_node(const int& fd, Node<K, T>* node) {
  if (write(fd, &node->is_leaf, sizeof(bool)) == -1) {
    throw std::runtime_error("write node is_leaf error\n");
  }

  int num = node->keys.size();
  // 写关键字数量
  if (write(fd, &num, sizeof(int)) == -1) {
    throw std::runtime_error("write node keys size error\n");
  }
  // 写关键字
  for (int i = 0; i < num; ++i) {
    if (write(fd, &node->keys[i], sizeof(K)) == -1) {
      throw std::runtime_error("write node keys element error\n");
    }
  }
  if (!node->is_leaf) {
    // write children
    for (int i = 0; i < num + 1; ++i) {
      write_node(fd, static_cast<InternalNode<K, T>*>(node)->child[i]);
    }
  } else {
    // 写值
    for (int i = 0; i < node->keys.size(); ++i) {
      if (write(fd, &static_cast<LeafNode<K, T>*>(node)->data[i], sizeof(T)) ==
          -1) {
        throw std::runtime_error("write data element error\n");
      }
    }
  }
}

template <typename K, typename T>
Node<K, T>* Serialization::read_node(const int& fd, Node<K, T>* parent) {
  bool is_leaf = false;
  if (read(fd, &is_leaf, sizeof(bool)) == -1) {
    throw std::runtime_error("read is_leaf error\n");
  }
  // 返回的结点
  Node<K, T>* pnode = nullptr;
  if (!is_leaf) {
    pnode = new InternalNode<K, T>();
  } else {
    pnode = new LeafNode<K, T>();
  }
  pnode->is_leaf = is_leaf;
  pnode->parent = parent;

  int num = 0;
  if (read(fd, &num, sizeof(num)) == -1) {
    throw std::runtime_error("read keys size error\n");
  }

  // 读取关键字数组
  int key = 0;
  for (int i = 0; i < num; ++i) {
    if (read(fd, &key, sizeof(K)) == -1) {
      throw std::runtime_error("read key element error\n");
    }
    pnode->keys.push_back(key);
  }

  if (!is_leaf) {
    for (int i = 0; i < num + 1; ++i) {
      static_cast<InternalNode<K, T>*>(pnode)->child.push_back(
          read_node<K, T>(fd, pnode));
    }
    // 叶子结点链表恢复
    for (int i = 0; i < num + 1; ++i) {
      Node<K, T>* node = static_cast<InternalNode<K, T>*>(pnode)->child[i];
      // 将链表连接起来
      // 找到每个结点的前驱结点
      Node<K, T>* prenode = search_pre_node(node);
      if (prenode) {
        static_cast<LeafNode<K, T>*>(prenode)->next =
            static_cast<LeafNode<K, T>*>(node);
        static_cast<LeafNode<K, T>*>(node)->prev =
            static_cast<LeafNode<K, T>*>(prenode);
      }
    }
  } else {
    // 读叶子结点
    int data = 0;
    for (int i = 0; i < num; ++i) {
      if (read(fd, &data, sizeof(T)) == -1) {
        throw std::runtime_error("read data element error\n");
      }
      static_cast<LeafNode<K, T>*>(pnode)->data.push_back(data);
    }
  }
  return pnode;
}

template <typename K, typename T>
void Serialization::write_node_bfs(const int& fd, Node<K, T>* node) {
  std::queue<Node<K, T>*> q;
  q.push(node);
  while (!q.empty()) {
    Node<K, T>* now = q.front();
    q.pop();

    if (write(fd, &now->is_leaf, sizeof(bool)) == -1) {
      throw std::runtime_error("write node is_leaf error\n");
    }
    int num = now->keys.size();
    // 写关键字数量
    if (write(fd, &num, sizeof(int)) == -1) {
      throw std::runtime_error("write node keys size error\n");
    }
    // 写关键字
    for (int i = 0; i < num; ++i) {
      if (write(fd, &now->keys[i], sizeof(K)) == -1) {
        throw std::runtime_error("write node keys element error\n");
      }
    }

    if (!now->is_leaf) {
      for (int i = 0; i < num + 1; ++i) {
        q.push(static_cast<InternalNode<K, T>*>(now)->child[i]);
      }
    } else {
      // 写值
      for (int i = 0; i < num; ++i) {
        if (write(fd, &static_cast<LeafNode<K, T>*>(now)->data[i], sizeof(T)) ==
            -1) {
          throw std::runtime_error("write data element error\n");
        }
      }
    }
  }
}

// 读取每个结点的值
template <typename K, typename T>
Node<K, T>* Serialization::read_node_per(const int& fd, Node<K, T>* parent) {
  bool is_leaf = false;
  if (read(fd, &is_leaf, sizeof(bool)) == -1) {
    throw std::runtime_error("read is_leaf error\n");
  }
  // 返回的结点
  Node<K, T>* pnode = nullptr;
  if (!is_leaf) {
    pnode = new InternalNode<K, T>();
  } else {
    pnode = new LeafNode<K, T>();
  }
  pnode->is_leaf = is_leaf;
  pnode->parent = parent;

  // 关键字数组大小
  int num = 0;
  if (read(fd, &num, sizeof(num)) == -1) {
    throw std::runtime_error("read keys size error\n");
  }

  // 读取关键字数组
  int key = 0;
  for (int i = 0; i < num; ++i) {
    if (read(fd, &key, sizeof(K)) == -1) {
      throw std::runtime_error("read key element error\n");
    }
    pnode->keys.push_back(key);
  }
  if (pnode->is_leaf) {
    // 读值
    int data = 0;
    for (int i = 0; i < num; ++i) {
      if (read(fd, &data, sizeof(T)) == -1) {
        throw std::runtime_error("read data element error\n");
      }
      static_cast<LeafNode<K, T>*>(pnode)->data.push_back(data);
    }
  }
  return pnode;
}

// BFS恢复结点
template <typename K, typename T>
Node<K, T>* Serialization::read_node_bfs(const int& fd) {
  bool is_leaf = false;
  if (read(fd, &is_leaf, sizeof(bool)) == -1) {
    throw std::runtime_error("read is_leaf error\n");
  }
  // 返回的结点
  Node<K, T>* pnode = nullptr;
  if (!is_leaf) {
    pnode = new InternalNode<K, T>();
  } else {
    pnode = new LeafNode<K, T>();
  }
  pnode->is_leaf = is_leaf;
  pnode->parent = nullptr;

  std::queue<Node<K, T>*> q;
  q.push(pnode);
  while (!q.empty()) {
    Node<K, T>* now = q.front();
    q.pop();
    if (now != pnode) {
      if (read(fd, &now->is_leaf, sizeof(bool)) == -1) {
        throw std::runtime_error("read is_leaf error\n");
      }
    }
    // 关键字数组大小
    int num = 0;
    if (read(fd, &num, sizeof(num)) == -1) {
      throw std::runtime_error("read keys size error\n");
    }

    // 读取关键字数组
    int key = 0;
    for (int i = 0; i < num; ++i) {
      if (read(fd, &key, sizeof(K)) == -1) {
        throw std::runtime_error("read key element error\n");
      }
      now->keys.push_back(key);
    }

    if (!now->is_leaf) {
      // 读入孩子结点
      for (int i = 0; i < num + 1; ++i) {
        static_cast<InternalNode<K, T>*>(pnode)->child.push_back(
            read_node_per(fd, pnode));
      }
      for (int i = 0; i < num + 1; ++i) {
        q.push(static_cast<InternalNode<K, T>*>(now)->child[i]);
      }
    } else {
      // 构建链表
      Node<K, T>* prenode = search_pre_node(now);
      if (prenode) {
        static_cast<LeafNode<K, T>*>(prenode)->next =
            static_cast<LeafNode<K, T>*>(now);
        static_cast<LeafNode<K, T>*>(now)->prev =
            static_cast<LeafNode<K, T>*>(prenode);
      }
    }
  }
  return pnode;
}

// 在树中寻找前驱结点
template <typename K, typename T>
Node<K, T>* search_pre_node(Node<K, T>* node) {
  if (node == nullptr || !node->is_leaf) {
    return nullptr;
  }
  int index = 0;
  Node<K, T>* parent = nullptr;
  Node<K, T>* prenode = nullptr;

  if (node->parent == nullptr) {
    return nullptr;
  }

  while (node->parent != nullptr) {
    parent = node->parent;
    index = 0;
    // 寻找孩子结点对应的index
    while (static_cast<InternalNode<K, T>*>(parent)->child[index] != node) {
      index++;
    }

    if (index) {
      // 寻找其兄弟结点最右侧的叶子结点
      prenode = static_cast<InternalNode<K, T>*>(parent)->child[index - 1];
      // 一直到叶子结点
      while (!prenode->is_leaf) {
        prenode = static_cast<InternalNode<K, T>*>(prenode)
                      ->child[prenode->keys.size()];
      }
      return prenode;
    }
    // 该结点为最左侧孩子结点
    node = parent;
  }
  return nullptr;
}
#endif  // SERIALIZATION_H