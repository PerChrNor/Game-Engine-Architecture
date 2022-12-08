#include "vector3d.h"
#include "math_constants.h"
#include "vector2d.h"
#include <cmath>

// Constructor for vector3d, takes three floats
Vector3D::Vector3D(float a, float b, float c) : x{a}, y{b}, z{c}
{

}

// Assignment
// Overloads operator = to work with vector3d
const Vector3D& Vector3D::operator = (const Vector3D& v)
{
    x = v.GetX();
    y = v.GetY();
    z = v.GetZ();
    return *this;
}

// Overloads operator + to do correct vector addition
Vector3D Vector3D::operator + (const Vector3D& v) const
{
    Vector3D sum(x + v.GetX(), y + v.GetY(), z + v.GetZ());
    return sum;
}

// Overloads operator - to do correct vector subtraction
Vector3D Vector3D::operator-(const Vector3D &v) const
{
    return{x - v.GetX(), y - v.GetY(), z - v.GetZ()};

}

// Overloads operator * to do correct vector dot product
float Vector3D::operator * (const Vector3D& v) const
{
    float prod = x * v.GetX() + y * v.GetY() + z * v.GetZ();
    return prod;
}

// Overloads operator ^ to do correct vecor cross product
Vector3D Vector3D::operator ^ (const Vector3D& v) const
{
    Vector3D prod((y * v.z - z * v.y), -(z * v.x - x * v.z), (x * v.y - y * v.x));
    return prod;
}

Vector3D& Vector3D::operator+=(const Vector3D& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
}



Vector3D& Vector3D::operator-=(const Vector3D& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
}

Vector3D Vector3D::operator-() const
{
    return {-x, -y, -z};
}

// length |v| = square root of x^2 + y^2 + z^2
float Vector3D::length() const
{
    float l = std::sqrt(x * x + y * y + z * z);
    return l;
}

// normalization = x / |v|, y / |v|, z / |v|
void Vector3D::normalize()
{
    if (length() > 0.f)
    {
    x = x / length();
    y = y / length();
    z = z / length();
    }
}

Vector3D Vector3D::cross(const Vector3D &v1, const Vector3D &v2)
{
    return {((v1.GetY() * v2.GetZ()) - (v1.GetZ() * v2.GetY())), ((v1.GetZ() * v2.GetX()) - (v1.GetX() * v2.GetZ())), ((v1.GetX() * v2.GetY()) - (v1.GetY() * v2.GetX()))};
}

float Vector3D::dot(const Vector3D &v1, const Vector3D &v2)
{
    return ((v1.GetX() * v2.GetX()) + (v1.GetY() * v2.GetY()) + (v1.GetZ() * v2.GetZ()));
}

// scaling with dot product
Vector3D Vector3D::operator * (float c) const
{
    Vector3D prod(x * c, y * c, z * c);
    return prod;
}

Vector3D Vector3D::mult(const Vector3D &v)
{
    Vector3D multiply = Vector3D(x * v.x, y * v.y, z * v.z);
    return multiply;
}

// output stream
std::ostream& operator<< (std::ostream &os, const Vector3D &v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

// input stream
std::istream& operator>> (std::istream &in, Vector3D &v)
{
    in >> v.x;
    in >> v.y;
    in >> v.z;
    return in;
}
/// Overlasting av << og >> operatorene gjør at vektoren blir vist som (1.0, 2.0, 3.0) når den blir lagret i en tekst fil.


// Funksjoner som blir brukt av Vector4d
float Vector3D::GetX() const
{
    return x;
}

float Vector3D::GetY() const
{
    return y;
}

float Vector3D::GetZ() const
{
    return z;
}

void Vector3D::rotateX(GLfloat angle)
{
    Vector3D dir;
    angle = deg2rad(angle);

    dir.x = x;
    dir.y = (y * std::cos(angle)) + (z * (-std::sin(angle)));
    dir.z = (y * std::sin(angle)) + (z * std::cos(angle));

    x = dir.GetX();
    y = dir.GetY();
    z = dir.GetZ();
}

void Vector3D::rotateY(GLfloat angle)
{
    Vector3D dir;
    angle = deg2rad(angle);

    dir.x = (y * std::cos(angle)) + (z * std::sin(angle));
    dir.y = y;
    dir.z = (y * (-std::sin(angle))) + (z * std::cos(angle));

    x = dir.GetX();
    y = dir.GetY();
    z = dir.GetZ();
}

void Vector3D::rotateZ(GLfloat angle)
{
    Vector3D dir;
    angle = deg2rad(angle);

    dir.x = (y * std::cos(angle)) + (z * (-std::sin(angle)));
    dir.z = (y * std::sin(angle)) + (z * std::cos(angle));
    dir.y = z;

    x = dir.GetX();
    y = dir.GetY();
    z = dir.GetZ();
}


float Vector3D::barycenticHeight(const Vector3D &point, const Vector3D &corner1, const Vector3D &corner2, const Vector3D &corner3)
{



    // This calculation assumes that height is in the Y axis.
    // We also assume that the point is projected down the Y axis onto the triangle
    // Because of this we flatten the problem down to 2D
    Vector2D p{point.x, point.z};
    Vector2D a{corner1.x, corner1.z};
    Vector2D b{corner2.x, corner2.z};
    Vector2D c{corner3.x, corner3.z};

    Vector2D v0 = b - a;
    Vector2D v1 = c - a;
    Vector2D v2 = p - a;
    float d00 = Vector2D::dot(v0,v0);
    float d01 = Vector2D::dot(v0, v1);
    float d11 = Vector2D::dot(v1, v1);
    float d20 = Vector2D::dot(v2, v0);
    float d21 = Vector2D::dot(v2, v1);
    float invDenom = 1.0f / (d00 * d11 - d01 * d01);

    // u, v, w are the barycentric coordinates
    float v = (d11 * d20 - d01 * d21) * invDenom;
    float w = (d00 * d21 - d01 * d20) * invDenom;
    float u = 1.0f - v - w;

//        qDebug() << "BaryCords:" << u << v << w;

    float heightOut;
    if(u >= 0.f && v >= 0.f && w >= 0.f )
        heightOut = corner1.y * u + corner2.y * v + corner3.y * w;
    else
        heightOut = -10000.f;

    return heightOut;
}

Vector3D Vector3D::rotatePointAroundPivot(Vector3D Point, Vector3D Pivot, Vector3D Angles){

    Vector3D temp = Point - Pivot;
    Vector3D returnVal;
    temp.rotateX(Angles.x);
    temp.rotateY(Angles.y);
    temp.rotateZ(Angles.z);
    returnVal = temp + Pivot;

    return returnVal;
}












