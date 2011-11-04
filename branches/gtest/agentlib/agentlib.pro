TEMPLATE = lib
TARGET = DDAgent
CONFIG += dll
CONFIG += console
include("../qmake/java_home.inc")
DEFINES += DDAGENT_LIBRARY
DEFINES += QT_NODLL

SOURCES += ddagent.cpp

HEADERS += ddagent.h\
        DDAgent_global.h


