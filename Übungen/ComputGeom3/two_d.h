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
#include <tuple>
#include "lessxcomparator.h"
#include "lessycomparator.h"

class TwoDTree
{
public:
  class Node
  {
  public:
    QPointF value;

    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    bool isVertical = true;

    Node(QPointF v = QPointF())
      : value(v)
    {
    }

    ~Node()
    {
      delete left;
      delete right;
    }
  };

  Node* root = nullptr;

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

      int medianIndex = partY->size() / 2;

      if (*p == nullptr) {
        *p = new Node();
        (**p).parent = parent;
        (**p).isVertical = isVertical;
      }

      QPointF median;
      std::vector<QPointF> partX1, partX2;
      std::vector<QPointF> partY1, partY2;

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
  ~TwoDTree() { delete root; }

  TwoDTree()
    : root(new Node())
  {
  }

  void insert(std::vector<QPointF>* x, std::vector<QPointF>* y)
  {
    std::sort(x->begin(), x->end(), LessXComparator());
    std::sort(y->begin(), y->end(), LessYComparator());

    constructBalanced2DTree(y, x, &root, nullptr, true);
  }
};

#endif // AVLTREE_H
