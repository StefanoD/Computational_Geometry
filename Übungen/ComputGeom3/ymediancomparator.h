#ifndef YMEDIANCOMPARATOR
#define YMEDIANCOMPARATOR

#include <QPointF>

//template < typename T >
class YMedianComparator
{
  QPointF yMedian;

public:
  YMedianComparator(QPointF _yMedian)
    : yMedian(_yMedian)
  {
  }

  bool operator < (const QPointF point) const { return yMedian.y() < point.y(); }
};

#endif // YMEDIANCOMPARATOR
