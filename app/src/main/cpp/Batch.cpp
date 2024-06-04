#include "Batch.h"
#include <cstdlib>

Batch::Batch() : _program(0) {}

void Batch::initProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    _program = createProgram(vertexShaderSource, fragmentShaderSource);
    if (!_program) {
        LOGE("Could not create program.");
        return;
    }
    
    _matrixHandle = glGetUniformLocation(_program, "uMVPMatrix");

    GLuint bufs[1];
    glGenBuffers(1, bufs);
    _index = bufs[0];
}

Batch::~Batch() {
    if (_program) {
        glDeleteProgram(_program);
        _program = 0;
    }
}

void Batch::setViewport(int width, int height) {
    glViewport(0, 0, width, height);
    checkGlError("glViewport");

    float ratio = (float) width / height;
    _projectionMatrix.perspective(0.7f, ratio, 0.1f, 100.0f);
}

void Batch::draw(const Matrix& modelViewMatrix) {
    glEnable(GL_DEPTH_TEST);
    
    glUseProgram(_program);
    checkGlError("glUseProgram");

    Matrix mvpMatrix = _projectionMatrix;
    mvpMatrix.multiply(modelViewMatrix);
    glUniformMatrix4fv(_matrixHandle, 1, GL_FALSE, mvpMatrix.getData());
    checkGlError("glUniformMatrix4fv");

    for (auto [key, attr] : _attributes) {
        glBindBuffer(GL_ARRAY_BUFFER, attr.buffer);

        glVertexAttribPointer(
                attr.location,
                attr.componentsPerVertex,
                attr.type,
                attr.needNormalize,
                0,
                0
        );
        checkGlError("glVertexAttribPointer " + key);
        glEnableVertexAttribArray(attr.location);
    }


    glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_SHORT, nullptr);
    checkGlError("glDrawElements");

    checkGlError("glDrawElements");
}

GLuint Batch::loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint Batch::createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, pixelShader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

void Batch::checkGlError(const std::string &op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGE("after %s() glError (0x%x)\n", op.c_str(), error);
    }
}
