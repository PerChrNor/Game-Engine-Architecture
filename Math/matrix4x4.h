#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <iostream>
#include <cmath>
#include <QDebug>
#include "vector4d.h"
#include "vector3d.h"


class Matrix4x4
{
public:
    Matrix4x4(std::initializer_list<float> values);
    Matrix4x4(bool isIdentity = false);

    void setToIdentity ();
    Matrix4x4 identity();

    bool inverse();

    void translateX(float x = 0.f);
    void translateY(float x = 0.f);
    void translateZ(float x = 0.f);

    void rotateX (float degrees = 0.f);
    void rotateY (float degrees = 0.f);
    void rotateZ (float degrees = 0.f);

    void scale(Vector3D s);
    void scale(float uniformScale);
    void scale(float x, float y, float z);

    Matrix4x4 operator * (float c);
    Vector4d operator * (const Vector4d v) const;
    Matrix4x4 operator * (const Matrix4x4);

    void translate(float x, float y, float z);
    void translate(Vector3D positionIn);
    void setPosition(Vector3D vec3In);

    void lookAt(const Vector3D &eye, const Vector3D &at, const Vector3D &up);

    void frustum(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    void perspective(float fovy, float aspectRatio, float n, float f);
    void ortho (float l, float r, float b, float t, float n, float f);

    static Matrix4x4 projectFrustum(float flLeft, float flRight, float flBottom, float flTop, float flNear, float flFar);

    void setRotationToVector(const Vector3D &direction, Vector3D up = Vector3D(0.f, 1.f, 0.f));

    // Operatorovelasting i Matrix4x4 klassen
    float& operator () (int i) {return matrix[i]; }

    float scale_X{1};
    float scale_Y{1};
    float scale_Z{1};

    float rotate_X{0};
    float rotate_Y{0};
    float rotate_Z{0};

    float translate_X{0};
    float translate_Y{0};
    float translate_Z{0};

    float matrix[16];

    inline const float *constData() const {return matrix;}


    // Overlaster << operatoren slik at man kan printe ut matrisen med qDebug.
    friend QDebug operator<<(QDebug output, const Matrix4x4 &m)
    {
        output <<
                  "{" << m.matrix[0] << "\t, " << m.matrix[1] << "\t, " << m.matrix[2] << "\t, " << m.matrix[3] << "}\n" <<
                  "{" << m.matrix[4] << "\t, " << m.matrix[5] << "\t, " << m.matrix[6] << "\t, " << m.matrix[7] << "}\n" <<
                  "{" << m.matrix[8] << "\t, " << m.matrix[9] << "\t, " << m.matrix[10] << "\t, " << m.matrix[11] << "}\n" <<
                  "{" << m.matrix[12] << "\t, " << m.matrix[13] << "\t, " << m.matrix[14] << "\t, " << m.matrix[15] << "}\n";
        return output;
    }

    GLfloat getFloat(int space);
    Vector4d getRowVec(int rowV);
    Vector4d getColVec(int colV);

private:
    GLfloat mFloat[16]{};
};
#endif // MATRIX4X4_H
