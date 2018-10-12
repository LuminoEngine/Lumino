#include <jni.h>
#include <string>

static ln::Application* g_app = nullptr;

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeInitialize(
    JNIEnv *env,
    jobject /* this */,
    jint width,
    jint height)
{
    ln::GlobalLogger::addLogcatAdapter();
	g_app = ::LuminoCreateApplicationInstance();
	ln::detail::ApplicationHelper::initialize(g_app);
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeUpdateFrame(
    JNIEnv *env,
    jobject /* this */)
{
	ln::detail::ApplicationHelper::processTick(g_app);
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeFinalize(
    JNIEnv *env,
    jobject /* this */)
{
	ln::detail::ApplicationHelper::finalize(g_app);
	ln::RefObjectHelper::release(g_app);
    g_app = nullptr;
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeOnSurfaceChanged(
    JNIEnv *env,
    jobject /* this */,
    jint width,
    jint height)
{
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeOnPause(
    JNIEnv *env,
    jobject /* this */)
{
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeOnResume(
    JNIEnv *env,
    jobject /* this */)
{
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeTouchesBegin(
    JNIEnv *env,
    jobject /* this */,
    jint id,
    jfloat x,
    jfloat y)
{
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeTouchesEnd(
    JNIEnv *env,
    jobject /* this */,
    jint id,
    jfloat x,
    jfloat y)
{
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeTouchesMove(
    JNIEnv *env,
    jobject /* this */,
    jintArray ids,
    jfloatArray xs,
    jfloatArray ys)
{
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeTouchesCancel(
    JNIEnv *env,
    jobject /* this */,
    jintArray ids,
    jfloatArray xs,
    jfloatArray ys)
{
}

extern "C" JNIEXPORT jboolean JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeKeyEvent(
    JNIEnv *env,
    jobject /* this */,
    jint keyCode,
    jboolean isPressed)
{
    return JNI_FALSE;
}
