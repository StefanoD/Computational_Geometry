// Convexe Hülle
//
// Widget für Interaktion und Kontrolle
//
// (c) Georg Umlauf, 2014

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QFuture>
#include <vector>
#include "event.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget                  (QWidget *parent=0);
    ~GLWidget                 ();
signals:
    void continueRequest      ();

protected:
    void paintGL              ();
    void initializeGL         ();
    void resizeGL             (int width, int height);
    void keyPressEvent        (QKeyEvent   *event);
    void mousePressEvent      (QMouseEvent *event);

private:

    QPointF transformPosition (const QPoint &p);

    double getScalarProduct      (const QPointF &lineSeg1,
                                  const QPointF &lineSeg2);

    bool isHorizontalSegment     (const QPointF &p1, const QPointF &p2);
    QPointF getLeftPoint         (const QPointF &p1, const QPointF &p2);
    QPointF getRightPoint        (const QPointF &p1, const QPointF &p2);

    std::vector<QPointF> points;
    std::vector<std::shared_ptr<IsoSegment>> segments;
    std::vector<Event> events;

    double  aspectx, aspecty;

    QPointF lastPoint;

    bool getFirstPoint = true;
    bool getSecondPoint = false;

    void drawSegments       ();
    void drawSegmentsIntersections();
};



#endif // GLWIDGET_H
