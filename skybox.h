#ifndef SKYBOX_H
#define SKYBOX_H

#include "visualobject.h"
///
/// \brief The SkyBox class.
/// by candiate 8003.
/// Sets up the skybox vertices and indicies.
/// makes init() and draw() function.
///
class SkyBox : public VisualObject
{
public:
    SkyBox();
    ~SkyBox() override = default;

    virtual void init() override;
    virtual void draw() override;
};

#endif // SKYBOX_H
