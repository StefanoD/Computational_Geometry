#ifndef XMEDIANCOMPARATOR
#define XMEDIANCOMPARATOR

#include <QPointF>

class XMedianComparator
{
  int xMedian;

public:
  XMedianComparator(int _xMedian)
    : xMedian(_xMedian)
  {
  }

  bool operator<(const QPointF point) { return xMedian <= point.x(); }
};

#endif // XMEDIANCOMPARATOR

