#ifndef CUBE_H
#define CUBE_H

#include "visualobject.h"
#include "interactiveobject.h"

class Cube : public InteractiveObject
{
public:
    Cube();

    ~Cube() override;

    virtual void init() override;
    virtual void draw() override;
};

#endif // CUBE_H
