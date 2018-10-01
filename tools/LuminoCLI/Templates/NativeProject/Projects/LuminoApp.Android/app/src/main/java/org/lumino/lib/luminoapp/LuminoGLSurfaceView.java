package org.lumino.lib.luminoapp;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class LuminoGLSurfaceView extends GLSurfaceView
{

    public LuminoGLSurfaceView(Context context)
    {
        super(context);

        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
    }
}
