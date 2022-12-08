#include "matrix4x4.h"
#include <iostream>
#include "math_constants.h"

Matrix4x4::Matrix4x4(bool isIdentity)
{
    if(isIdentity)
    {
        identity();
    }
    else
    {
        for(int i = 0; i < 16; i++)
            matrix[i] = 0.f;
    }
}

Matrix4x4::Matrix4x4(std::initializer_list<float> values)
{
    int i = 0;
    for(auto value : values)
        matrix[i++] = value;
}

Matrix4x4 Matrix4x4::identity()
{
    setToIdentity();
    return  *this;
}
// Lager en identitetsmatrise som setter hele matrisen til en standard 4x4 matrise.
void Matrix4x4::setToIdentity()
{
    *this =
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

bool Matrix4x4::inverse()
{
    GLfloat inv[16], det;
    GLfloat invOut[16];

    int i;

    inv[0] = matrix[5]  * matrix[10] * matrix[15] -
            matrix[5]  * matrix[11] * matrix[14] -
            matrix[9]  * matrix[6]  * matrix[15] +
            matrix[9]  * matrix[7]  * matrix[14] +
            matrix[13] * matrix[6]  * matrix[11] -
            matrix[13] * matrix[7]  * matrix[10];

    inv[4] = -matrix[4]  * matrix[10] * matrix[15] +
            matrix[4]  * matrix[11] * matrix[14] +
            matrix[8]  * matrix[6]  * matrix[15] -
            matrix[8]  * matrix[7]  * matrix[14] -
            matrix[12] * matrix[6]  * matrix[11] +
            matrix[12] * matrix[7]  * matrix[10];

    inv[8] = matrix[4]  * matrix[9] * matrix[15] -
            matrix[4]  * matrix[11] * matrix[13] -
            matrix[8]  * matrix[5] * matrix[15] +
            matrix[8]  * matrix[7] * matrix[13] +
            matrix[12] * matrix[5] * matrix[11] -
            matrix[12] * matrix[7] * matrix[9];

    inv[12] = -matrix[4]  * matrix[9] * matrix[14] +
            matrix[4]  * matrix[10] * matrix[13] +
            matrix[8]  * matrix[5] * matrix[14] -
            matrix[8]  * matrix[6] * matrix[13] -
            matrix[12] * matrix[5] * matrix[10] +
            matrix[12] * matrix[6] * matrix[9];

    inv[1] = -matrix[1]  * matrix[10] * matrix[15] +
            matrix[1]  * matrix[11] * matrix[14] +
            matrix[9]  * matrix[2] * matrix[15] -
            matrix[9]  * matrix[3] * matrix[14] -
            matrix[13] * matrix[2] * matrix[11] +
            matrix[13] * matrix[3] * matrix[10];

    inv[5] = matrix[0]  * matrix[10] * matrix[15] -
            matrix[0]  * matrix[11] * matrix[14] -
            matrix[8]  * matrix[2] * matrix[15] +
            matrix[8]  * matrix[3] * matrix[14] +
            matrix[12] * matrix[2] * matrix[11] -
            matrix[12] * matrix[3] * matrix[10];

    inv[9] = -matrix[0]  * matrix[9] * matrix[15] +
            matrix[0]  * matrix[11] * matrix[13] +
            matrix[8]  * matrix[1] * matrix[15] -
            matrix[8]  * matrix[3] * matrix[13] -
            matrix[12] * matrix[1] * matrix[11] +
            matrix[12] * matrix[3] * matrix[9];

    inv[13] = matrix[0]  * matrix[9] * matrix[14] -
            matrix[0]  * matrix[10] * matrix[13] -
            matrix[8]  * matrix[1] * matrix[14] +
            matrix[8]  * matrix[2] * matrix[13] +
            matrix[12] * matrix[1] * matrix[10] -
            matrix[12] * matrix[2] * matrix[9];

    inv[2] = matrix[1]  * matrix[6] * matrix[15] -
            matrix[1]  * matrix[7] * matrix[14] -
            matrix[5]  * matrix[2] * matrix[15] +
            matrix[5]  * matrix[3] * matrix[14] +
            matrix[13] * matrix[2] * matrix[7] -
            matrix[13] * matrix[3] * matrix[6];

    inv[6] = -matrix[0]  * matrix[6] * matrix[15] +
            matrix[0]  * matrix[7] * matrix[14] +
            matrix[4]  * matrix[2] * matrix[15] -
            matrix[4]  * matrix[3] * matrix[14] -
            matrix[12] * matrix[2] * matrix[7] +
            matrix[12] * matrix[3] * matrix[6];

    inv[10] = matrix[0]  * matrix[5] * matrix[15] -
            matrix[0]  * matrix[7] * matrix[13] -
            matrix[4]  * matrix[1] * matrix[15] +
            matrix[4]  * matrix[3] * matrix[13] +
            matrix[12] * matrix[1] * matrix[7] -
            matrix[12] * matrix[3] * matrix[5];

    inv[14] = -matrix[0]  * matrix[5] * matrix[14] +
            matrix[0]  * matrix[6] * matrix[13] +
            matrix[4]  * matrix[1] * matrix[14] -
            matrix[4]  * matrix[2] * matrix[13] -
            matrix[12] * matrix[1] * matrix[6] +
            matrix[12] * matrix[2] * matrix[5];

    inv[3] = -matrix[1] * matrix[6] * matrix[11] +
            matrix[1] * matrix[7] * matrix[10] +
            matrix[5] * matrix[2] * matrix[11] -
            matrix[5] * matrix[3] * matrix[10] -
            matrix[9] * matrix[2] * matrix[7] +
            matrix[9] * matrix[3] * matrix[6];

    inv[7] = matrix[0] * matrix[6] * matrix[11] -
            matrix[0] * matrix[7] * matrix[10] -
            matrix[4] * matrix[2] * matrix[11] +
            matrix[4] * matrix[3] * matrix[10] +
            matrix[8] * matrix[2] * matrix[7] -
            matrix[8] * matrix[3] * matrix[6];

    inv[11] = -matrix[0] * matrix[5] * matrix[11] +
            matrix[0] * matrix[7] * matrix[9] +
            matrix[4] * matrix[1] * matrix[11] -
            matrix[4] * matrix[3] * matrix[9] -
            matrix[8] * matrix[1] * matrix[7] +
            matrix[8] * matrix[3] * matrix[5];

    inv[15] = matrix[0] * matrix[5] * matrix[10] -
            matrix[0] * matrix[6] * matrix[9] -
            matrix[4] * matrix[1] * matrix[10] +
            matrix[4] * matrix[2] * matrix[9] +
            matrix[8] * matrix[1] * matrix[6] -
            matrix[8] * matrix[2] * matrix[5];

    det = matrix[0] * inv[0] + matrix[1] * inv[4] + matrix[2] * inv[8] + matrix[3] * inv[12];

    if (det == 0.f)
        return false;

    det = 1.f / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    memcpy(matrix, invOut, 16*sizeof(GLfloat));

    return true;
}


Vector4d Matrix4x4::operator *(const Vector4d v) const
{
    // Overlaster operatoren * for vertex-multiplikasjon med en matrise.
    Vector4d u(Vector3D(matrix[0] * v.GetX() + matrix[1] * v.GetY() + matrix[2] * v.GetZ() + matrix[3] * v.GetW(), // x
            matrix[4] * v.GetX() + matrix[5] * v.GetY() + matrix[6] * v.GetZ() + matrix[7] * v.GetW(), // y
            matrix[8] * v.GetX() + matrix[9] * v.GetY() + matrix[10] * v.GetZ() + matrix[11] * v.GetW()),// z
            matrix[12] * v.GetX() + matrix[13] * v.GetY() + matrix[14] * v.GetZ() + matrix[15] * v.GetW());// w
    return u;
}


Matrix4x4 Matrix4x4::operator *(const Matrix4x4 m)
{
    // Overlaster operatoren * for matrise-multiplikasjoner med en annen matrise
    return
    {
        matrix[0]  * m.matrix[0] + matrix[1]  * m.matrix[4] + matrix[2]  * m.matrix[8]  + matrix[3]  * m.matrix[12],
        matrix[0]  * m.matrix[1] + matrix[1]  * m.matrix[5] + matrix[2]  * m.matrix[9]  + matrix[3]  * m.matrix[13],
        matrix[0]  * m.matrix[2] + matrix[1]  * m.matrix[6] + matrix[2]  * m.matrix[10] + matrix[3]  * m.matrix[14],
        matrix[0]  * m.matrix[3] + matrix[1]  * m.matrix[7] + matrix[2]  * m.matrix[11] + matrix[3]  * m.matrix[15],

        matrix[4]  * m.matrix[0] + matrix[5]  * m.matrix[4] + matrix[6]  * m.matrix[8]  + matrix[7]  * m.matrix[12],
        matrix[4]  * m.matrix[1] + matrix[5]  * m.matrix[5] + matrix[6]  * m.matrix[9]  + matrix[7]  * m.matrix[13],
        matrix[4]  * m.matrix[2] + matrix[5]  * m.matrix[6] + matrix[6]  * m.matrix[10] + matrix[7]  * m.matrix[14],
        matrix[4]  * m.matrix[3] + matrix[5]  * m.matrix[7] + matrix[6]  * m.matrix[11] + matrix[7]  * m.matrix[15],

        matrix[8]  * m.matrix[0] + matrix[9]  * m.matrix[4] + matrix[10] * m.matrix[8]  + matrix[11] * m.matrix[12],
        matrix[8]  * m.matrix[1] + matrix[9]  * m.matrix[5] + matrix[10] * m.matrix[9]  + matrix[11] * m.matrix[13],
        matrix[8]  * m.matrix[2] + matrix[9]  * m.matrix[6] + matrix[10] * m.matrix[10] + matrix[11] * m.matrix[14],
        matrix[8]  * m.matrix[3] + matrix[9]  * m.matrix[7] + matrix[10] * m.matrix[11] + matrix[11] * m.matrix[15],

        matrix[12] * m.matrix[0] + matrix[13] * m.matrix[4] + matrix[14] * m.matrix[8]  + matrix[15] * m.matrix[12],
        matrix[12] * m.matrix[1] + matrix[13] * m.matrix[5] + matrix[14] * m.matrix[9]  + matrix[15] * m.matrix[13],
        matrix[12] * m.matrix[2] + matrix[13] * m.matrix[6] + matrix[14] * m.matrix[10] + matrix[15] * m.matrix[14],
        matrix[12] * m.matrix[3] + matrix[13] * m.matrix[7] + matrix[14] * m.matrix[11] + matrix[15] * m.matrix[15]
    };
}

void Matrix4x4::scale(float x, float y, float z)
{
    Matrix4x4 temp =
    {
        x, 0.f, 0.f, 0.f,
        0.f, y, 0.f, 0.f,
        0.f, 0.f, z, 0.f,
        0.f, 0.f, 0.f, 1.f
    };

    *this = (*this)*temp;
}

void Matrix4x4::translateX(float x)
{
    translate(x, 0.f, 0.f);
}

void Matrix4x4::translateY(float y)
{
    translate(0.f, y, 0.f);
}


void Matrix4x4::translateZ(float z)
{
    translate(0.f, 0.f, z);
}

void Matrix4x4::translate(float x, float y, float z)
{
    // Forflytning av en 4x4 Matrise, der retningen blir bestemt i objektklassene
    Matrix4x4 mat =
    {
        1.f, 0.f, 0.f, x,
        0.f, 1.f, 0.f, y,
        0.f, 0.f, 1.f, z,
        0.f, 0.f, 0.f, 1.f
    };

    *this = (*this)*mat;
}

void Matrix4x4::translate(Vector3D positionIn)
{
    Matrix4x4 mat =
    {
        1.f, 0.f, 0.f, positionIn.GetX(),
        0.f, 1.f, 0.f, positionIn.GetY(),
        0.f, 0.f, 1.f, positionIn.GetZ(),
        0.f, 0.f, 0.f, 1.f
    };

    *this = (*this)*mat;
}

void Matrix4x4::setPosition(Vector3D vec3In)
{
    matrix[3] = vec3In.x;
    matrix[7] = vec3In.y;
    matrix[11] = vec3In.z;
}

void Matrix4x4::rotateX(GLfloat degrees)
{
    GLfloat rad = deg2radf(degrees);

    Matrix4x4 temp;
    temp.identity();
    temp(5) = cosf(rad);
    temp(6) = sinf(rad);
    temp(9) = -sinf(rad);
    temp(10) = cosf(rad);

    *this = (*this)*temp;
}

void Matrix4x4::rotateY(GLfloat degrees)
{
    GLfloat rad = deg2radf(degrees);

    Matrix4x4 temp;
    temp.identity();
    temp(0) = cosf(rad);
    temp(2) = -sinf(rad);
    temp(8) = sinf(rad);
    temp(10) = cosf(rad);

    *this = *this * temp;
}

void Matrix4x4::rotateZ(GLfloat degrees)
{
    GLfloat rad = deg2radf(degrees);

    Matrix4x4 temp;
    temp.identity();
    temp(0) = cosf(rad);
    temp(1) = sinf(rad);
    temp(4) = -sinf(rad);
    temp(5) = cosf(rad);

    *this = *this * temp;
}

void Matrix4x4::lookAt(const Vector3D &eye, const Vector3D &at, const Vector3D &up)
{
    Vector3D f = at-eye;    //forward
    f.normalize();
    Vector3D s = Vector3D::cross(f, up);   //sideways
    s.normalize();
    Vector3D u = Vector3D::cross(s, f);     //up

    *this =
    {
        s.GetX(),  s.GetY(),  s.GetZ(), -Vector3D::dot(s, eye),
                u.GetX(),  u.GetY(),  u.GetZ(), -Vector3D::dot(u, eye),
                -f.GetX(), -f.GetY(), -f.GetZ(), Vector3D::dot(f, eye),
                0.f, 0.f, 0.f, 1.f
    };
}

void Matrix4x4::frustum(float left, float right, float bottom, float top, float n, float f)
{
    *this =
    {
        2*n/(right-left),    0.f,                (right+left)/(right-left),  0.f,
        0.f,                  2*n/(top-bottom),  (top+bottom)/(top-bottom),  0.f,
        0.f,                    0.f,                -(f+n)/(f-n),     -2*f*n/(f-n),
        0.f,                    0.f,                -1.0f,                      0.f
    };
}

void Matrix4x4::perspective(float fovy, float aspectRatio, float n, float f)
{
    if (fovy <= 0.f)
        fovy = 30.f;
    if (aspectRatio <= 0.f)
        aspectRatio = 1.3f;
    if (f - n <= 0.f)
    {
        n = 1.f;
        f = 100.f;
    }

    GLfloat uh = static_cast<float>(1/std::tan(deg2rad(static_cast<double>(fovy)/2)));
    GLfloat uw = (1/aspectRatio) * uh;

    *this =
    {
        uw,     0.f,    0.f,                                        0.f,
        0.f,    uh,     0.f,                                        0.f,
        0.f,    0.f,    -(f)/(f-n),    -2 * f*n/(f-n),
        0.f,    0.f,    -1.f,                                        0.f
    };
}

void Matrix4x4::ortho(float l, float r, float b, float t, float n, float f)
{
    *this =
    {
        2.f/(r-l), 0.f, 0.f, 0.f,
                0.f, 2.f/(t-b), 0.f, 0.f,
                0.f, 0.f, -2.f/(f-n), 0.f,
                -(r+l)/(r-l), -(t+b)/(t-b), -(f+n)/(f-n), 1.f
    };
}

GLfloat Matrix4x4::getFloat(int space)
{
    return matrix[space];
}

/// Gets the row Vector from a Vector4d.
///
/// Written by candidate: 8004
/// \param rowV
/// \return If rowV is less than 0 or greater than 3, then return 0. If else return values times 4 gradually.
///
Vector4d Matrix4x4::getRowVec(int rowV)
{
    rowV -= 1;
    if (rowV < 0 || rowV > 3) return {0, 0, 0, 0};
    else return {mFloat[rowV], mFloat[4+rowV], mFloat[8+rowV], mFloat[12+rowV]};
}

/// Gets the column Vector from a Vector4d.
///
/// Written by candidate: 8004
/// \param colV
/// \return
///
Vector4d Matrix4x4::getColVec(int colV)
{
    switch (colV) {
    case 1:
        return {mFloat[0], mFloat[1], mFloat[2], mFloat[3]};
    case 2:
        return {mFloat[4], mFloat[5], mFloat[6], mFloat[7]};
    case 3:
        return {mFloat[8], mFloat[9], mFloat[10], mFloat[11]};
    case 4:
        return {mFloat[12], mFloat[13], mFloat[14], mFloat[15]};
    default:
        return {0, 0, 0, 0};
    }
}
