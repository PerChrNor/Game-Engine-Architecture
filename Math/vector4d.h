#ifndef VECTOR4D_H
#define VECTOR4D_H

#include "vector3d.h"

class Vector4d
{
public:
    Vector4d(){ };
    Vector4d(Vector3D v, GLfloat w = 0);
    Vector4d(GLfloat x_in, GLfloat y_in = 0.f, GLfloat z_in = 0.f, GLfloat w_in = 0.f);
    Vector4d(const int v);
    Vector4d(const double v);
    Vector3D getVector();

    float GetX() const;
    float GetY() const;
    float GetZ() const;
    float GetW() const;

    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);
    void SetW(float w);

    GLfloat length() const;
    GLfloat lengthNoSqrt() const;
    void normalize();

    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;

private:
    Vector3D mVector;
    float m_x, m_y, m_z, m_w;
};

#endif // VECTOR4D_H
