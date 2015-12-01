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
};

#endif // LESSXCOMPARATOR

