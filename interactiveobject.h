#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "visualobject.h"

class InteractiveObject : public VisualObject
{
public:
    InteractiveObject(){};
    ~InteractiveObject() override;
     void init() override;
     void draw() override;
     void move(float x, float y, float z);
     void scale(float x, float y, float z);
     void rotate(int deg, float x, float y, float z);
     void barycentricMove(float x, float y, float z, VisualObject* ground);
};

#endif // INTERACTIVEOBJECT_H
