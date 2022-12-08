#include "vector4d.h"

Vector4d::Vector4d(Vector3D v, float w) : m_x{v.GetX()}, m_y{v.GetY()}, m_z{v.GetZ()}, m_w{w} // Bruker funksjonaliteten fra Vector3d til å lage en Vector4d
{

}

Vector4d::Vector4d(GLfloat x_in, GLfloat y_in, GLfloat z_in, GLfloat w_in) : x{x_in}, y{y_in}, z{z_in}, w{w_in}
{

}

Vector4d::Vector4d(const int v) : x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}, w{static_cast<float>(v)}
{

}

Vector4d::Vector4d(const double v) : x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}, w{static_cast<float>(v)}
{

}
Vector3D Vector4d::getVector()
{
    return mVector;
}

float Vector4d::GetX() const
{
    return m_x;
}

float Vector4d::GetY() const
{
    return m_y;
}

float Vector4d::GetZ() const
{
    return m_z;
}

// Bruker en ekstra retning i tillegg til Vector3d for å gjøre det til en V4ctor4d
float Vector4d::GetW() const
{
    return m_w;
}

void Vector4d::SetX(float x)
{
    m_x = x;
}

void Vector4d::SetY(float y)
{
    m_y = y;
}

void Vector4d::SetZ(float z)
{
    m_z = z;
}

void Vector4d::SetW(float w)
{
    m_w = w;
}

GLfloat Vector4d::length() const
{
    return std::sqrt(std::pow(x, 2) + std::pow (y, 2) + std::pow(z, 2) + std::pow(w, 2));
}

GLfloat Vector4d::lengthNoSqrt() const
{
    return std::pow(x, 2) + std::pow (y, 2) + std::pow(z, 2) + std::pow(w, 2);
}

/// A quick and easy normalize function.
///
/// Written by candidate: 8004
void Vector4d::normalize()
{
    GLfloat l = length();
    if (l > 0.f)
    {
        x = x/l;
        y = y/l;
        z = z/l;
        w = w/l;
    }
}
