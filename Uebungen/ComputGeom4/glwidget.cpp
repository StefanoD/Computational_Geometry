// Convexe Hülle
// (c) Georg Umlauf, 2015

#include <QtGui>
#include <GL/glu.h>

#include "glwidget.h"
#include "mainwindow.h"


GLWidget::GLWidget(QWidget *parent) :   QGLWidget(parent)
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

    toDelaunay();

    if (!mesh.empty()) {

        for(const auto &face : mesh.faces()) {
            glBegin(GL_LINE_LOOP);
            glColor4f( 0.00f, 0.9, 0.0f, 1.0f );
            for (const auto &vertex : mesh.vertices(face)) {
                const Point p = mesh.position(vertex);
                glVertex2f(p[0], p[1]);
            }
            glEnd();
        }
    }
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

void GLWidget::toDelaunay()
{
    mesh.clear();

    // Bounding Triangle
    const Surface_mesh::Vertex v0 = mesh.add_vertex(Point(-10.0, -10.0, 0.0));
    const Surface_mesh::Vertex v1 = mesh.add_vertex(Point(0.0, 10.0, 0.0));
    const Surface_mesh::Vertex v2 = mesh.add_vertex(Point(10.0, -10.0, 0.0));

    mesh.add_triangle(v0,v1,v2);

    for (const QPointF &p : points) {
        const Surface_mesh::Face enclosingTriangle = getEnclosingTriangle(p);
        const Surface_mesh::Vertex v = mesh.split(enclosingTriangle, Point(p.x(),
                                                                           p.y(),
                                                                           0));

        for (const Surface_mesh::Halfedge &halfedge : mesh.halfedges(v)) {
            Surface_mesh::Halfedge next_halfedge = mesh.next_halfedge(halfedge);
            legalize(v, next_halfedge);
        }
    }

    // Delete Bounding Triangle
    mesh.delete_vertex(v0);
    mesh.delete_vertex(v1);
    mesh.delete_vertex(v2);
}

void GLWidget::legalize(const Surface_mesh::Vertex v, const Surface_mesh::Halfedge halfedge)
{
    const Point p0 = mesh.position(v);
    const Point p1 = mesh.position(mesh.to_vertex(halfedge));
    const Point p2 = mesh.position(mesh.from_vertex(halfedge));
    Point p3;

    const Surface_mesh::Halfedge opposite_halfedge = mesh.opposite_halfedge(halfedge);
    const Surface_mesh::Face opposite_face = mesh.face(opposite_halfedge);

    if (opposite_face.idx() != -1) {
        for (Surface_mesh::Vertex v : mesh.vertices(mesh.face(opposite_halfedge))) {
            if(mesh.position(v) != p1 && mesh.position(v) != p2) {
                p3 = mesh.position(v);
            }
        }

        if (is_in_circle(p0, p1, p2, p3)) {
            const Surface_mesh::Halfedge halfedge_ji = mesh.opposite_halfedge(halfedge);

            const Surface_mesh::Halfedge halfedge_ik = mesh.next_halfedge(halfedge_ji);
            const Surface_mesh::Halfedge halfedge_kj = mesh.next_halfedge(halfedge_ik);

            mesh.flip(mesh.edge(halfedge));

            legalize(v, halfedge_ik);
            legalize(v, halfedge_kj);
        }
    }
}



double GLWidget::tri_area(const Point &a, const Point &b, const Point &c)
{
    return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
}

bool GLWidget::is_in_circle(const Point &a, const Point &b, const Point &c, const Point &d)
{
    return (a[0] * a[0] + a[1] * a[1]) * tri_area(b, c, d) -
             (b[0] * b[0] + b[1] * b[1]) * tri_area(a, c, d) +
             (c[0] * c[0] + c[1] * c[1]) * tri_area(a, b, d) -
             (d[0] * d[0] + d[1] * d[1]) * tri_area(a, b, c) > 0;
}

Surface_mesh::Face GLWidget::getEnclosingTriangle(const QPointF &pr)
{
    Surface_mesh::Face enclosingTriangle;

    for (const Surface_mesh::Face &face : mesh.faces()) {
        QPolygonF triangle;

        for (const Surface_mesh::Vertex &v : mesh.vertices(face)) {
            const Point p = mesh.position(v);

            triangle.append(QPointF(p[0], p[1]));
        }

        if (triangle.containsPoint(pr, Qt::OddEvenFill)) {
            enclosingTriangle = face;
            break;
        }
    }

    return enclosingTriangle;
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
    QPointF clickedPoint = transformPosition(event->pos());

    if (event->buttons() & Qt::LeftButton ) {
        points.push_back(clickedPoint);
    }

    update();
}
