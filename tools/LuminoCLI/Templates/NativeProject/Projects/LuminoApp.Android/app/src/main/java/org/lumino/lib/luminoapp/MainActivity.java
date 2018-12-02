package org.lumino.lib.luminoapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.util.Log;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

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

        // 端末がOpenGL ES 3.0をサポートしているかチェック
        if (configurationInfo.reqGlEsVersion >= 0x30000) {
            _luminoGLView.setEGLContextClientVersion(3);  // OpenGLバージョンを設定

            String archiveFilePath = expandAssetFileToLocal("Assets.lca");
            LuminoRenderer.nativeAddAssetArchive(archiveFilePath);

            _renderer = new LuminoRenderer();
            _luminoGLView.setRenderer(_renderer);
        } else {
            Log.d("Lumino", "OpenGL ES 3.0 not supported.");
            return;
        }

        setContentView(_luminoGLView);




/*
        AssetManager as = getResources().getAssets();
        InputStream is = null;
        try {
            is = as.open("test.txt");

            InputStreamReader reader = new InputStreamReader(is);
            StringBuilder builder = new StringBuilder();
            char[] buffer = new char[512];
            int read;
            while (0 <= (read = reader.read(buffer))) {
                builder.append(buffer, 0, read);
            }

            String s = builder.toString();
            Log.i("onCreate", s);


        } catch (IOException e) {
            e.printStackTrace();
        }
        */
    }

    private String expandAssetFileToLocal(String fileName)
    {
        try {
            InputStream inputStream = getAssets().open(fileName);
            FileOutputStream fileOutputStream = openFileOutput(fileName, MODE_PRIVATE);
            byte[] buffer = new byte[1024];
            int length = 0;
            while ((length = inputStream.read(buffer)) >= 0) {
                fileOutputStream.write(buffer, 0, length);
            }
            fileOutputStream.close();
            inputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        File f = getFileStreamPath(fileName);
        Log.d("Lumino", "Asset archive loaded. (" + f.getPath() + ")");

        return f.getPath();
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
}
