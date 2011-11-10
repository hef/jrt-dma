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
        printf("ERROR: JVMTI: %d(%s): %s\n", errnum, (errnum_str==NULL?"Unknown":errnum_str), (str==NULL?"":str));
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
/* JVMTI callback function. */
static jvmtiIterationControl JNICALL
reference_object(jvmtiObjectReferenceKind reference_kind,
                jlong class_tag, jlong size, jlong* tag_ptr,
                jlong referrer_tag, jint referrer_index, void *user_data)
{
        combined_size = combined_size + size;
        switch (reference_kind)
        {
                case JVMTI_REFERENCE_CLASS:
                        num_class_refs = num_class_refs + 1;
                        break;
                case JVMTI_REFERENCE_FIELD:
                        num_field_refs = num_field_refs + 1;
                        break;
                case JVMTI_REFERENCE_ARRAY_ELEMENT:
                        num_array_refs = num_array_refs + 1;
                        break;
                case JVMTI_REFERENCE_CLASS_LOADER:
                        num_classloader_refs = num_classloader_refs + 1;
                        break;
                case JVMTI_REFERENCE_SIGNERS:
                        num_signer_refs = num_signer_refs + 1;
                        break;
                case JVMTI_REFERENCE_PROTECTION_DOMAIN:
                        num_protection_domain_refs = num_protection_domain_refs + 1;
                        break;
                case JVMTI_REFERENCE_INTERFACE:
                        num_interface_refs = num_interface_refs + 1;
                        break;
                case JVMTI_REFERENCE_STATIC_FIELD:
                        num_static_field_refs = num_static_field_refs + 1;
                        break;
                case JVMTI_REFERENCE_CONSTANT_POOL:
                        num_constant_pool_refs = num_constant_pool_refs + 1;
                        break;
                default:
                        break;
        }
    return JVMTI_ITERATION_CONTINUE;
}
/*
static void JNICALL callbackVMObjectAlloc(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jobject object, jclass object_klass, jlong size)
{
    char *methodName;
    char *className;
    char *declaringClassName;
    jclass declaring_class;
    jvmtiError err;
    if (size > 50) {
            err = (jvmti)->GetClassSignature(object_klass, &className, NULL);
            if (className != NULL)
            {
                    printf("\ntype %s object allocated with size %d\n", className, (jint)size);
            }
            //print stack trace
            jvmtiFrameInfo frames[5];
            jint count;
            int i;
           err = (jvmti)->GetStackTrace(NULL, 0, 5, (jvmtiFrameInfo *)&frames, &count);
            if (err == JVMTI_ERROR_NONE && count >= 1) {
                    for (i = 0; i < count; i++) {
                            err = (jvmti)->GetMethodName(frames[i].method, &methodName, NULL, NULL);
                            if (err == JVMTI_ERROR_NONE) {
                                    err = (jvmti)->GetMethodDeclaringClass(frames[i].method, &declaring_class);
                                    err = (jvmti)->GetClassSignature(declaring_class, &declaringClassName, NULL);
                                    if (err == JVMTI_ERROR_NONE) {
                                            printf("at method %s in class %s\n", methodName, declaringClassName);
                                    }
                            }
                    }
            }
            //reset counters
            combined_size  = 0;
            num_class_refs = 0;
            num_field_refs = 0;
            num_array_refs = 0;
            num_classloader_refs = 0;
            num_signer_refs = 0;
            num_protection_domain_refs = 0;
            num_interface_refs = 0;
            num_static_field_refs = 0;
            num_constant_pool_refs = 0;
            err = (jvmti)->IterateOverObjectsReachableFromObject(object, &reference_object, NULL);
            if ( err != JVMTI_ERROR_NONE ) {
                    printf("Cannot iterate over reachable objects\n");
            }
            printf("\nThis object has references to objects of combined size %d\n", (jint)combined_size);
            printf("This includes %d classes, %d fields, %d arrays, %d classloaders, %d signers arrays,\n", num_class_refs, num_field_refs, num_array_refs, num_classloader_refs, num_signer_refs);
            printf("%d protection domains, %d interfaces, %d static fields, and %d constant pools.\n\n", num_protection_domain_refs, num_interface_refs, num_static_field_refs, num_constant_pool_refs);
            err = (jvmti)->Deallocate((unsigned char*)className);
            err = (jvmti)->Deallocate((unsigned char*)methodName);
            err = (jvmti)->Deallocate((unsigned char*)declaringClassName);
    }
}
*/
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
 //static void JNICALL callbackException(jvmtiEnv *jvmti_env, JNIEnv* env, jthread thr, jmethodID method, jlocation location, jobject exception, jmethodID catch_method, jlocation catch_location)
