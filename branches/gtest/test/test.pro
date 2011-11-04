TEMPLATE = console
TARGET = test
CONFIG += dll
CONFIG += console

#DEFINES += DDAGENT_LIBRARY
#DEFINES += QT_NODLL

SOURCES += gtest-all.cc gtest_main.cc

HEADERS += gtest.h

#INCLUDEPATH += "$$(JAVA_HOME)/include"
#unix:INCLUDEPATH += "$$(JAVA_HOME)/include/linux"
#win32:INCLUDEPATH += "$$(JAVA_HOME)/include/win32"
