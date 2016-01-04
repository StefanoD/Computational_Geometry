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
        glwidget.cpp

HEADERS  += mainwindow.h\
            glwidget.h \

FORMS    += mainwindow.ui
