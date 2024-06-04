#include "Matrix.h"
#include <cmath>
#include <stdexcept>

Matrix::Matrix() {
    setIdentity();
}

[[maybe_unused]] Matrix::Matrix(const float *array) {
    data[0] = array[0];
    data[1] = array[1];
    data[2] = array[2];
    data[3] = array[3];
    data[4] = array[4];
    data[5] = array[5];
    data[6] = array[6];
    data[7] = array[7];
    data[8] = array[8];
    data[9] = array[9];
    data[10] = array[10];
    data[11] = array[11];
    data[12] = array[12];
    data[13] = array[13];
    data[14] = array[14];
    data[15] = array[15];
}

void Matrix::multiply(const Matrix& rhs) {
    auto b = rhs.data;
    float a00 = data[0],
            a01 = data[1],
            a02 = data[2],
            a03 = data[3];
    float a10 = data[4],
            a11 = data[5],
            a12 = data[6],
            a13 = data[7];
    float a20 = data[8],
            a21 = data[9],
            a22 = data[10],
            a23 = data[11];
    float a30 = data[12],
            a31 = data[13],
            a32 = data[14],
            a33 = data[15];
    float b0 = b[0],
            b1 = b[1],
            b2 = b[2],
            b3 = b[3];
    data[0] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    data[1] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    data[2] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    data[3] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;
    b0 = b[4];
    b1 = b[5];
    b2 = b[6];
    b3 = b[7];
    data[4] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    data[5] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    data[6] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    data[7] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;
    b0 = b[8];
    b1 = b[9];
    b2 = b[10];
    b3 = b[11];
    data[8] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    data[9] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    data[10] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    data[11] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;
    b0 = b[12];
    b1 = b[13];
    b2 = b[14];
    b3 = b[15];
    data[12] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    data[13] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    data[14] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    data[15] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;
}

void Matrix::rotate(float a, float x, float y, float z) {
    float len = sqrtf(x * x + y * y + z * z);
    float a00, a01, a02, a03;
    float a10, a11, a12, a13;
    float a20, a21, a22, a23;
    float b00, b01, b02;
    float b10, b11, b12;
    float b20, b21, b22;
    if (len < 0.000000001) return;
    len = 1 / len;
    x *= len;
    y *= len;
    z *= len;
    float s = sin(a);
    float c = cos(a);
    float t = 1 - c;
    a00 = data[0];
    a01 = data[1];
    a02 = data[2];
    a03 = data[3];
    a10 = data[4];
    a11 = data[5];
    a12 = data[6];
    a13 = data[7];
    a20 = data[8];
    a21 = data[9];
    a22 = data[10];
    a23 = data[11];

    b00 = x * x * t + c;
    b01 = y * x * t + z * s;
    b02 = z * x * t - y * s;
    b10 = x * y * t - z * s;
    b11 = y * y * t + c;
    b12 = z * y * t + x * s;
    b20 = x * z * t + y * s;
    b21 = y * z * t - x * s;
    b22 = z * z * t + c;

    data[0] = a00 * b00 + a10 * b01 + a20 * b02;
    data[1] = a01 * b00 + a11 * b01 + a21 * b02;
    data[2] = a02 * b00 + a12 * b01 + a22 * b02;
    data[3] = a03 * b00 + a13 * b01 + a23 * b02;
    data[4] = a00 * b10 + a10 * b11 + a20 * b12;
    data[5] = a01 * b10 + a11 * b11 + a21 * b12;
    data[6] = a02 * b10 + a12 * b11 + a22 * b12;
    data[7] = a03 * b10 + a13 * b11 + a23 * b12;
    data[8] = a00 * b20 + a10 * b21 + a20 * b22;
    data[9] = a01 * b20 + a11 * b21 + a21 * b22;
    data[10] = a02 * b20 + a12 * b21 + a22 * b22;
    data[11] = a03 * b20 + a13 * b21 + a23 * b22;
}

void Matrix::setIdentity() {
    data[1] =
    data[2] =
    data[3] =
    data[4] =
    data[6] =
    data[7] =
    data[8] =
    data[9] =
    data[11] =
    data[12] =
    data[13] =
    data[14] = 0;
    data[0] =
    data[5] =
    data[10] =
    data[15] = 1;
}

void Matrix::translate(float x, float y, float z) {
    float a00, a01, a02, a03;
    float a10, a11, a12, a13;
    float a20, a21, a22, a23;
    data[12] = data[0] * x + data[4] * y + data[8] * z + data[12];
    data[13] = data[1] * x + data[5] * y + data[9] * z + data[13];
    data[14] = data[2] * x + data[6] * y + data[10] * z + data[14];
    data[15] = data[3] * x + data[7] * y + data[11] * z + data[15];
}

const float* Matrix::getData() const {
    return data;
}

float& Matrix::operator[](int index)  {
    if (index < 0 || index >= 16) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    return data[index];
}

void Matrix::perspective(float fovY, float aspect, float near, float far) {
    float f = 1.0f / tan(fovY / 2.0f),
            nf;
    data[0] = f / aspect;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;
    data[5] = f;
    data[6] = 0;
    data[7] = 0;
    data[8] = 0;
    data[9] = 0;
    data[11] = -1.0f;
    data[12] = 0;
    data[13] = 0;
    data[15] = 0;
    if (far != 0) {
        nf = 1 / (near - far);
        data[10] = (far + near) * nf;
        data[14] = 2.0f * far * near * nf;
    } else {
        data[10] = -1.0f;
        data[14] = -2.0f * near;
    }
}
