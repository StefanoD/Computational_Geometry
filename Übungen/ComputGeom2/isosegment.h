#ifndef ISOSEGMENT
#define ISOSEGMENT

#include <QPointF>

/**
 * In ISO oriented segments there are only horizontal and vertical lines
 */

class IsoSegment {

public:
  IsoSegment(const QPointF &_p1, const QPointF &_p2) :
      p1(_p1), p2(_p2) {}

  const QPointF p1;
  const QPointF p2;

  // Kleiner-Ordnung
  bool operator < (const IsoSegment &seg) const
  {
      return p1.y() < seg.p1.y();
  }
};

#endif // ISOSEGMENT
