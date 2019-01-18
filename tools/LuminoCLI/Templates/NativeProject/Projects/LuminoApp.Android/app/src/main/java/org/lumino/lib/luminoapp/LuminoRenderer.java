package org.lumino.lib.luminoapp;

import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class LuminoRenderer implements GLSurfaceView.Renderer
{
    boolean _firstSurfaceChanged;

    LuminoRenderer() {
        _firstSurfaceChanged = true;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.d("onSurfaceCreated", "call");
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        if (_firstSurfaceChanged) {
            nativeInitialize(width, height);
            _firstSurfaceChanged = false;
        }
        else {
            nativeOnSurfaceChanged(width, height);
        }
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        /*
         onDrawFrame is called by 60 FPS.
        */

        nativeUpdateFrame();
    }

    static {
        System.loadLibrary("native-lib");
    }

    public static native void nativeInitialize(final int width, final int height);
    public static native void nativeUpdateFrame();
    public static native void nativeFinalize();
    public static native void nativeAddAssetArchive(String filePath);
    public static native void nativeOnSurfaceChanged(final int width, final int height);
    public static native void nativeOnPause();
    public static native void nativeOnResume();
    public static native void nativeTouchesBegin(final int id, final float x, final float y);
    public static native void nativeTouchesEnd(final int id, final float x, final float y);
    public static native void nativeTouchesMove(final int[] ids, final float[] xs, final float[] ys);
    public static native void nativeTouchesCancel(final int[] ids, final float[] xs, final float[] ys);
    public static native boolean nativeKeyEvent(final int keyCode,boolean isPressed);
}
