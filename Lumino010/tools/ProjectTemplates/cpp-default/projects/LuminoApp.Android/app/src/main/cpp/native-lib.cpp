#include <jni.h>
#include <string>
#include <LuminoEngine/Platform/AndroidPlatformInterface.hpp>


extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeInitialize(
    JNIEnv *env,
    jobject /* this */,
    jint width,
    jint height)
{
    ln::AndroidPlatformInterface::init(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeUpdateFrame(
    JNIEnv *env,
    jobject /* this */)
{
    ln::AndroidPlatformInterface::updateFrame();
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeFinalize(
    JNIEnv *env,
    jobject /* this */)
{
    ln::AndroidPlatformInterface::finalize();
}

extern "C" JNIEXPORT void JNICALL
Java_org_lumino_lib_luminoapp_LuminoRenderer_nativeAddAssetArchive(
    JNIEnv *env,
    jobject /* this */,
    jstring filePath)
{
    const char* str = env->GetStringUTFChars(filePath, 0);
    ln::EngineSettings::addAssetArchive(ln::String::fromCString(str), ln::String::Empty);
    env->ReleaseStringUTFChars(filePath, str);
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
