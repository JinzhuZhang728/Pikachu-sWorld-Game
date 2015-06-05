#-------------------------------------------------
#
# Project created by QtCreator 2015-04-23T23:04:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = PacMan1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gameboard.cpp \
    gameboardpieces.cpp

HEADERS  += mainwindow.h \
    gameboard.h \
    gameboardpieces.h

FORMS    += mainwindow.ui \
    gameboard.ui \
    gameboardpieces.ui

CONFIG += c++11

DISTFILES +=

RESOURCES += \
    images.qrc
