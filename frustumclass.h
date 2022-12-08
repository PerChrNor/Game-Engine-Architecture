#ifndef FRUSTUMCLASS_H
#define FRUSTUMCLASS_H

#include "Math/matrix4x4.h"
#include "QMatrix4x4"
#include "Math/vector3d.h"
#include "Math/vector4d.h"
#include "visualobject.h"
#include "camera.h"

/// A class which uses the camera class to make frustum.
///
/// This class was made to seperate Camera fuctions and Frustum functions. It technically inherits from the Camera class.
/// The functions in here are divided into the used functions and the unused functions.
/// The unused ones are still present since they are good examples on different methods.
///
/// Written by candidate: 8004
class FrustumClass
{
public:
    FrustumClass();

    Camera *tempCamera;

    void saveCamera(Camera *temp)
    {
        tempCamera = temp;
    }

    float frustum[6][4];

    void extractFrustum();
    bool pointInFrustum( float x, float y, float z );
    bool sphereInFrustum ( float x, float y, float z, float radius );
    float sphereInFrustumDistance( float x, float y, float z, float radius );
    bool cubeInFrustum( float x, float y, float z, Vector3D size );


    // --- Unused Frustum functions, though other types of finding frustum, so useful examples --- //
    Vector4d mFrustum[6];

    float mFieldOfView{45.f};
    float mAspectRatio{1.7778f}; // 16/9
    float mNearPlane{0.1f};
    float mFarPlane{100.f};

    Vector3D mFrustumCornerNear;
    Vector3D mFrustumCornerFar;

    void calculateFrustum();
    void createFrustum(Matrix4x4 pMatrix, Matrix4x4 vMatrix);
    bool frustumCull(QVector3D point);
    bool frustumCull(collisionComponent OBB, QVector3D scale);
};

#endif // FRUSTUMCLASS_H
