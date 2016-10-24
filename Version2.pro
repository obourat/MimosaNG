#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T10:28:21
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS_WARN_ON += -Wno-reoder

TARGET = Version2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filereader.cpp \
    datamanager.cpp \
    model.cpp \
    dataviewer.cpp \
    optionsviewer.cpp \
    descriptivecard.cpp \
    searchcard.cpp \
    filewriter.cpp

HEADERS  += mainwindow.h \
    filereader.h \
    datamanager.h \
    model.h \
    dataviewer.h \
    optionsviewer.h \
    descriptivecard.h \
    searchcard.h \
    filewriter.h

FORMS    += mainwindow.ui \
    dataviewer.ui \
    optionsviewer.ui \
    descriptivecard.ui \
    searchcard.ui
