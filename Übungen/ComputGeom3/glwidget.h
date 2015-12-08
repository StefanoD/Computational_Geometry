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
#include "two_d.h"
#include "rangequery.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget                  (QWidget *parent=0);
    ~GLWidget                 ();
signals:
    void continueRequest      ();

protected:
    void paintGL              () override;
    void initializeGL         () override;
    void resizeGL             (int width, int height) override;
    void keyPressEvent        (QKeyEvent   *event) override;
    void mousePressEvent      (QMouseEvent *event) override;
    void mouseReleaseEvent    (QMouseEvent *event) override;

private:

    QPointF transformPosition (const QPoint &p);

    std::vector<QPointF> points;
    std::vector<QPointF> xPoints;

    TwoDTree twoDTree;

    double  aspectx, aspecty;

    bool getFirstPoint = true;

    void drawPartitions       ();
    void rangeSearch(TwoDTree::Node* p, RangeQuery &rq, std::vector<QPointF> &includingPoints);
    void drawQuery();
    void drawPointsInRange();
    void inOrder(TwoDTree::Node *n);
};



#endif // GLWIDGET_H
