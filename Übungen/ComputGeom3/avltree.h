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
#include <tuple>
#include <functional>
#include "lessxcomparator.h"
#include "lessycomparator.h"
#include "ymediancomparator.h"
#include "xmediancomparator.h"

template <typename T>
class AVLTree
{
public:
  class Node
  {
  public:
    T value;

    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    bool isVertical = true;

    Node(T v = T())
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

  std::vector<QPointF>* x;
  std::vector<QPointF>* y;

  Node* insertR(T value, Node* p)
  {
    if (p == nullptr) {
      p = new Node(value);
    } else if (value < p->value) {
      p->left = insertR(value, p->left);
    } else { // if (value >= p->value). If you want a set, use else if (value >
             // p->value) and an empty else instruction
      p->right = insertR(value, p->right);
    }

    return p;
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

  template <typename OrderComparator>
  auto partitionField(std::vector<QPointF>* field, QPointF median,
                      OrderComparator orderSort)
  {
    std::vector<QPointF> partition1;
    std::vector<QPointF> partition2;


    for (auto elem : *field) {
      if (elem == median) {
        // Do nothing
      } else if (orderSort(median, elem)) {
        partition2.push_back(elem);
      } else {
        partition1.push_back(elem);
      }
    }

    return std::make_tuple(partition1, partition2);
  }

  void constructBalanced2DTree(std::vector<QPointF>* partY,
                               std::vector<QPointF>* partX,
                               Node** p, Node* parent, const bool isVertical)
  {
    if (partY->size() > 0) {

      int medianIndex = (partY->size()-1) / 2;

      if (*p == nullptr) {
        *p = new Node();
        (**p).parent = parent;
        (**p).isVertical = isVertical;
      }

      QPointF median;
      std::vector<T> partX1, partX2;
      std::vector<T> partY1, partY2;

      if (isVertical) {
          median = (*partY)[medianIndex];
          std::tie(partY1, partY2) = partitionField(partY, median, LessYComparator());
          std::tie(partX1, partX2) = partitionField(partX, median, LessYComparator());
      } else {
          median = (*partX)[medianIndex];
          std::tie(partX1, partX2) = partitionField(partX, median, LessXComparator());
          std::tie(partY1, partY2) = partitionField(partY, median, LessXComparator());
      }

      (**p).value = median;

      constructBalanced2DTree(&partY1, &partX1, &(*p)->left, *p, !isVertical);
      constructBalanced2DTree(&partY2, &partX2, &(*p)->right, *p, !isVertical);
    }
  }

public:
  ~AVLTree() { delete root; }

  AVLTree()
    : root(new Node())
    , x(nullptr)
    , y(nullptr)
  {
  }

  void clear()
  {
    delete root;
    root = nullptr;
  }

  void insert(std::vector<QPointF>* _x, std::vector<QPointF>* _y)
  {
    x = _x;
    y = _y;

    std::sort(x->begin(), x->end(), LessXComparator());
    std::sort(y->begin(), y->end(), LessYComparator());

    constructBalanced2DTree(y, x, &root, nullptr, true);
  }

  bool contains(T value) { return contains(value, root); }

  T findMax() { findMax(root)->value; }
};

#endif // AVLTREE_H
