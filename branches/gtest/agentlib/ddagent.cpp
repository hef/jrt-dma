#include "ddagent.h"
#include <QDebug>
#include <jvmti.h>
JNIEXPORT jint JNICALL
Agent_OnLoad(JavaVM *vm, char *options, void *reserved)
{
    qDebug() << "DDAgent Loaded";
    return 0;
}
JNIEXPORT jint JNICALL
Agent_OnAttach(JavaVM *vm, char *options, void *reserved)
{
    qDebug() << "DDAgent Attached";
    return 0;
}
JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved)
{
    qDebug() << "DDAgent unLoaded";
    return;
}

DDAgent::DDAgent()
{
}
