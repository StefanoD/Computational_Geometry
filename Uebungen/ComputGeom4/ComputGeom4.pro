#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T20:41:36
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS_DEBUG += -O0

LIBS += -lGLU

TARGET = ComputGeom4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        glwidget.cpp \
        surface_mesh/IO.cpp \
        surface_mesh/IO_obj.cpp \
        surface_mesh/IO_off.cpp \
        surface_mesh/IO_poly.cpp \
        surface_mesh/IO_stl.cpp \
        surface_mesh/Surface_mesh.cpp


HEADERS  += mainwindow.h\
            glwidget.h \
            surface_mesh/IO.h \
            surface_mesh/properties.h \
            surface_mesh/Surface_mesh.h \
            surface_mesh/types.h \
            surface_mesh/Vector.h

FORMS    += mainwindow.ui
