#-------------------------------------------------
#
# Project created by QtCreator 2011-10-27T22:35:17
#
#-------------------------------------------------

TARGET = DDAgent
TEMPLATE = lib
CONFIG += dll
CONFIG += console

DEFINES += DDAGENT_LIBRARY
DEFINES += QT_NODLL

SOURCES += ddagent.cpp

HEADERS += ddagent.h\
        DDAgent_global.h

INCLUDEPATH += "$$(JAVA_HOME)/include"
unix:INCLUDEPATH += "$$(JAVA_HOME)/include/linux"
win32:INCLUDEPATH += "$$(JAVA_HOME)/include/win32"
