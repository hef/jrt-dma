QT       += core gui

TARGET = test
TEMPLATE = app
CONFIG += qtestlib
CONFIG += console

INCLUDEPATH += ../agentlib/
INCLUDEPATH += /usr/include/
SOURCES += main.cpp\
        ../agentlib/mainwindow.cpp\
        testmainwindow.cpp\
        testgraph.cpp

HEADERS  += testmainwindow.h\
         ../agentlib/mainwindow.h \
        ../agentlib/graph.h\
        testgraph.h

FORMS += ../agentlib/mainwindow.ui


