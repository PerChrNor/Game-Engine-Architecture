#ifndef XYZ_H
#define XYZ_H

#include "visualobject.h"
#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "Objects/grid_area.h"

class XYZ : public VisualObject
{
public:
    XYZ();
    ~XYZ() override;
    virtual void init() override;
    virtual void draw() override;

    Grid_Area *mGrid;

private:
    std::vector<Vertex> mVertices;
    QOpenGLContext *mContext;
};

#endif // XYZ_H
