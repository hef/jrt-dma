#include "ddagent.h"
#include <QDebug>
#include <jvmti.h>

//see http://hope.nyc.ny.us/~lprimak/java/demo/jvmti/waiters/src/Agent.cpp
//A lot of code was pulled from that file.

static void JNICALL monitor_contended_enter(jvmtiEnv* jvmti, JNIEnv *env, jthread thread, jobject object);
static void JNICALL monitor_contended_entered(jvmtiEnv* jvmti, JNIEnv *env, jthread thread, jobject object);
static void JNICALL vm_init(jvmtiEnv *jvmti, JNIEnv *env, jthread thread);
JNIEXPORT jint JNICALL
Agent_OnLoad(JavaVM *vm, char *options, void *reserved)
{
    jvmtiEnv           *jvmti;
    //jint                rc;
    //jvmtiError          err;
    jvmtiCapabilities   capabilities;
    jvmtiEventCallbacks callbacks;
    vm->GetEnv((void **)&jvmti, JVMTI_VERSION);
    jvmti->GetCapabilities(&capabilities);
    capabilities.can_generate_monitor_events    = 1;
    capabilities.can_get_monitor_info           = 1;
    capabilities.can_tag_objects                = 1;
    jvmti->AddCapabilities(&capabilities);
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.VMInit                  = &vm_init;
    callbacks.MonitorContendedEnter   = &monitor_contended_enter;
    callbacks.MonitorContendedEntered = &monitor_contended_entered;
    jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, NULL);
    qDebug() << "DDAgent Loaded";
    // Might need to get moved to vm_init.
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_MONITOR_CONTENDED_ENTER, NULL);
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_MONITOR_CONTENDED_ENTERED, NULL);
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
static void JNICALL
vm_init(jvmtiEnv *jvmti, JNIEnv *env, jthread thread)
{
    qDebug() << "vm_init called";
}

static void JNICALL
monitor_contended_enter(jvmtiEnv* jvmti, JNIEnv *env, jthread thread, jobject object)
{
    qDebug() << "monitor_contended_enter called";
}
static void JNICALL
monitor_contended_entered(jvmtiEnv* jvmti, JNIEnv *env, jthread thread, jobject object)
{
    qDebug() << "monitor_contended_entered called";
}
DDAgent::DDAgent()
{
}
