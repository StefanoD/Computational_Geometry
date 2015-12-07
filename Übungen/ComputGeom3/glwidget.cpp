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
    drawPartitions();
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

        points.push_back(clickedPoint);
    }

    update();
}

void GLWidget::inOrder(typename AVLTree<QPointF>::Node* n) {
    if ( n != nullptr ) {
        inOrder(n->left);

        if (n->isVertical) {
            double mostRightXInPartition = n->value.x();
            double mostLeftXInPartition = n->value.x();

            for (AVLTree<QPointF>::Node *parent = n->parent; parent != nullptr; parent = parent->parent) {
                if (!parent->isVertical && parent->value.x() > mostRightXInPartition) {
                    mostRightXInPartition = parent->value.x();
                    break;
                }
            }

            for (AVLTree<QPointF>::Node *parent = n->parent; parent != nullptr; parent = parent->parent) {
                if (!parent->isVertical && parent->value.x() < mostLeftXInPartition) {
                    mostLeftXInPartition = parent->value.x();
                    break;
                }
            }

            double leftX = mostLeftXInPartition;
            double rightX = mostRightXInPartition;

            if ( n->value.x() <= mostLeftXInPartition) {
                leftX = -1.0;
            }
            if ( n->value.x() >= mostRightXInPartition) {
                rightX = 1;
            }

            glBegin(GL_LINE_STRIP);

            glVertex2f( leftX, n->value.y() );
            glVertex2f( rightX, n->value.y() );

            glEnd();
       } else {
            double highestYInPartition = n->value.y();
            double lowestYInPartition = n->value.y();

            for (AVLTree<QPointF>::Node *parent = n->parent; parent != nullptr; parent = parent->parent) {
                if (parent->isVertical && parent->value.y() > highestYInPartition) {
                    highestYInPartition = parent->value.y();
                    break;
                }
            }

            for (AVLTree<QPointF>::Node *parent = n->parent; parent != nullptr; parent = parent->parent) {
                if (parent->isVertical && parent->value.y() < lowestYInPartition) {
                    lowestYInPartition = parent->value.y();
                    break;
                }
            }

            double lowerY = lowestYInPartition;
            double upperY = highestYInPartition;

            if ( n->value.y() >= highestYInPartition ) {
                upperY = 1.0;
            }
            if ( n->value.y() <= lowestYInPartition ) {
                lowerY = -1;
            }

            glBegin(GL_LINE_STRIP);

            glVertex2f( n->value.x(), lowerY );
            glVertex2f( n->value.x(), upperY );

            glEnd();
       }

       inOrder(n->right);
    }
}


void GLWidget::drawPartitions()
{
    if (points.size() == 0) return;

    glColor4f( 0.0f, 0.407, 0.95f, 1.0f );

    xPoints = points;
/*
    std::vector<QPointF> xPoints = {
      QPointF(-0.4690721835232112, -0.5309278350515463),
      QPointF(0.2886598052450531, -0.31443298969072164),
      QPointF(0.40206187159132367, -0.6855670103092784),
      QPointF(0.5515464135932261, -0.3608247422680413),
      QPointF(-0.1649484601400303, -0.12886597938144329),
      QPointF(-0.6030928073869857, 0.7010309278350515),
      QPointF(-0.39175259283257197, 0.30927835051546393),
      QPointF(0.7061855949745045, 0.2938144329896907),
      QPointF(0.5154639379375946, 0.711340206185567),
    };

    points = xPoints;*/


    twoDTree.insert(&xPoints, &points);

    inOrder(twoDTree.root);
}
