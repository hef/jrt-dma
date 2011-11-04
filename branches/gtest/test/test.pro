TEMPLATE = app
TARGET = test
CONFIG += console
include("../qmake/java_home.inc")
SOURCES += gtest/gtest-all.cc gtest/gtest_main.cc

HEADERS += gtest/gtest.h
INCLUDEPATH += "../agentlib/"
