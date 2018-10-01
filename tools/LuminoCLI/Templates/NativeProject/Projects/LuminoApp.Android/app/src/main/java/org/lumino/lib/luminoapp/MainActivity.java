package org.lumino.lib.luminoapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.util.Log;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.

    LuminoGLSurfaceView _luminoGLView;
    LuminoRenderer _renderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        _luminoGLView = new LuminoGLSurfaceView(this);

        final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();

        // 端末がOpenGL ES 2.0をサポートしているかチェック
        if (configurationInfo.reqGlEsVersion >= 0x30000) {
            _luminoGLView.setEGLContextClientVersion(3);  // OpenGLバージョンを設定


            _renderer = new LuminoRenderer();
            _luminoGLView.setRenderer(_renderer);
        } else {
            Log.d("Lumino", "OpenGL ES 3.0 not supported.");
            return;
        }

        setContentView(_luminoGLView);
    }

    @Override
    protected void onResume() {
        super.onResume();
        _luminoGLView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        _luminoGLView.onPause();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    //public native String stringFromJNI();
}
