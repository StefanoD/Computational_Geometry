// Convexe Hülle
//
// Widget für Interaktion und Kontrolle
//
// (c) Georg Umlauf, 2014

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <vector>
#include "surface_mesh/Surface_mesh.h"

using namespace surface_mesh;

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

private:

    QPointF transformPosition (const QPoint &p);

    void toDelaunay(std::vector<QPointF> &sites);

    Surface_mesh::Face getEnclosingTriangle(const QPointF &pr);

    std::vector<QPointF> points;

    double  aspectx, aspecty;

    Surface_mesh mesh;
};



#endif // GLWIDGET_H
