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

SOURCES += \
    DDAgent.cpp

HEADERS += ddagent.h\
        DDAgent_global.h \
    Agent.hpp \
    Monitor.hpp \
    Thread.hpp

INCLUDEPATH += "$$(JAVA_HOME)/include"
unix:INCLUDEPATH += "$$(JAVA_HOME)/include/linux"
win32:INCLUDEPATH += "$$(JAVA_HOME)/include/win32"

















