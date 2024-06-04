#include <jni.h>
#include <string>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#include <cmath>
#include "Matrix.h"
#include "Batch.h"

const char* vertexShaderSource = R"(
uniform mat4 uMVPMatrix;
attribute vec3 aPosition;
attribute vec3 aColor;
varying vec3 vColor;
void main() {
    gl_Position = uMVPMatrix * vec4(aPosition, 1.0);
    vColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
precision mediump float;
varying vec3 vColor;
void main() {
    gl_FragColor = vec4(vColor, 1.0);
}
)";

const GLfloat vertices[] = {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f
};

const GLushort indices[] = {
        0, 1, 2, 0, 2, 3, // Back face
        4, 5, 6, 4, 6, 7, // Front face
        4, 5, 1, 4, 1, 0, // Left face
        7, 6, 2, 7, 2, 3, // Right face
        4, 7, 3, 4, 3, 0, // Top face
        5, 6, 2, 5, 2, 1  // Bottom face
};

const GLubyte colors[] = {
        255, 0, 0, 255,
        0, 255, 0, 255,
        0, 0, 255, 255,
        255, 255, 0, 255,
        0, 255, 255, 255,
        255, 0, 255, 255,
        127, 127, 127, 255,
        255, 255, 255, 255
};


Batch batch;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_testwork_MyGLRenderer_nativeOnSurfaceCreated(JNIEnv* env, jobject obj) {
    batch.initProgram(vertexShaderSource, fragmentShaderSource);
    batch.setAttribData("aPosition", vertices, 3);
    batch.setAttribData("aColor", colors, 3, true);
    batch.setIndexData(indices);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_testwork_MyGLRenderer_nativeOnSurfaceChanged(JNIEnv* env, jobject obj, jint width, jint height) {
    batch.setViewport(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_testwork_MyGLRenderer_nativeOnDrawFrame(JNIEnv* env, jobject obj, jfloat orbit) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix modelViewMatrix;
    modelViewMatrix.translate(0.0f, 0.0f, -5.0f);
    modelViewMatrix.rotate(orbit, 0.0f, 1.0f, 0.0f);
    modelViewMatrix.translate(0.0f, 0.0f, -5.0f);

    batch.draw(modelViewMatrix);
}