static void callbackMonitorContendedEnter(jvmtiEnv *jvmti_env,JNIEnv* jni_env,jthread thr,jobject object)
 {
    enter_critical_section(jvmti_env);
    {
      jvmtiError err, err1, err2, error;
      jmethodID method;
      jvmtiThreadInfo info, info1;
      jvmtiThreadGroupInfo groupInfo;
      jint num_monitors;
      jobject *arr_monitors;
      jvmtiFrameInfo frames[5];
      jint count;
      jint flag = 0;
      jint thr_st_ptr;
      jint thr_count;
      jthread *thr_ptr;
      jvmtiError err3;
      char *name;
      char *sig;
      char *gsig;
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
            if (err1 != JVMTI_ERROR_NONE)
            {
                printf("(GetThreadInfo) Error expected: %d, got: %d\n", JVMTI_ERROR_NONE, err1);
                printf("\n");
            }

            printf("\n=====================================\n");
            printf("Thread %d : %s\n",i+1,info1.name);
            printf("=====================================\n");


            printf("Priority: %d  \ncontext class loader:%s\n", info1.priority,(info1.context_class_loader == NULL ? ": NULL" : "Not Null"));
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
               printf("Number of records filled: %d\n", count);
            if (err3 == JVMTI_ERROR_NONE && count >=1)
            {
               char *methodName;
               char *declaringClassName;
               jclass declaring_class;
               int i=0;


               printf("\nStack Trace\n");
               printf("-----------\n\n");
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
     /* Get Stack Trace */
 } exit_critical_section(jvmti_env);
 }
// VM init callback
static void JNICALL callbackVMInit(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread)
{
    enter_critical_section(jvmti);
    {
        char  tname[512];
        static jvmtiEvent events[] = { JVMTI_EVENT_THREAD_START, JVMTI_EVENT_THREAD_END };
        int i;
        jvmtiFrameInfo frames[5];
        jvmtiError err, err1;
        jvmtiError error;
        jint count;
        /* The VM has started. */
        printf("Got VM init event\n");
        get_thread_name(jvmti_env , thread, tname, sizeof(tname));
        printf("callbackVMInit:  %s thread\n", tname);
        error = (jvmti)->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION, (jthread)NULL);
        check_jvmti_error(jvmti_env, error, "Cannot set event notification");
    }
    exit_critical_section(jvmti);
}

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
     //capa.can_generate_single_step_events=1;
     capa.can_generate_monitor_events=1;
     error = (jvmti)->AddCapabilities(&capa);
     check_jvmti_error(jvmti, error, "Unable to get necessary JVMTI capabilities.");
     //Memory allocation to call back event
     (void)memset(&callbacks, 0, sizeof(callbacks));
      callbacks.VMInit = &callbackVMInit; /* JVMTI_EVENT_VM_INIT */
      //callbacks.MonitorContendedEnter= &callbackMonitorContendedEnter;
      callbacks.MonitorContendedEnter= &callbackMonitorContendedEnter;
      callbacks.VMDeath = &callbackVMDeath; /* JVMTI_EVENT_VM_DEATH */
     // callbacks.Exception = &callbackException;/* JVMTI_EVENT_EXCEPTION */
     // callbacks.ThreadStart=&callbackThreadStart;
      //callbacks.VMObjectAlloc = &callbackVMObjectAlloc;/* JVMTI_EVENT_VM_OBJECT_ALLOC */
      error = (jvmti)->SetEventCallbacks(&callbacks, (jint)sizeof(callbacks));
      check_jvmti_error(jvmti, error, "ERROR in setting jvmti callbacks");
      error = (jvmti)->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, (jthread)NULL);
      error = (jvmti)->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_DEATH, (jthread)NULL);
      //error = (jvmti)->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_OBJECT_ALLOC, (jthread)NULL);
    error = (jvmti)->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_MONITOR_CONTENDED_ENTER, (jthread)NULL);
    //  error = (jvmti)->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_MONITOR_CONTENDED_ENTERED, (jthread)NULL);
     // error = (jvmti)->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_THREAD_START, (jthread)NULL);
      check_jvmti_error(jvmti, error, "Cannot set event notification");
      error = (jvmti)->CreateRawMonitor("Agent Data", &(gdata->lock));
      check_jvmti_error(jvmti, error, "Cannot create raw monitor");
    return JNI_OK;
}
static void callbackThreadStart(jvmtiEnv *jvmti_env,JNIEnv* jni_env, jthread thread)
{
    enter_critical_section(jvmti);
    {
        jvmtiError err;
        jint thr_count;
        jthread *thr_ptr;
        err = (jvmti)->GetAllThreads(&thr_count, &thr_ptr);
        if (err != JVMTI_ERROR_NONE)
        {
             printf("(GetThreadInfo) Error expected: %d, got: %d\n", JVMTI_ERROR_NONE, err);
             printf("\n");
         }
        printf("The Thread Count is %d ",thr_count );
    }
    printf("Thread Started");
}
/* Agent_OnUnload() is called last */
JNIEXPORT void JNICALL Agent_OnUnload(JavaVM *vm)
{
}
