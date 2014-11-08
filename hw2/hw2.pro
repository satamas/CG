#-------------------------------------------------
#
# Project created by QtCreator 2014-10-28T17:00:44
#
#-------------------------------------------------

QT       += widgets opengl

TARGET = hw1
TEMPLATE = app

SOURCES += main.cpp\
    glwidget.cpp \
    mainwindow.cpp

HEADERS  += \
    glwidget.h \
    mainwindow.h

OTHER_FILES += \
    1.fsh \
    1.vsh \
    2.fsh \
    2.vsh \
    model.obj

CONFIG += c++11

DESTDIR = $$PWD
