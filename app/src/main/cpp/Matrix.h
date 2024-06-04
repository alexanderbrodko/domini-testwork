#pragma once

class Matrix {
public:
    Matrix();

    [[maybe_unused]] Matrix(const float *array);
    void multiply(const Matrix& rhs);
    void rotate(float a, float x, float y, float z);
    void setIdentity();
    void translate(float x, float y, float z);
    const float* getData() const;
    float& operator[](int index);
    void perspective(float fovY, float aspect, float near, float far);

private:
    float data[16];
};
