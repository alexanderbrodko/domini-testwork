package com.example.testwork;

import android.annotation.SuppressLint;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class MyGLSurfaceView extends GLSurfaceView {

    private final MyGLRenderer renderer;

    private float previousX;
    private float orbit = 0.0f;
    private float orbitSpeed = 0.0f;

    public MyGLSurfaceView(Context context) {
        super(context);

        setEGLContextClientVersion(2);

        renderer = new MyGLRenderer();

        setRenderer(renderer);
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

    @SuppressLint("ClickableViewAccessibility")
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        float x = event.getX();
        float delta = x - previousX;

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                previousX = x;
                orbitSpeed = 0.0f;
                break;
            case MotionEvent.ACTION_MOVE:
                moveOrbit(delta);
                previousX = x;
                break;
            case MotionEvent.ACTION_UP:
                moveOrbit(delta);
                break;
        }

        return true;
    }

    public void moveOrbit(float delta) {
        orbitSpeed += delta * 0.0001f;
    }

    public void update() {
        orbit += orbitSpeed;
        renderer.setOrbit(orbit);

        orbitSpeed *= 0.99f;
    }
}
