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

  std::vector<T>* x;
  std::vector<T>* y;

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

  template <typename SortComparator>
  void partitionField(std::vector<T>* field, const int leftIndex,
                      const int medianIndex, const int rightIndex,
                      const std::function<int(QPointF)>& medianCompare,
                      const SortComparator sortCompare)
  {
    auto leftIt = field->begin() + leftIndex;

    auto medianIt = field->begin() + medianIndex;

    // auto medianItPlusOne = field->begin() + medianIndex + 1;

    // Ende ist bei std::partition() explizit, deshalb + 1
    auto rightIt = field->begin() + rightIndex + 1;

    std::partition(leftIt, rightIt, medianCompare);

    // Nach der Partitionierung, muss innerhalb der Partitionierungen nochmals
    // der Größe nach sortiert werden, damit später wieder der Median geholt
    // werden kann, in dem man auf den halben Index zugreift.
    std::sort(leftIt, medianIt, sortCompare);
    std::sort(medianIt + 1, rightIt, sortCompare);
  }

  void constructBalanced2DTree(const int leftIndex, const int rightIndex,
                               Node* p, const bool isVertical)
  {
    if (leftIndex <= rightIndex) {
      // +1 um aufzurunden
      int medianIndex = (leftIndex + rightIndex + 1) / 2;

      if (isVertical) {
        QPointF median = (*y)[medianIndex];
        p->value = median;

        const auto compare = [median](const QPointF& point) {
          return point.y() <= median.y();
        };

        partitionField(x, leftIndex, medianIndex, rightIndex, compare,
                       LessXComparator());
      } else {
        QPointF median = (*x)[medianIndex];
        p->value = median;

        const auto compare = [median](const QPointF& point) {
          return point.x() <= median.x();
        };

        partitionField(y, leftIndex, medianIndex, rightIndex, compare,
                       LessYComparator());
      }

      if (leftIndex != rightIndex) {
        p->left = new Node();
        p->right = new Node();

        constructBalanced2DTree(leftIndex, medianIndex - 1, p->left,
                                !isVertical);
        constructBalanced2DTree(medianIndex + 1, rightIndex, p->right,
                                !isVertical);
      }
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

  void insert(std::vector<T>* _x, std::vector<T>* _y)
  {
    x = _x;
    y = _y;

    std::sort(x->begin(), x->end(), LessXComparator());
    std::sort(y->begin(), y->end(), LessYComparator());

    constructBalanced2DTree(0, x->size() - 1, root, true);
  }

  bool contains(T value) { return contains(value, root); }

  T findMax() { findMax(root)->value; }
};

#endif // AVLTREE_H
