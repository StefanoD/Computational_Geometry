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

protected:
    void paintGL              () override;
    void initializeGL         () override;
    void resizeGL             (int width, int height) override;
    void keyPressEvent        (QKeyEvent   *event) override;
    void mousePressEvent      (QMouseEvent *event) override;

private:

    QPointF transformPosition (const QPoint &p);

    std::vector<QPointF> points;

    double  aspectx, aspecty;
};



#endif // GLWIDGET_H
