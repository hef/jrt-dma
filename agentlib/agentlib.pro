TEMPLATE = lib
TARGET = DDAgent
CONFIG += dll
CONFIG += console

include("../qmake/java_home.pri")
DEFINES += DDAGENT_LIBRARY
DEFINES += QT_NODLL
SOURCES += ddagent.cpp \
    mainwindow.cpp

HEADERS += ddagent.h\
        DDAgent_global.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

