#ifndef XMEDIANCOMPARATOR
#define XMEDIANCOMPARATOR

#include <QPointF>

//template < typename T >
class XMedianComparator
{
  QPointF xMedian;

public:
  XMedianComparator(QPointF _xMedian)
    : xMedian(_xMedian)
  {
  }

  bool operator()(const QPointF point) { return xMedian.x() < point.x(); }
};

#endif // XMEDIANCOMPARATOR

