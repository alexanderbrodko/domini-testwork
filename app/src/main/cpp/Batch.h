#pragma once

#include <GLES2/gl2.h>
#include <android/log.h>
#include <string>
#include <map>
#include "Matrix.h"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Batch", __VA_ARGS__)

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

    template<typename glType,
            typename = typename std::enable_if<std::is_array<glType>::value>>
    void setAttribData(const std::string &name, const glType &data, int cpv, GLuint type, bool needNormalize = false) {
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

    template<typename glType,
            typename = typename std::enable_if<std::is_array<glType>::value &&
                                               std::is_integral<typename std::remove_reference<decltype(std::declval<glType>()[0])>::type>::value>::type>
    void setIndexData(const glType &data) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
        checkGlError("glBufferData index");

        _indicesCount = sizeof(data) / sizeof(data[0]);
    }

    template<typename glType,
            typename = typename std::enable_if<std::is_array<glType>::value &&
                                               std::is_floating_point<typename std::remove_reference<decltype(std::declval<glType>()[0])>::type>::value>::type>
    void setUniform(const std::string &name, const glType &value) {
        glUseProgram(_program);

        GLint location = glGetUniformLocation(_program, name.c_str());
        checkGlError("glGetUniformLocation " + name);

        size_t count = sizeof(glType) / sizeof(value[0]);

        if (count > 1 && count < 5) {
            using setUniformFunc = void (*) (GLint location, GLsizei count, const GLfloat *value);
            setUniformFunc functions[] = {
                    glUniform2fv,
                    glUniform3fv,
                    glUniform4fv
            };
            auto glUniformFunc = functions[count - 2];
            glUniformFunc(location, 1, value);
        } else {
            LOGE("uniform array size %d not supported\n", (int)count);
        }
        checkGlError("set uniform " + name);
    }

    void setUniform(const std::string &name, float value);

    void setTexture(const std::string &name, const uint8_t* imageData, int width, int height, int channels, GLuint filter);

    void draw(const Matrix& modelViewMatrix);

private:
    GLuint _program;
    GLuint _projectionMatrixHandle;
    GLuint _modelViewMatrixHandle;
    Matrix _projectionMatrix;

    std::map<std::string, Attribute> _attributes;

    GLuint _textureId;
    GLint _textureLocation;

    GLuint _index;
    GLsizei _indicesCount;

    GLuint loadShader(GLenum shaderType, const char* pSource);
    GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
    void checkGlError(const std::string &op);
};
