#ifndef RANGEQUERY
#define RANGEQUERY

#include <QPointF>

class RangeQuery {
public:
    QPointF leftUpper;
    QPointF rightBottom;

    RangeQuery(QPointF _leftUpper, QPointF _rightBottom):
        leftUpper(_leftUpper),
        rightBottom(_rightBottom)
    {}
};

#endif // RANGEQUERY

