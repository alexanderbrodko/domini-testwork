package com.example.testwork;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MyGLRenderer implements GLSurfaceView.Renderer {

    private float orbit;

    static {
        System.loadLibrary("native-lib");
    }

    public void setOrbit(float val) {
        orbit = val;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeOnSurfaceCreated();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        nativeOnSurfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        nativeOnDrawFrame(orbit);
    }

    private native void nativeOnSurfaceCreated();
    private native void nativeOnSurfaceChanged(int width, int height);
    private native void nativeOnDrawFrame(float orbit);
}
