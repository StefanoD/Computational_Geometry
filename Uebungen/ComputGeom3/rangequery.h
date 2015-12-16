#ifndef RANGEQUERY
#define RANGEQUERY

#include <QPointF>

class RangeQuery {
public:
    QPointF p1;
    QPointF p2;

    RangeQuery(QPointF _p1, QPointF _p2):
        p1(_p1),
        p2(_p2)
    {}
};

#endif // RANGEQUERY

