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

        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
        gl.glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

        nativeUpdateFrame();
    }

    static {
        System.loadLibrary("native-lib");
    }

    private static native void nativeInitialize(final int width, final int height);
    private static native void nativeUpdateFrame();
    private static native void nativeFinalize();
    private static native void nativeOnSurfaceChanged(final int width, final int height);
    private static native void nativeOnPause();
    private static native void nativeOnResume();
    private static native void nativeTouchesBegin(final int id, final float x, final float y);
    private static native void nativeTouchesEnd(final int id, final float x, final float y);
    private static native void nativeTouchesMove(final int[] ids, final float[] xs, final float[] ys);
    private static native void nativeTouchesCancel(final int[] ids, final float[] xs, final float[] ys);
    private static native boolean nativeKeyEvent(final int keyCode,boolean isPressed);
}
