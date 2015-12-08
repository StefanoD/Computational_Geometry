// Convexe Hülle
// (c) Georg Umlauf, 2015

#include <QtGui>
#include <GL/glu.h>

#include "glwidget.h"
#include "mainwindow.h"


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

void GLWidget::inOrder(typename TwoDTree<QPointF>::Node* n) {
    if ( n != nullptr ) {
        inOrder(n->left);

        if (n->isVertical) {
            double mostRightXInPartition = n->value.x();
            double mostLeftXInPartition = n->value.x();

            for (TwoDTree<QPointF>::Node *parent = n->parent; parent != nullptr; parent = parent->parent) {
                if (!parent->isVertical && parent->value.x() > mostRightXInPartition) {
                    mostRightXInPartition = parent->value.x();
                    break;
                }
            }

            for (TwoDTree<QPointF>::Node *parent = n->parent; parent != nullptr; parent = parent->parent) {
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

            for (TwoDTree<QPointF>::Node *parent = n->parent; parent != nullptr; parent = parent->parent) {
                if (parent->isVertical && parent->value.y() > highestYInPartition) {
                    highestYInPartition = parent->value.y();
                    break;
                }
            }

            for (TwoDTree<QPointF>::Node *parent = n->parent; parent != nullptr; parent = parent->parent) {
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

    twoDTree.insert(&xPoints, &points);

    inOrder(twoDTree.root);
}
