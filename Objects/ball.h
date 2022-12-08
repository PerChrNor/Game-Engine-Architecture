#ifndef BALL_H
#define BALL_H


#include "interactiveobject.h"

class Vertex;
typedef Vector3D Vec3;
class Ball : public InteractiveObject {
private:
   int m_rekursjoner;
   int m_indeks;               // brukes i rekursjon, til å bygge m_vertices

   void lagTriangel(const Vec3& v1, const Vec3& v2, const Vec3& v3);
   void subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n);
   void UnitBall();

public:
   Ball(int n=3);
   ~Ball();
   void init() override;
   void draw() override;

   void roll(float speed = 1.f, bool x =true, bool y = false, bool z = false);
   float mRollSpeed = 0.005f;

   bool sceneChange = false;

   void moveAlongCurve(unsigned int increment, std::vector<Vector3D> points);

};

#endif // BALL_H
