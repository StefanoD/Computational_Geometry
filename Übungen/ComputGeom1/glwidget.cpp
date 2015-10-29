// Convexe Hülle
// (c) Georg Umlauf, 2015

#include <QtGui>
#include <GL/glu.h>
#include <algorithm>

#include "glwidget.h"
#include "mainwindow.h"
#include "graham_comparator.h"


GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::paintGL()
{
    // clear
    glClear(GL_COLOR_BUFFER_BIT);

    // Koordinatensystem
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINES);
    glVertex2f(-1.0, 0.0);
    glVertex2f( 1.0, 0.0);
    glVertex2f( 0.0,-1.0);
    glVertex2f( 0.0, 1.0);
    glEnd();

    // Draw points
    glBegin( GL_POINTS );
    glColor4f( 0.95f, 0.207, 0.031f, 1.0f );

    for (auto &point : points) {
        glVertex2f( point.x(), point.y() );
    }
    glEnd();

    // Konvexe Hülle zeichnen
    drawConvexHull();
}


void GLWidget::initializeGL()
{
    resizeGL(width(),height());
}

void GLWidget::resizeGL(int width, int height)
{
    aspectx=1.0;
    aspecty=1.0;
    if (width>height) aspectx = float(width) /height;
    else              aspecty = float(height)/ width;
    glViewport    (0,0,width,height);
    glMatrixMode  (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D    (-aspectx,aspectx,-aspecty,aspecty);
    glMatrixMode  (GL_MODELVIEW);
    glLoadIdentity();


    // Hinzu
    glPointSize(4.0);
    glEnable( GL_POINT_SPRITE );
    glEnable( GL_POINT_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

QPointF GLWidget::transformPosition(const QPoint &p)
{
    return QPointF( (2.0*p.x()/ width() - 1.0)*aspectx,
                    -(2.0*p.y()/height() - 1.0)*aspecty);
}

double GLWidget::getScalarProduct(const QPointF &lineSeg1,
                                  const QPointF &lineSeg2)
{
    const QPointF transpLineSeg1 = transposePosition(lineSeg1);

    const double scalarProduct = transpLineSeg1.x() * lineSeg2.x() +
                                 transpLineSeg1.y() * lineSeg2.y();

    return scalarProduct;
}

double GLWidget::getAngleRad(const QPointF &p1,
                             const QPointF &p2,
                             const QPointF &p3)
{
    const QPointF p2p1 = QPointF(p2.x() - p1.x(),
                                 p2.y() - p1.y());

    const QPointF p3p1 = QPointF(p3.x() - p1.x(),
                                 p3.y() - p1.y());

    const double scalarProduct = getScalarProduct(p2p1, p3p1);

    const double lengthP2P1 = qSqrt(p2p1.x() * p2p1.x() +
                                    p2p1.y() * p2p1.y());

    const double lengthP3P1 = qSqrt(p3p1.x() * p3p1.x() +
                                    p3p1.y() * p3p1.y());

    const double angleRad = qAcos( scalarProduct /
                            (lengthP2P1 * lengthP3P1));

    return angleRad;
}

QPointF GLWidget::transposePosition(const QPointF &p)
{
    return QPointF( p.y(),
                   -p.x());
}

bool GLWidget::isLeftTurn(std::vector<QPointF> &points)
{
    const QPointF &p1 = points[points.size() - 3];
    const QPointF &p2 = points[points.size() - 2];
    const QPointF &p3 = points[points.size() - 1];

    const QPointF p2p1 = QPointF(p2.x() - p1.x(),
                                 p2.y() - p1.y());

    const QPointF p3p1 = QPointF(p3.x() - p1.x(),
                                 p3.y() - p1.y());

    // Für Graham-Scan:
    // <= statt <, um kolineare Punkte zu überspringen
    return getScalarProduct(p2p1, p3p1) <= 0;
}

bool GLWidget::isLeftTurn(const QPointF &lineSeg1, const QPointF &lineSeg2)
{
    return getScalarProduct(lineSeg1, lineSeg2) < 0;
}

QPointF GLWidget::getLeftMostPoint()
{
    QPointF leftMostPoint;

    for (auto &point : points) {
         if (point.x() < leftMostPoint.x()) {
            leftMostPoint = point;
        }
    }

    return leftMostPoint;
}

std::vector<QPointF> GLWidget::grahamScan()
{
    // Sortierung
    std::vector<QPointF> sortedPoints = points;
    std::sort(sortedPoints.begin(), sortedPoints.end(), GrahamComparator());

    // Obere Hülle berechnen

    // Die ersten beiden Punkte in Liste einfügen
    std::vector<QPointF> hull = { sortedPoints[0], sortedPoints[1] };

    for (auto it = sortedPoints.begin() + 2; it < sortedPoints.end(); ++it) {
        QPointF &p = *it;

        hull.push_back(p);

        while (hull.size() >= 3 && isLeftTurn(hull)) {
            hull.erase(hull.end() - 2);
        }
    }

    // Untere Hülle berechnen

    // Die ersten beiden Punkte in Liste einfügen
    std::vector<QPointF> lowerHull = { sortedPoints[points.size() - 1],
                                       sortedPoints[points.size() - 2] };

    for (auto it = sortedPoints.end() - 2; it >= sortedPoints.begin(); --it) {
        QPointF &p = *it;

        lowerHull.push_back(p);

        while (lowerHull.size() >= 3 && isLeftTurn(lowerHull)) {
            lowerHull.erase(lowerHull.end() - 2);
        }
    }

    lowerHull.erase(lowerHull.begin());
    lowerHull.erase(lowerHull.end() - 1);

    // Merge Upper + Lower Hull
    hull.reserve(hull.size() + lowerHull.size());
    hull.insert(hull.end(), lowerHull.begin(), lowerHull.end());

    return hull;
}

std::vector<QPointF> GLWidget::JarvisScan()
{
    QPointF pointOnHull = getLeftMostPoint();
    std::vector<QPointF> convexHull;

    // So etwas ähnliches wie eine Gerade durch den ersten Punkt
    QPointF endpoint = QPointF(-1, pointOnHull.y());

    do {
        convexHull.push_back(pointOnHull);
        endpoint = points[0];

        for (auto it = points.begin() + 1; it < points.end(); ++it) {
            QPointF &lastPointOnHull = convexHull.back();
            QPointF &q = *it;

            QPointF lineSeg1 = QPointF(endpoint.x() - lastPointOnHull.x(),
                                         endpoint.y() - lastPointOnHull.y());

            QPointF lineSeg2 = QPointF(q.x() - lastPointOnHull.x(),
                                  q.y() - lastPointOnHull.y());

            if (endpoint == pointOnHull || isLeftTurn(lineSeg1, lineSeg2)) {
                endpoint = q;
            }
        }

        pointOnHull = endpoint;

    } while (endpoint != convexHull[0]);

    return convexHull;
}

void GLWidget::keyPressEvent(QKeyEvent * event)
{
    switch (event->key()) {
    default:
        break;
    }
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton ) {
        const QPointF posF = transformPosition(event->pos());
        points.push_back(posF);
    }

    update();
}

void GLWidget::drawConvexHull()
{
    if (points.size() < 2) return;

    std::vector<QPointF> hull;

    if (doGrahamScan) {
        hull = grahamScan();
    } else if (doJarvisScan) {
        hull = JarvisScan();
    } else {
        return;
    }

    // Konvexe Hülle zeichnen
    glBegin(GL_LINE_STRIP);

    glColor4f( 0.0f, 0.407, 0.95f, 1.0f );

    for (auto &point : hull) {
        glVertex2f( point.x(), point.y() );
    }

    // Ersten Punkt nochmals angeben, damit sich die Hülle schließt!
    glVertex2f( hull[0].x(), hull[0].y() );

    glEnd();
}


void GLWidget::radioButtonGrahamClicked()
{
    doGrahamScan = true;
    doJarvisScan = false;

    update();
}

void GLWidget::radioButtonJarvisClicked()
{
    doGrahamScan = false;
    doJarvisScan = true;

    update();
}
