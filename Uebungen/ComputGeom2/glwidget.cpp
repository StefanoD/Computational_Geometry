// Convexe Hülle
// (c) Georg Umlauf, 2015

#include <QtGui>
#include <GL/glu.h>
#include <algorithm>

#include <memory>
#include <map>

#include "glwidget.h"
#include "mainwindow.h"
#include "isosegment.h"


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

    // Linien zeichnen
    drawSegments();

    drawSegmentsIntersections();
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
    const double scalarProduct = lineSeg1.x() * lineSeg2.x() +
                                 lineSeg1.y() * lineSeg2.y();

    return scalarProduct;
}

bool GLWidget::isHorizontalSegment(const QPointF &p1, const QPointF &p2)
{
    // Punkt, der auf der Waagrechten von Punkt p1 liegt.
    const QPointF pHorizontal = QPointF(p1.x() + 10, p1.y());

    // Repräsentiert ein horizontales Segment vom Punkt p1
    const QPointF lineSegHorizontal = QPointF(pHorizontal.x() - p1.x(),
                                              pHorizontal.y() - p1.y());

    const QPointF p2p1 = QPointF(p2.x() - p1.x(),
                                 p2.y() - p1.y());

    const double scalarProduct = getScalarProduct(lineSegHorizontal, p2p1);

    const double magnitudeSeg1 = qSqrt(lineSegHorizontal.x() * lineSegHorizontal.x() +
                                       lineSegHorizontal.y() * lineSegHorizontal.y());

    const double magnitudeSeg2 = qSqrt(p2p1.x() * p2p1.x() + p2p1.y() * p2p1.y());

    const double degree = qAcos( scalarProduct / (magnitudeSeg1 * magnitudeSeg2) ) *
                          180 / M_PI;

    return degree < 45 || degree > 135;
}

QPointF GLWidget::getLeftPoint(const QPointF &p1, const QPointF &p2)
{
    if (p1.x() < p2.x()) return p1;
    return p2;
}

QPointF GLWidget::getRightPoint(const QPointF &p1, const QPointF &p2)
{
    if (p1.x() > p2.x()) return p1;
    return p2;
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
        QPointF clickedPoint = transformPosition(event->pos());

        if (getFirstPoint) {
            getFirstPoint = false;
            lastPoint = clickedPoint;
        } else {
            getFirstPoint = true;

            std::shared_ptr<IsoSegment> isoSeg;

            if (isHorizontalSegment(lastPoint, clickedPoint)) {
                // Horizontal: Gleiche Y-Position an
                clickedPoint.ry() = lastPoint.ry();

                QPointF leftPoint = getLeftPoint(clickedPoint, lastPoint);
                QPointF rightPoint = getRightPoint(clickedPoint, lastPoint);

                // ISO-Segment auf dem Heap-Speicher erstellen
                isoSeg = std::make_shared<IsoSegment>(leftPoint, rightPoint);

                // Start- und End-Event in Vector einfügen
                events.push_back(Event(leftPoint.rx (), START_EVENT, isoSeg));
                events.push_back(Event(rightPoint.rx(), END_EVENT  , isoSeg));
            } else {
                // Vertikal: Gleiche X-Position an
                clickedPoint.rx() = lastPoint.rx();

                // ISO-Segment auf dem Heap-Speicher erstellen
                isoSeg = std::make_shared<IsoSegment>(lastPoint, clickedPoint);
                events.push_back(Event(clickedPoint.rx(), VERTICAL, isoSeg));
            }

            segments.push_back(isoSeg);
        }
        points.push_back(clickedPoint);
    }

    update();
}

void GLWidget::drawSegments()
{
    glColor4f( 0.0f, 0.407, 0.95f, 1.0f );

    for (auto &segment : segments) {
        glBegin(GL_LINE_STRIP);

        glVertex2f( segment->pLeft.x(), segment->pLeft.y() );
        glVertex2f( segment->pRight.x(), segment->pRight.y() );

        glEnd();
    }
}

void GLWidget::drawSegmentsIntersections()
{
    std::map<double, std::shared_ptr<IsoSegment>> activeSegments;

    std::sort(events.begin(), events.end());

    for (auto &event : events)
    {
        // Key bei horizontalen Linien --> yLower == yUpper
        const double y = event.isoSeg->pLeft.y();

        if (event.eventType == START_EVENT)
        {
            activeSegments[y] = event.isoSeg;
        }
        else if (event.eventType == END_EVENT)
        {
            activeSegments.erase(y);
        }
        else // VERTICAL_EVENT
        {
            double yVerticalLower = qMin(event.isoSeg->pLeft.y(), event.isoSeg->pRight.y());

            // Suche kleinstes Element das größer als y ist
            // O(log n)
            auto itGreaterThan = activeSegments.upper_bound(yVerticalLower);

            // Nichts gefunden
            if (itGreaterThan == activeSegments.end()) continue;

            // Für >= Operator um eins dekrementieren und auf Gleichheit testen
            auto itPrev = itGreaterThan;
            --itPrev;

            if (itGreaterThan != activeSegments.begin() && itPrev->second->pLeft.y() == y ) {
                // Größer-Gleich-Iterator
                --itGreaterThan;
            }

            const double yVerticalUpper = qMax(event.isoSeg->pLeft.y(), event.isoSeg->pRight.y());
            const double xVertical = event.isoSeg->pLeft.x();

            glBegin( GL_POINTS );
            glColor4f( 1.0, 1.0f, 0.0f, 1.0f );

            for (; itGreaterThan != activeSegments.end(); ++itGreaterThan) {
                std::shared_ptr<IsoSegment> horizontalSegment = itGreaterThan->second;

                // Schnittpunkt zeichnen
                if (horizontalSegment->pLeft.y() <= yVerticalUpper)
                {
                    glVertex2f( xVertical, horizontalSegment->pLeft.y() );
                }
                else // Horizontales Segment liegt höher als das Vertikale
                {
                    break;
                }
            }

            glEnd();
        }
    }
}
