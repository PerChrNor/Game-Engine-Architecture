#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include <QMatrix4x4>
#include <QVector4D>
#include <QVector2D>
#include "components.h"
#include "Math/matrix4x4.h"

struct TriangleStruct{
    TriangleStruct(Vertex v1, Vertex v2, Vertex v3) : V1{v1}, V2{v2}, V3{v3}{}

    TriangleStruct(Vertex v1, Vertex v2, Vertex v3, int n1, int n2, int n3) :
        V1{v1}, V2{v2}, V3{v3}, n1{n1}, n2{n2}, n3{n3} {}

    Vertex V1;
    Vertex V2;
    Vertex V3;

    int n1 = -1;
    int n2 = -1;
    int n3 = -1;
};

class VisualObject : public QOpenGLFunctions_4_1_Core{

public:

    VisualObject() { mTransformationMatrix.setToIdentity(); }
    ~VisualObject()
    {
        glDeleteVertexArrays(1, &mVAO);
        glDeleteBuffers(1, &mVBO);
    }

    virtual void init() = 0;
    virtual void draw()=0;

    ///Use to draw the normals of this mesh
    void drawNormals(GLint matrixUniform);

    struct Transformation {
        QVector4D translation;
        QVector4D rotation;
        QVector4D scale;
    };
    Transformation mTransformation;
    QMatrix4x4 mTransformationMatrix;

    std::vector<TriangleStruct> triangles;
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    void getTriangles();
    QVector3D barycentricCoords(const QVector2D &p1, const QVector2D &p2, const QVector2D &p3, const QVector2D &position);

    void readFile(std::string filename);
    void writeFile(std::string filename);

    // Smooth shading
    void calculateNormals(std::vector<Vertex>* vertices, bool smooth);

    std::vector<QVector3D> importedVertexNorms;
    std::vector<QVector3D> vertexPos;
    std::vector<QVector3D> smoothedVertexNorms;

    // Collision
    Vector3D boxMin;
    Vector3D boxMax;
    float boxMinX, boxMaxX, boxMinY, boxMaxY, boxMinZ, boxMaxZ;

    void setBoundingBox(std::vector<Vertex>& positions);
    void updateBoundingBox();
    bool CheckCollide(Vector3D Pmin, Vector3D Pmax);
    unsigned short mIndiceCount{0};

    Matrix4x4 mMatrix;

    //components
    transformComponent mTransform{};
    //meshComponent mMesh;
    std::string mName{"noname"};

    materialComponent *mMaterial{nullptr};
    class RenderWindow *mRenderWindow{nullptr};

protected:
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};

    GLint mMatrixUniform{0};

};



#endif // VISUALOBJECT_H
