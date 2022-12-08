#ifndef HEIGHTMAPTERRAIN_H
#define HEIGHTMAPTERRAIN_H

#include "visualobject.h"
#include "texture.h"

class HeightMapTerrain : public VisualObject
{
public:
    HeightMapTerrain(Texture *map, float hSpace = 1.f, float vSpace = 0.5f);
    virtual ~HeightMapTerrain() override;
    virtual void init() override;
    virtual void draw() override;

    void generateTerrain();

    Texture *mTexture{nullptr};
    unsigned int mWidth = 0, mDepth = 0;
    float mHSpace = 1.f, mVSpace = 1.f;
};

#endif // HEIGHTMAPTERRAIN_H
