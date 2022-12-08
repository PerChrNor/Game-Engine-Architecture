#ifndef GRID_AREA_H
#define GRID_AREA_H

#include "visualobject.h"

class Grid_Area : public VisualObject
{
public:
    Grid_Area(int grids);
    ~Grid_Area() override;
    void init() override;
    void draw() override;

    std::vector<Vertex*> mGridLines;

    int mNumberOfGrids;
    float mOuterGridPosition;
    Vector3D mGridColor;
};
#endif // GRID_AREA_H
