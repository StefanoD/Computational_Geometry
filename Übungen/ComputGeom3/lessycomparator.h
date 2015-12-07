#ifndef LESSYCOMPARATOR
#define LESSYCOMPARATOR

#include <QPointF>

class LessYComparator
{
public:

    inline bool operator() (const QPointF& p1, const QPointF& p2)
    {
        return (p1.y() < p2.y());
    }

    bool cmp(const QPointF &p1, const QPointF &p2) {
        return p1.y() < p2.y();
    }
};

#endif // LESSYCOMPARATOR

