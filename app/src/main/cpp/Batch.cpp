#include "Batch.h"
#include <cstdlib>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Batch", __VA_ARGS__)

Batch::Batch() : _program(0) {}

void Batch::initProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    _program = createProgram(vertexShaderSource, fragmentShaderSource);
    if (!_program) {
        LOGE("Could not create program.");
        return;
    }

    _projectionMatrixHandle = glGetUniformLocation(_program, "uProjectionMatrix");
    _modelViewMatrixHandle = glGetUniformLocation(_program, "uModelViewMatrix");

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

    glBindTexture(GL_TEXTURE_2D, _textureId);
    glUniform1i(_textureLocation, 0);

    glUniformMatrix4fv(_projectionMatrixHandle, 1, GL_FALSE, _projectionMatrix.getData());
    checkGlError("glUniformMatrix4fv _projectionMatrixHandle");

    glUniformMatrix4fv(_modelViewMatrixHandle, 1, GL_FALSE, modelViewMatrix.getData());
    checkGlError("glUniformMatrix4fv modelViewMatrix");

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

void Batch::setTexture(const std::string &name, const uint8_t* imageData, int width, int height, int channels, GLuint filter) {
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    GLenum format;
    if (channels == 3) {
        format = GL_RGB;
    } else if (channels == 4) {
        format = GL_RGBA;
    } else {
        LOGE("Unsupported number of channels %d\n", channels);
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    _textureLocation = glGetUniformLocation(_program, name.c_str());
}

void Batch::setUniform(const std::string &name, float value)  {
    GLint location = glGetUniformLocation(_program, name.c_str());

    glUniform1f(location, value);
    checkGlError("set uniform " + name);
}

