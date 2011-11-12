#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jni.h"
#include "jvmti.h"

static jvmtiEnv *jvmti = NULL;
static jvmtiCapabilities capa;
static jlong combined_size;

static int num_class_refs;
static int num_field_refs;
static int num_array_refs;
static int num_classloader_refs;
static int num_signer_refs;
static int num_protection_domain_refs;
static int num_interface_refs;
static int num_static_field_refs;
static int num_constant_pool_refs;

/* Global agent data structure */

typedef struct
{
    jvmtiEnv      *jvmti;
    jboolean       vm_is_started;
    jrawMonitorID  lock;
} GlobalAgentData;
static GlobalAgentData *gdata;



static void check_jvmti_error(jvmtiEnv *jvmti, jvmtiError errnum, const char *str)
{
    if ( errnum != JVMTI_ERROR_NONE )
    {
        char *errnum_str;
        errnum_str = NULL;
        (void)(jvmti)->GetErrorName(errnum, &errnum_str);
        printf("ERROR: JVMTI: %d(%s): %s\n", errnum, (errnum_str==NULL?"Unknown":errnum_str),(str==NULL?"":str));
    }
}

/* Enter a critical section by doing a JVMTI Raw Monitor Enter */
static void enter_critical_section(jvmtiEnv *jvmti)
{
    jvmtiError error;
    error = (jvmti)->RawMonitorEnter(gdata->lock);
    check_jvmti_error(jvmti, error, "Cannot enter with raw monitor");
}

/* Exit a critical section by doing a JVMTI Raw Monitor Exit */
static void exit_critical_section(jvmtiEnv *jvmti)
{
    jvmtiError error;
    error = (jvmti)->RawMonitorExit(gdata->lock);
    check_jvmti_error(jvmti, error, "Cannot exit with raw monitor");
}

/* Get a name for a jthread */
static void get_thread_name(jvmtiEnv *jvmti, jthread thread, char *tname, int maxlen)
{
    jvmtiThreadInfo info;
    jvmtiError error;
    /* Make sure the stack variables are garbage free */
    (void)memset(&info,0, sizeof(info));
    /* Assume the name is unknown for now */
    (void)strcpy(tname, "Unknown");
    /* Get the thread information, which includes the name */
    error = (jvmti)->GetThreadInfo(thread, &info);
    check_jvmti_error(jvmti, error, "Cannot get thread info");
    /* The thread might not have a name, be careful here. */
    if ( info.name != NULL )
    {
        int len;
        /* Copy the thread name into tname if it will fit */
        len = (int)strlen(info.name);
        if ( len < maxlen )
        {
            (void)strcpy(tname, info.name);
        }
        /* Every string allocated by JVMTI needs to be freed */
            error = (jvmti)->Deallocate((unsigned char *)(info.name));
            if (error != JVMTI_ERROR_NONE)
            {
                printf("(get_thread_name) Error expected: %d, got: %d\n", JVMTI_ERROR_NONE, error);
                printf("\n");
            }
    }
}



// VM Death callback
static void JNICALL callbackVMDeath(jvmtiEnv *jvmti_env, JNIEnv* jni_env)
{
    enter_critical_section(jvmti);
    {
        printf("Got VM Death event\n");
    }
    exit_critical_section(jvmti);
}
// Exception callback





