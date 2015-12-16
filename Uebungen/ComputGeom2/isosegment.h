#ifndef ISOSEGMENT
#define ISOSEGMENT

#include <QPointF>

/**
 * In ISO oriented segments there are only horizontal and vertical lines
 */

class IsoSegment {

public:
  IsoSegment(const QPointF &_pLeft, const QPointF &_pRight) :
      pLeft(_pLeft), pRight(_pRight) {}

  const QPointF pLeft;
  const QPointF pRight;

  // Kleiner-Ordnung
  bool operator < (const IsoSegment &seg) const
  {
      return pLeft.y() < seg.pLeft.y();
  }
};

#endif // ISOSEGMENT
