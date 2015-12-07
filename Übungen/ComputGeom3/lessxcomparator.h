#ifndef LESSXCOMPARATOR
#define LESSXCOMPARATOR

#include <QPointF>

class LessXComparator
{
public:

    inline bool operator() (const QPointF& p1, const QPointF& p2)
    {
        return (p1.x() < p2.x());
    }

    bool cmp(const QPointF &p1, const QPointF &p2) {
        return p1.x() < p2.x();
    }

    bool equiv(const QPointF &p1, const QPointF &p2) {
        return p1.x() == p2.x() && p1.y() == p2.y();
    }
};

#endif // LESSXCOMPARATOR

