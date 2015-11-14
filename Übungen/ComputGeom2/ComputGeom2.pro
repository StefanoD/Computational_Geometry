QT += core gui opengl

QMAKE_CXXFLAGS += -std=c++11
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
    graham_comparator.h \
    isosegment.h \
    event.h

FORMS   += mainwindow.ui
