#ifndef OBJMESH_H
#define OBJMESH_H

#include "interactiveobject.h"
#include <sstream>

#include <QOpenGLFunctions_4_1_Core>

#include "vertex.h"
#include "gltypes.h"
#include "constants.h"
#include "components.h"

class objMesh : public InteractiveObject
{
public:
    objMesh(std::string fileName);
    objMesh(std::string cameraName, Vector3D cornerNear, Vector3D cornerFar);
    ~objMesh() override;

    virtual void draw() override;
    virtual void init() override;

    void readFile(std::string fileName);
    void makeVertex();
    void moveAlongCurve(unsigned int increment, std::vector<Vector3D> points, VisualObject* ground);
    void makeFrustum(Vector3D cornerNear, Vector3D cornerFar);

    std::string mMeshName;

    std::ifstream objFile;
    std::string oneLine;
    std::string oneWord = "";
    std::stringstream sStream;
    float v1 = 0;
    float v2 = 0;
    float v3 = 0;
    std::vector<Vector3D> importedPos;
    std::vector<Vector3D> importedNorms;
    std::vector<QVector2D> importedUVs;
    size_t OBJi{2};

    int indexPos;
    int indexUVs;
    int indexNorms;
    int indices = 0;

    meshComponent mMesh;
};

#endif // OBJMESH_H
