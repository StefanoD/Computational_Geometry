/**
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @author: Stefano Di Martino
 */

#ifndef AVLTREE_H
#define AVLTREE_H

#include <algorithm>
#include <assert.h>
#include <iterator>

template <typename T>
class AVLTree
{
private:
  class Node
  {
  public:
    int height = 0;
    T value;

    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;

    Node(T v)
      : value(v)
    {
    }

    // Kleiner-Ordnung
    bool operator<(const T& _value) const { return value < _value; }

    ~Node()
    {
      delete left;
      delete right;
    }
  };

  Node* root = nullptr;

  int getHeight(Node* p)
  {
    if (p == nullptr)
      return -1;
    else
      return p->height;
  }

  int getBalance(Node* p)
  {
    if (p == nullptr)
      return 0;
    else
      return getHeight(p->right) - getHeight(p->left);
  }

  Node* insertR(T value, Node* p)
  {
    if (p == nullptr) {
      p = new Node(value);
    } else if (value < p->value) {
      Node* temp = insertR(value, p->left);
      p->left = temp;
      temp->parent = p;
    } else { // if (value >= p->value). If you want a set, use else if (value >
             // p->value) and an empty else instruction
      Node* temp = insertR(value, p->right);
      p->right = temp;
      temp->parent = p;
    }

    p = balance(p);

    return p;
  }

  Node* balance(Node* p)
  {
    if (p == nullptr)
      return nullptr;

    p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;

    if (getBalance(p) == -2) {
      if (getBalance(p->left) <= 0)
        p = rotateRight(p);
      else
        p = rotateLeftRight(p);
    } else if (getBalance(p) == 2) {
      if (getBalance(p->right) >= 0)
        p = rotateLeft(p);
      else
        p = rotateRightLeft(p);
    }

    return p;
  }

  Node* rotateRight(Node* p)
  {
    assert(p->left != nullptr);

    Node* q = p->left;
    p->left = q->right;
    q->right = p;

    p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
    q->height = std::max(getHeight(q->left), getHeight(q->right)) + 1;

    q->parent = p->parent;

    if (p->left != nullptr) {
      p->left->parent = p;
    }

    p->parent = q;

    return q;
  }

  Node* rotateLeft(Node* p)
  {
    assert(p->right != nullptr);

    Node* q = p->right;
    p->right = q->left;
    q->left = p;

    p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
    q->height = std::max(getHeight(q->left), getHeight(q->right)) + 1;

    q->parent = p->parent;

    if (p->right != nullptr) {
      p->right->parent = p;
    }

    p->parent = q;

    return q;
  }

  Node* rotateLeftRight(Node* p)
  {
    assert(p->left != nullptr);

    p->left = rotateLeft(p->left);
    return rotateRight(p);
  }

  Node* rotateRightLeft(Node* p)
  {
    assert(p->right != nullptr);

    p->right = rotateRight(p->right);
    return rotateLeft(p);
  }

  bool contains(T value, Node* p)
  {
    if (p == nullptr) {
      return false;
    }

    if (value < p->value) {
      return contains(value, p->left);
    }

    if (value > p->value) {
      return contains(value, p->right);
    }

    return true;
  }

  Node* findMax(Node* p)
  {
    if (p == nullptr) {
      return p;
    } else {
      while (p->right != nullptr) {
        p = p->right;
      }
      return p;
    }
  }

  Node* findMin(Node* p)
  {
    if (p == nullptr) {
      return p;
    } else {
      while (p->left != nullptr) {
        p = p->left;
      }
      return p;
    }
  }

  Node* findBiggerEqualThan(Node* p, T value)
  {
    if (p != nullptr) {
      if (p->value == value) {
        return p;
      } else if (p->value < value) {
        if (p->right == nullptr) {
          return inOrderSuccessor(p);
        }
        return findBiggerEqualThan(p->right, value);
      } else { // if (p->value > value) {
        if (p->left == nullptr) {
          return p;
        }
        return findBiggerEqualThan(p->left, value);
      }
    } else {
      return p;
    }
  }

  Node* inOrderSuccessor(Node* n)
  {
    if (n->right != nullptr) {
      return findMin(n->right);
    }

    Node* p = n->parent;

    while (p != nullptr && n == p->right) {
      n = p;
      p = p->parent;
    }

    return p;
  }

  class iterator : std::iterator<std::forward_iterator_tag, T>
  {
    AVLTree* tree;
    Node* node;

  public:
    explicit iterator()
      : tree(nullptr)
      , node(nullptr)
    {
    }

    explicit iterator(AVLTree* tree, Node* startNode = nullptr)
      : tree(tree)
      , node(startNode)
    {
    }

    T& operator*() { return node->value; }

    iterator& operator++()
    {
      if (node != nullptr) {
        node = tree->inOrderSuccessor(node);
      }

      return *this;
    }

    iterator operator++(int)
    {
      iterator tmp = *this;
      ++*this;
      return tmp;
    }

    bool operator==(const iterator& other) const { return node == other.node; }
    bool operator!=(const iterator& other) const { return node != other.node; }
  };

public:
  ~AVLTree() { delete root; }

  void clear()
  {
    delete root;
    root = nullptr;
  }

  iterator begin()
  {
    Node* startNode = findMin(root);
    return iterator(this, startNode);
  }
  iterator end() { return iterator(); }

  iterator findBiggerEqualThan(T value)
  {
    Node* p = findBiggerEqualThan(root, value);
    return iterator(this, p);
  }

  void insert(T value)
  {
    if (root == nullptr) {
      root = new Node(value);
    } else {
      root = insertR(value, root);
    }
  }

  bool contains(T value) { return contains(value, root); }

  T findMax() { findMax(root)->value; }
};

#endif // AVLTREE_H
