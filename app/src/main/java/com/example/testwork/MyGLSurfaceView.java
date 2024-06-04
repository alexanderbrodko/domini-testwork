package com.example.testwork;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class MyGLSurfaceView extends GLSurfaceView {

    private final MyGLRenderer renderer;

    private float previousX;
    private float orbit = 0.0f;
    private float orbitSpeed = 0.0f;
    private final float dampingFactor = 0.99f; // Коэффициент затухания скорости (демпфирования)

    public MyGLSurfaceView(Context context) {
        super(context);

        // Создаем OpenGL ES 2.0 контекст
        setEGLContextClientVersion(2);

        renderer = new MyGLRenderer();

        // Назначаем рендерер для GLSurfaceView
        setRenderer(renderer);
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

    public boolean onTouchEvent(MotionEvent event) {
        float x = event.getX();

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                previousX = x;
                orbitSpeed = 0.0f; // Сброс скорости при нажатии
                break;
            case MotionEvent.ACTION_MOVE:
                float delta = x - previousX;
                moveOrbit(delta);
                previousX = x;
                break;
            case MotionEvent.ACTION_UP:
                // Ничего не делаем при отпускании, скорость будет затухать постепенно
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

        orbitSpeed *= dampingFactor;
    }
}
