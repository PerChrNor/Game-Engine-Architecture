#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"

class light : public VisualObject
{
public:
    light();
    virtual void init() override;
    virtual void draw() override;

    float ambientStrength{1.3f};
    QVector3D ambientColor{0.3f, 0.3f, 0.3f};

    float lightStrength{1.7f};
    QVector3D lightColor{0.9f, 0.9f, 0.5f};

    float specularStrength{1.3f};
    int specularExponent{4};
};

#endif // LIGHT_H