static void callbackMonitorContendedEnter(jvmtiEnv *jvmti_env,JNIEnv* jni_env,jthread thr,jobject object)
 {
    enter_critical_section(jvmti_env);
    {
      jvmtiError err, err1, err2;
      jvmtiThreadInfo info1;
      jvmtiFrameInfo frames[5];
      jint count;
      jint thr_count;
      jthread *thr_ptr;
      jvmtiError err3;

      jint flag = 0;

      jint owned_monitor_count_ptr, thr_st_ptr;
      jobject *owned_monitors_ptr;


      /* Get All Threads */
     err = (jvmti)->GetAllThreads(&thr_count, &thr_ptr);
     if (err != JVMTI_ERROR_NONE)
     {
          printf("(GetThreadInfo) Error expected: %d, got: %d\n", JVMTI_ERROR_NONE, err);
          printf("\n");
      }

      if (err == JVMTI_ERROR_NONE && thr_count >= 1)
      {
        int i = 0;
        printf("Thread Count: %d\n", thr_count);
        for ( i=0; i < thr_count; i++)
        {

            /* Make sure the stack variables are garbage free */
            (void)memset(&info1,0, sizeof(info1));
            err1 = (jvmti)->GetThreadInfo(thr_ptr[i], &info1);

            err1 = (jvmti)->GetOwnedMonitorInfo(thr_ptr[i], &owned_monitor_count_ptr, &owned_monitors_ptr);

            err1 = (jvmti)->GetThreadState(thr_ptr[i], &thr_st_ptr);


            if (err1 != JVMTI_ERROR_NONE)
            {
                printf("(GetThreadInfo) Error expected: %d, got: %d\n", JVMTI_ERROR_NONE, err1);
                printf("\n");
            }

            printf("\n=====================================\n");
            printf("Thread %d : %s\n",i+1,info1.name);
            printf("=====================================\n");

            //printf("Priority: %d  \ncontext class loader:%s\n", info1.priority,(info1.context_class_loader == NULL ? ": NULL" : "Not Null"));

            printf("Number of Monitors Owned by this thread : %d\n\nSTATE:\n", owned_monitor_count_ptr);






            if  ( thr_st_ptr & JVMTI_THREAD_STATE_ALIVE) {
                        printf("Thread %s is Alive\n", info1.name);
                            flag = 1;
                    }

                    if  ( thr_st_ptr & JVMTI_THREAD_STATE_TERMINATED) {

                       printf("Thread %s has been Terminated\n", info1.name);
                            flag = 1;
                    }

                    if ( thr_st_ptr & JVMTI_THREAD_STATE_RUNNABLE ) {

                       printf("Thread %s is Runnable\n", info1.name);
                            flag = 1;
                    }

                    if ( thr_st_ptr & JVMTI_THREAD_STATE_WAITING ) {
                       printf("Thread %s waiting\n", info1.name);
                            flag = 1;
                    }

                    if ( thr_st_ptr & JVMTI_THREAD_STATE_WAITING_INDEFINITELY ) {
                       printf("Thread %s waiting indefinitely\n", info1.name);
                            flag = 1;
                    }

                    if ( thr_st_ptr & JVMTI_THREAD_STATE_WAITING_WITH_TIMEOUT ) {
                       printf("Thread %s waiting with Timeout\n", info1.name);
                            flag = 1;
                    }

                    if ( thr_st_ptr & JVMTI_THREAD_STATE_SLEEPING ) {
                       printf("Thread %s Sleeping \n", info1.name);
                            flag = 1;
                    }


                    if ( thr_st_ptr & JVMTI_THREAD_STATE_IN_OBJECT_WAIT ) {
                       printf("Thread %s is in Object Wait \n", info1.name);
                            flag = 1;
                    }
                    if ( thr_st_ptr & JVMTI_THREAD_STATE_PARKED ) {
                       printf("Thread %s is Parked \n", info1.name);
                            flag = 1;
                    }
                    if ( thr_st_ptr & JVMTI_THREAD_STATE_BLOCKED_ON_MONITOR_ENTER ) {
                       printf("Thread %s is blocked on monitor enter \n", info1.name);
                            flag = 1;
                    }
                    if ( thr_st_ptr & JVMTI_THREAD_STATE_SUSPENDED ) {
                       printf("Thread %s is Suspended \n", info1.name);
                            flag = 1;
                    }
                    if ( thr_st_ptr & JVMTI_THREAD_STATE_INTERRUPTED ) {
                       printf("Thread %s is Interrupted \n", info1.name);
                            flag = 1;
                    }
                    if ( thr_st_ptr & JVMTI_THREAD_STATE_IN_NATIVE ) {

                       printf("Thread %s is in Native \n", info1.name);
                            flag = 1;
                    }

                    if ( flag != 1 )  {
                               printf("Illegal value  %d for Thread State\n", thr_st_ptr);
                           }






































            /* Every string allocated by JVMTI needs to be freed */
            err2 = (jvmti)->Deallocate((unsigned char*)info1.name);

            if (err2 != JVMTI_ERROR_NONE)
            {
                printf("(GetThreadInfo) Error expected: %d, got: %d\n", JVMTI_ERROR_NONE, err2);
                printf("\n");
            }
            err3 = (jvmti)->GetStackTrace(thr, 0, 5, (jvmtiFrameInfo *)&frames, &count);

            if (err3 != JVMTI_ERROR_NONE)
            {
                 printf("(GetThreadInfo) Error expected: %d, got: %d\n", JVMTI_ERROR_NONE, err3);
                 printf("\n");
             }
              // printf("Number of records filled: %d\n", count);

            if (err3 == JVMTI_ERROR_NONE && count >=1)
            {
               char *methodName;
               char *declaringClassName;
               jclass declaring_class;
               int i=0;


               printf("\n\nStack Trace\n");
               printf("-----------\n");
               printf("Stack Trace Depth: %d\n", count);
               for ( i=0; i < count; i++)
               {
                       err3 = (jvmti)->GetMethodName(frames[i].method, &methodName, NULL, NULL);
                       if (err3 == JVMTI_ERROR_NONE)
                       {
                                err3 = (jvmti)->GetMethodDeclaringClass(frames[i].method, &declaring_class);
                                err3 = (jvmti)->GetClassSignature(declaring_class, &declaringClassName, NULL);

                                if (err3 == JVMTI_ERROR_NONE)
                                {
                                        printf("at method %s() in class %s\n", methodName, declaringClassName);
                                }
                                else {printf("\nError found !\n");}
                       }
               }
               printf("\n");
               err3 = (jvmti)->Deallocate((unsigned char*)methodName);
               err3 = (jvmti)->Deallocate((unsigned char*)declaringClassName);

           }
        }
      }
 } exit_critical_section(jvmti_env);
 }





