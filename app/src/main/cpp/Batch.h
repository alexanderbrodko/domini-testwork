#pragma once

#include <GLES2/gl2.h>
#include <android/log.h>
#include <string>
#include <map>
#include "Matrix.h"

#define LOG_TAG "Batch"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

class Batch {
    typedef struct {
        GLint location;
        GLuint buffer;
        int componentsPerVertex;
        GLuint type;
        bool needNormalize;
        size_t size;
    } Attribute;

public:
    Batch();
    ~Batch();

    void initProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    void setViewport(int width, int height);

    template<typename glType>
    void setAttribData(const std::string &name, const glType &data, int cpv, GLuint type, bool needNormalize = false) {
        static_assert(std::is_array<glType>::value);

        size_t count = sizeof(data) / sizeof(data[0]);

        if (_attributes.count(name) == 0) {
            GLint location = glGetAttribLocation(_program, name.c_str());
            if (location == -1) {
                LOGE("can not find attribute %s\n", name.c_str());
                return;
            }

            GLuint bufs[1];
            glGenBuffers(1, bufs);

            _attributes[name] = { location, bufs[0], cpv, type, needNormalize, count };
        }

        auto &attrib = _attributes[name];

        glBindBuffer(GL_ARRAY_BUFFER, attrib.buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
        checkGlError("glBufferData " + name);
    }

    template<typename glType>
    void setIndexData(const glType &data) {
        static_assert(std::is_array<glType>::value);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
        checkGlError("glBufferData index");

        _indicesCount = sizeof(data) / sizeof(data[0]);
    }

    void draw(const Matrix& modelViewMatrix);

private:
    GLuint _program;
    GLuint _matrixHandle;
    Matrix _projectionMatrix;

    std::map<std::string, Attribute> _attributes;

    GLuint _index;
    GLsizei _indicesCount;

    GLuint loadShader(GLenum shaderType, const char* pSource);
    GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
    void checkGlError(const std::string &op);
};
