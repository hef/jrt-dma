QT       += core gui

TARGET = test
TEMPLATE = app
CONFIG += qtestlib

INCLUDEPATH += ../agentlib/
SOURCES += main.cpp\
        ../agentlib/mainwindow.cpp\
        testmainwindow.cpp

HEADERS  += testmainwindow.h\
         ../agentlib/mainwindow.h

FORMS += ../agentlib/mainwindow.ui
