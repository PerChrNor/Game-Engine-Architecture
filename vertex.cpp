#include "vertex.h"

Vertex::Vertex()
{
    m_xyz[0] = 0;
    m_xyz[1] = 0;
    m_xyz[2] = 0;

    m_rgb[0] = 0;
    m_rgb[1] = 0;
    m_rgb[2] = 0;
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;

    m_rgb[0] = r;
    m_rgb[1] = g;
    m_rgb[2] = b;
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float s, float t)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;

    m_rgb[0] = r;
    m_rgb[1] = g;
    m_rgb[2] = b;

    m_st[0] = s;
    m_st[1] = t;
}

Vertex::Vertex(Vector3D a, Vector3D b, float s, float t)
{
    m_xyz[0] = a.GetX();
    m_xyz[1] = a.GetY();
    m_xyz[2] = a.GetZ();

    m_rgb[0] = b.GetX();
    m_rgb[1] = b.GetY();
    m_rgb[2] = b.GetZ();

    m_st[0] = s;
    m_st[1] = t;
}

Vertex::Vertex(Vector3D a, Vector3D b , Vector2D c)
{
    m_xyz[0] = a.GetX();
    m_xyz[1] = a.GetY();
    m_xyz[2] = a.GetZ();

    m_rgb[0] = b.GetX();
    m_rgb[1] = b.GetY();
    m_rgb[2] = b.GetZ();

    m_st[0] = c.getX();
    m_st[1] = c.getY();
}

void Vertex::set_xyz(float x, float y, float z)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
}

void Vertex::set_rgb(float r, float g, float b)
{
    m_rgb[0] = r;
    m_rgb[1] = g;
    m_rgb[2] = b;
}

void Vertex::set_rgb(Vector3D &v)
{
    set_rgb(v.GetX(), v.GetY(), v.GetZ());
}

void Vertex::set_uv(float u, float v)
{
    m_st[0] = u;
    m_st[1] = v;
}

float Vertex::getX()
{
    return m_xyz[0];
}

float Vertex::getY()
{
    return m_xyz[1];
}

float Vertex::getZ()
{
    return m_xyz[2];
}

QVector3D Vertex::getXYZ()
{
    QVector3D xyz;
    xyz.setX(m_xyz[0]);
    xyz.setY(m_xyz[1]);
    xyz.setZ(m_xyz[2]);
    return xyz;
}

std::ostream& operator<< (std::ostream& os, const Vertex& v)
{
  os << std::fixed;
  os << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
  os << "(" << v.m_rgb[0] << ", " << v.m_rgb[1] << ", " << v.m_rgb[2] << ") ";
  os << "(" << v.m_st[0] << ", " << v.m_st[1] << ") ";
  return os;
}

std::istream& operator>> (std::istream& is, Vertex& v)
{
// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
  char dum, dum2, dum3, dum4;
  is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
  is >> dum >> v.m_rgb[0] >> dum2 >> v.m_rgb[1] >> dum3 >> v.m_rgb[2] >> dum4;
  is >> dum >> v.m_st[0] >> dum2 >> v.m_st[1] >> dum3;
  return is;
}

float Vertex::getR()
{
    return m_rgb[0];
}

float Vertex::getG()
{
    return m_rgb[1];
}

float Vertex::getB()
{
    return m_rgb[2];
}
