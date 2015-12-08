#-------------------------------------------------
#
# Project created by QtCreator 2015-11-25T21:28:26
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS_DEBUG += -O0

TARGET = ComputGeom1

LIBS += -lGLU

INCLUDEPATH += -L/usr/lib/

TEMPLATE = app

SOURCES +=  main.cpp \
            mainwindow.cpp \
            glwidget.cpp

HEADERS +=  mainwindow.h \
            ViewEventHandler.h \
            glwidget.h \
    lessxcomparator.h \
    lessycomparator.h \
    two_d.h \
    rangequery.h

FORMS   += mainwindow.ui
