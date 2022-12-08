#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <fstream>
#include <cmath>
#include "gltypes.h"
#include <qvector.h>

class Vector3D
{
public:
    Vector3D(float a=0.0f, float b=0.0f, float c=0.0);  // Constructor

    const Vector3D& operator = (const Vector3D& v);     // Assignment

    Vector3D operator + (const Vector3D& v) const;      // Addition

    Vector3D operator-(const Vector3D& v) const;      // Subtraction

    float operator * (const Vector3D& v) const;         // Dot product

    Vector3D operator ^ (const Vector3D& v) const;      // Cross product

    Vector3D& operator+=(const Vector3D &rhs);
    Vector3D& operator-=(const Vector3D &rhs);

    Vector3D operator-() const;

    float length() const;                               // return length
    void normalize();                                   // Normalize to unit length
    static Vector3D cross(const Vector3D &v1, const Vector3D &v2);
    static float dot(const Vector3D &v1, const Vector3D &v2);

    Vector3D operator * (float c) const;                // Scaling

    Vector3D mult (const Vector3D& v);

    friend std::istream& operator>> (std::istream &in, Vector3D &v); // input

    friend std::ostream& operator<< (std::ostream &os, const Vector3D &v); // output

    float x, y, z;

    static float barycenticHeight(const Vector3D &point, const Vector3D &corner, const Vector3D &corner2, const Vector3D &corner3);

    static Vector3D rotatePointAroundPivot(Vector3D Point, Vector3D Pivot, Vector3D Angles);



//  Funksjonene som blir brukt for å få Vector4d til å fungere.
    float GetX() const;
    float GetY() const;
    float GetZ() const;

    void rotateX(GLfloat angle);
    void rotateY(GLfloat angle);
    void rotateZ(GLfloat angle);
};

#endif // VECTOR3D_H
