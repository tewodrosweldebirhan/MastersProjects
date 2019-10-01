#-------------------------------------------------
#
# Project created by QtCreator 2018-11-08T01:23:37
# Developer: Tewodros W. Arega
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HarrisDetector
TEMPLATE = app

LIBS  += -lopengl32

SOURCES += main.cpp\
        window.cpp \
    myglwidget.cpp \
    face.cpp \
    harrisoperator.cpp \
    mesh.cpp \
    vertex.cpp

HEADERS  += window.h \
    myglwidget.h \
    face.h \
    harrisoperator.h \
    mesh.h \
    vertex.h

FORMS    += window.ui

INCLUDEPATH += "D:\\Downlaods\\eigen-eigen-b3f3d4950030\\eigen-eigen-b3f3d4950030"
