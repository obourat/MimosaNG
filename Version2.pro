#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T10:28:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Version2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filereader.cpp \
    datamanager.cpp \
    model.cpp \
    dataviewer.cpp

HEADERS  += mainwindow.h \
    filereader.h \
    datamanager.h \
    model.h \
    dataviewer.h

FORMS    += mainwindow.ui \
    dataviewer.ui
