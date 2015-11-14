#ifndef GRAHAMCOMPARATOR_H
#define GRAHAMCOMPARATOR_H

#include <QPointF>

class GrahamComparator
{
public:

    inline bool operator() (const QPointF& p1, const QPointF& p2)
    {
        if (p1.x() == p2.x()) {
            return (p1.y() > p2.y());
        }

        return (p1.x() < p2.x());
    }
};

#endif // GRAHAMCOMPARATOR_H
