TEMPLATE = app
TARGET = test
CONFIG += console

#DEFINES += DDAGENT_LIBRARY
#DEFINES += QT_NODLL

SOURCES += gtest/gtest-all.cc gtest/gtest_main.cc

HEADERS += gtest/gtest.h
INCLUDEPATH += "../agentlib/

INCLUDEPATH += "$$(JAVA_HOME)/include"
unix:INCLUDEPATH += "$$(JAVA_HOME)/include/linux"
win32:INCLUDEPATH += "$$(JAVA_HOME)/include/win32"
