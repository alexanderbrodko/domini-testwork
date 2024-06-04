package com.example.testwork;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;

import android.view.WindowManager;

public class MainActivity extends Activity {

    private MyGLSurfaceView glSurfaceView;

    private final Handler handler = new Handler();
    private final Runnable updateRunnable = new Runnable() {
        @Override
        public void run() {
            glSurfaceView.update();
            handler.postDelayed(this, 16);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        glSurfaceView = new MyGLSurfaceView(this);
        setContentView(glSurfaceView);

        handler.postDelayed(updateRunnable, 16);
    }

    static {
        System.loadLibrary("native-lib");
    }
}
