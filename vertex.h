#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include "Math/vector3d.h"
#include "Math/vector2d.h"
#include <QVector3D>

class Vertex {
   //! Overloaded ostream operator which writes all vertex data on an open textfile stream
   friend std::ostream& operator<< (std::ostream&, const Vertex&);

   //! Overloaded ostream operator which reads all vertex data from an open textfile stream
   friend std::istream& operator>> (std::istream&, Vertex&);

private:
   float m_xyz[3];       	// position i 3D
   float m_rgb[3];    // normal in 3D or rgb colors
   float m_st[2];        	// texture coordinates if used

public:
   Vertex();
   Vertex(float x, float y, float z, float r, float g, float b);
   Vertex(float x, float y, float z, float r, float g, float b, float s, float t);
   Vertex(Vector3D a, Vector3D b, float s, float t);
   Vertex(Vector3D a, Vector3D b, Vector2D c);
   void set_xyz (float x, float y, float z);
   void set_rgb (float r, float g, float b);
   void set_rgb (Vector3D &v);
   void set_uv (float u, float v);
   float getX();
   float getY();
   float getZ();
   QVector3D getXYZ();
   float getR();
   float getG();
   float getB();


};

#endif // VERTEX_H
