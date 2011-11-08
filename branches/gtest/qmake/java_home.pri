#This is a qmake include that sets JAVA_HOME

JAVA_HOME = $$(JAVA_HOME)
isEmpty( JAVA_HOME )
{
    macx:JAVA_HOME="/Library/Java/Home"
}

INCLUDEPATH += "$${JAVA_HOME}/include"
unix:INCLUDEPATH += "$${JAVA_HOME}/include/linux"
win32:INCLUDEPATH += "$${JAVA_HOME}/include/win32"
