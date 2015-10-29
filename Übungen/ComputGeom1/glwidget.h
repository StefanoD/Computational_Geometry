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

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget                  (QWidget *parent=0);
    ~GLWidget                 ();
signals:
    void continueRequest      ();
public slots:
    void radioButtonGrahamClicked  ();
    void radioButtonJarvisClicked  ();
protected:
    void paintGL              ();
    void initializeGL         ();
    void resizeGL             (int width, int height);
    void keyPressEvent        (QKeyEvent   *event);
    void mousePressEvent      (QMouseEvent *event);

    void drawConvexHull       ();
private:
    bool doJarvisScan = false;
    bool doGrahamScan = false;

    QPointF transformPosition (const QPoint &p);

    double getScalarProduct      (const QPointF &lineSeg1,
                                  const QPointF &lineSeg2);

    double getAngleRad           (const QPointF &p1,
                               const QPointF &p2,
                               const QPointF &p3);

    QPointF transposePosition (const QPointF &p);

    bool isLeftTurn           (std::vector<QPointF> &points);
    bool isLeftTurn           (const QPointF &lineSeg1,
                               const QPointF &lineSeg2);

    QPointF getLeftMostPoint        ();

    std::vector<QPointF> grahamScan();
    std::vector<QPointF> JarvisScan();

    std::vector<QPointF> points;

    double  aspectx, aspecty;
};



#endif // GLWIDGET_H
