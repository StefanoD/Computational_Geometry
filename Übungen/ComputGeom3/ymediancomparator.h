#ifndef YMEDIANCOMPARATOR
#define YMEDIANCOMPARATOR

#include <QPointF>

class YMedianComparator
{
  int yMedian;

public:
  YMedianComparator(int _yMedian)
    : yMedian(_yMedian)
  {
  }

  bool operator<(const QPointF point) { return yMedian < point.y(); }
};

#endif // YMEDIANCOMPARATOR