// VM init callback
static void JNICALL callbackVMInit(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread)
{
    enter_critical_section(jvmti);
    {
        char  tname[512];
        jvmtiError error;

        /* The VM has started. */
        printf("Got VM init event\n");
        get_thread_name(jvmti_env , thread, tname, sizeof(tname));
        printf("callbackVMInit:  %s thread\n", tname);
        error = (jvmti)->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION, (jthread)NULL);
        check_jvmti_error(jvmti_env, error, "Cannot set event notification");
    }
    exit_critical_section(jvmti);
}





//static void JNICALL callbackVMObjectAlloc(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jobject object, jclass object_klass, jlong size)
//{
//    enter_critical_section(jvmti_env);
//    {

//    }
//    exit_critical_section(jvmti_env);
//}







JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *vm, char *options, void *reserved)
{
    printf("\nAgent_OnLoad : Agent Loaded !\n");
    static GlobalAgentData data;
    jvmtiError error;
    jint res;
    jvmtiEventCallbacks callbacks;

    //Memory allocation to GlobalAgentData
    (void)memset((void*)&data, 0, sizeof(data));
    gdata = &data;

    //First get the JVMTI environment
    res = (vm)->GetEnv((void **) &jvmti, JVMTI_VERSION_1_0);
    if (res != JNI_OK || jvmti == NULL)
    {
        printf("VM was unable to obtain this version of the JVMTI interface, FATAL ERROR!!");
    }

    //save the jvmtiEnv* for Agent_OnUnload()
    gdata->jvmti = jvmti;

    //Allocate memory for JVMTI_Capabilities and set the flags.
    (void)memset(&capa, 0, sizeof(jvmtiCapabilities));
     capa.can_signal_thread = 1;
     capa.can_get_owned_monitor_info = 1;
     capa.can_generate_method_entry_events = 1;
     capa.can_generate_exception_events = 1;
     capa.can_generate_vm_object_alloc_events = 1;
     capa.can_tag_objects = 1;
     capa.can_generate_monitor_events=1;

     error = (jvmti)->AddCapabilities(&capa);
     check_jvmti_error(jvmti, error, "Unable to get necessary JVMTI capabilities.");


     //Memory allocation to call back event
     (void)memset(&callbacks, 0, sizeof(callbacks));

      callbacks.VMInit = &callbackVMInit;
      callbacks.VMDeath = &callbackVMDeath;
      callbacks.MonitorContendedEnter = &callbackMonitorContendedEnter;

      error = (jvmti)->SetEventCallbacks(&callbacks, (jint)sizeof(callbacks));
      check_jvmti_error(jvmti, error, "ERROR in setting jvmti callbacks");

      error = (jvmti)->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, (jthread)NULL);
      error = (jvmti)->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_DEATH, (jthread)NULL);
      error = (jvmti)->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_MONITOR_CONTENDED_ENTER, (jthread)NULL);


      check_jvmti_error(jvmti, error, "Cannot set event notification");

      error = (jvmti)->CreateRawMonitor("Agent Data", &(gdata->lock));
      check_jvmti_error(jvmti, error, "Cannot create raw monitor");

    return JNI_OK;
}




/* Agent_OnUnload() is called last */
JNIEXPORT void JNICALL Agent_OnUnload(JavaVM *vm)
{


}
