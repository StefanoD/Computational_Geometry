#ifndef GRAHAMCOMPARATOR_H
#define GRAHAMCOMPARATOR_H

#include <QPointF>

class GrahamComparator
{
public:

    inline bool operator() (const QPointF& p1, const QPointF& p2)
    {
        return (p1.x() < p2.x());
    }
};

#endif // GRAHAMCOMPARATOR_H
