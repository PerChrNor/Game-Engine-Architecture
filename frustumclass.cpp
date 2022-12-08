#include "frustumclass.h"

FrustumClass::FrustumClass()
{

}

/// Extracts the numbers for frustum so it can be used for culling.
///
/// This function will extract numbers from the PROJECTION matrix and MODELVIEW matrix to make the different planes of the frustum.
/// The function wil run in render in renderwindow.cpp, where it wil run each frame, calculating where the frustum is.
void FrustumClass::extractFrustum()
{
    float proj[16];
    float modl[16];
    float clip[16];
    float t;

    // Get the current PROJECTION matrix from OpenGL
    for ( int i=0; i<16; i++ )
        proj[i] = tempCamera->m_ProjectionMatrix.matrix[i];

    // Get the current MODELVIEW matrix from OpenGL
    for ( int i=0; i<16; i++ )
        modl[i] = tempCamera->m_ViewMatrix.matrix[i];

    // Combines the two matrices (multiply projection by modelview)
    clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
    clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
    clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
    clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

    clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
    clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
    clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
    clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

    clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
    clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
    clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
    clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

    clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
    clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
    clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

    // Extract the numbers for the RIGHT plane
    frustum[0][0] = clip[ 3] - clip[ 0];
    frustum[0][1] = clip[ 7] - clip[ 4];
    frustum[0][2] = clip[11] - clip[ 8];
    frustum[0][3] = clip[15] - clip[12];

    // Normalize the result
    t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
    frustum[0][0] /= t;
    frustum[0][1] /= t;
    frustum[0][2] /= t;
    frustum[0][3] /= t;

    // Extract the numbers for the LEFT plane
    frustum[1][0] = clip[ 3] + clip[ 0];
    frustum[1][1] = clip[ 7] + clip[ 4];
    frustum[1][2] = clip[11] + clip[ 8];
    frustum[1][3] = clip[15] + clip[12];

    // Normalize the result
    t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
    frustum[1][0] /= t;
    frustum[1][1] /= t;
    frustum[1][2] /= t;
    frustum[1][3] /= t;

    // Extract the BOTTOM plane
    frustum[2][0] = clip[ 3] + clip[ 1];
    frustum[2][1] = clip[ 7] + clip[ 5];
    frustum[2][2] = clip[11] + clip[ 9];
    frustum[2][3] = clip[15] + clip[13];

    // Normalize the result
    t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
    frustum[2][0] /= t;
    frustum[2][1] /= t;
    frustum[2][2] /= t;
    frustum[2][3] /= t;

    // Extract the TOP plane
    frustum[3][0] = clip[ 3] - clip[ 1];
    frustum[3][1] = clip[ 7] - clip[ 5];
    frustum[3][2] = clip[11] - clip[ 9];
    frustum[3][3] = clip[15] - clip[13];

    // Normalize the result
    t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
    frustum[3][0] /= t;
    frustum[3][1] /= t;
    frustum[3][2] /= t;
    frustum[3][3] /= t;

    // Extract the FAR plane
    frustum[4][0] = clip[ 3] - clip[ 2];
    frustum[4][1] = clip[ 7] - clip[ 6];
    frustum[4][2] = clip[11] - clip[10];
    frustum[4][3] = clip[15] - clip[14];

    // Normalize the result
    t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
    frustum[4][0] /= t;
    frustum[4][1] /= t;
    frustum[4][2] /= t;
    frustum[4][3] /= t;

    // Extract the NEAR plane
    frustum[5][0] = clip[ 3] + clip[ 2];
    frustum[5][1] = clip[ 7] + clip[ 6];
    frustum[5][2] = clip[11] + clip[10];
    frustum[5][3] = clip[15] + clip[14];

    // Normalize the result
    t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
    frustum[5][0] /= t;
    frustum[5][1] /= t;
    frustum[5][2] /= t;
    frustum[5][3] /= t;
}

/// Checks if points of a mesh are in the frustum.
///
/// This function checks for points in different objects' vertexes by looping through the six planes of the frustum.
/// If the point is behind at least one of the planes, then it will register it as within the frustum.
/// \param x
/// \param y
/// \param z
/// \return Returns false if the point is not within the planes of the frustum, return true otherwise.
///
bool FrustumClass::pointInFrustum(float x, float y, float z)
{
    int p;

    for( p = 0; p < 6; p++ )
        if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3] <= 0 )
            return false;
    return true;
}

/// Checks if a bounding sphere collision is in the frustum.
///
/// This function checks for a bounding sphere within the frustum by looping through all the six planes of the frustum.
/// We compare the distance to the bounding sphere's radius to see if it is in the frustum.
/// \param x
/// \param y
/// \param z
/// \param radius - the radius of the bounding sphere of the object.
/// \return Returns false if the bounding sphere is not within the planes of the frustum, return true otherwise.
///
bool FrustumClass::sphereInFrustum( float x, float y, float z, float radius )
{
   int p;

   for( p = 0; p < 6; p++ )
      if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3] <= -radius )
         return false;
   return true;
}

/// Checks if a bounding sphere collision is in the frustum, and finds it distance from the near plane.
///
/// This function checks for a bounding sphere within the frustum by looping through all the six planes of the frustum.
/// We compare the distance to the bounding sphere's radius to see if it is in the frustum.
/// We will also get the distance from the position to the near plane.
/// \param x
/// \param y
/// \param z
/// \param radius - the radius of the bounding sphere of the object.
/// \return Returns 0 if the bounding sphere is not within the frustum, returns the distance + radius of the object.
///
float FrustumClass::sphereInFrustumDistance( float x, float y, float z, float radius )
{
   int p;
   float d;

   for( p = 0; p < 6; p++ )
   {
      d = frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3];
      if( d <= -radius )
         return 0;
   }
   return d + radius;
}

/// Checks if a bounding box collision is in the frustum.
/// This function checks for a bounding box within the frustum by looping through all the six planes of the frustum.
/// We compare the distance to the bounding box's size to see if it is in the frustum.
/// \param x
/// \param y
/// \param z
/// \param size - this is basically the same as a box's "radius".
/// \return Returns false if the bounding box is not within the planes of the frustum, return true otherwise.
///
bool FrustumClass::cubeInFrustum( float x, float y, float z, Vector3D size )
{
   int p;

   for( p = 0; p < 6; p++ )
   {
      if( frustum[p][0] * (x - size.x) + frustum[p][1] * (y - size.y) + frustum[p][2] * (z - size.z) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x + size.x) + frustum[p][1] * (y - size.y) + frustum[p][2] * (z - size.z) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x - size.x) + frustum[p][1] * (y + size.y) + frustum[p][2] * (z - size.z) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x + size.x) + frustum[p][1] * (y + size.y) + frustum[p][2] * (z - size.z) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x - size.x) + frustum[p][1] * (y - size.y) + frustum[p][2] * (z + size.z) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x + size.x) + frustum[p][1] * (y - size.y) + frustum[p][2] * (z + size.z) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x - size.x) + frustum[p][1] * (y + size.y) + frustum[p][2] * (z + size.z) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x + size.x) + frustum[p][1] * (y + size.y) + frustum[p][2] * (z + size.z) + frustum[p][3] > 0 )
         continue;
      return false;
   }
   return true;
}

// --- Unused functions --- //

/// Calculates the frustum's corners on the near and far planes.
///
/// Uses information of the FOV and aspect ratio to calculate where and how the corners of the frustum will be.
void FrustumClass::calculateFrustum()
{
    float tanFOV = tanf(mFieldOfView);          // expensive calculation - save answer
    float widthNear = tanFOV * mNearPlane;      // oposite side
    float widthFar = tanFOV * mFarPlane;        // oposite side

    float heightNear = widthNear / mAspectRatio;
    float heightFar = widthFar / mAspectRatio;

    // camera looks down -Z so near and far-plane are negative when drawn
    mFrustumCornerNear = Vector3D(widthNear, heightNear, -mNearPlane);
    mFrustumCornerFar = Vector3D(widthFar, heightFar, -mFarPlane);
}

/// Creates the different planes of the frustum.
///
/// This function takes the Projection Matrix and the Modelview Matrix and creates the Frustum's planes.
/// \param pMatrix - wants the ProjectionMatrix
/// \param vMatrix - wants the ModelViewMatrix
///
void FrustumClass::createFrustum(Matrix4x4 pMatrix, Matrix4x4 vMatrix)
{
    Matrix4x4 clPlanes = pMatrix * vMatrix;

    //Right frustum plane
    mFrustum[0] =
    {
        Vector3D (clPlanes.getColVec(1).w - clPlanes.getColVec(1).x,
        clPlanes.getColVec(2).w - clPlanes.getColVec(2).x,
        clPlanes.getColVec(3).w - clPlanes.getColVec(3).x),
        clPlanes.getColVec(4).w - clPlanes.getColVec(4).x
    };
    mFrustum[0].normalize();

    //Left frustum plane
    mFrustum[1] =
    {
        Vector3D (clPlanes.getColVec(1).w + clPlanes.getColVec(1).x,
        clPlanes.getColVec(2).w + clPlanes.getColVec(2).x,
        clPlanes.getColVec(3).w + clPlanes.getColVec(3).x),
        clPlanes.getColVec(4).w + clPlanes.getColVec(4).x
    };
    mFrustum[1].normalize();

    //Bottom Frustum plane
    mFrustum[2] =
    {
        Vector3D (clPlanes.getColVec(1).w + clPlanes.getColVec(1).y,
        clPlanes.getColVec(2).w + clPlanes.getColVec(2).y,
        clPlanes.getColVec(3).w + clPlanes.getColVec(3).y),
        clPlanes.getColVec(4).w + clPlanes.getColVec(4).y
    };
    mFrustum[2].normalize();

    //Top Frustum plane
    mFrustum[3] =
    {
        Vector3D (clPlanes.getColVec(1).w - clPlanes.getColVec(1).y,
        clPlanes.getColVec(2).w - clPlanes.getColVec(2).y,
        clPlanes.getColVec(3).w - clPlanes.getColVec(3).y),
        clPlanes.getColVec(4).w - clPlanes.getColVec(4).y
    };
    mFrustum[3].normalize();

    //Near Frustum plane
    mFrustum[4] =
    {
        Vector3D (clPlanes.getColVec(1).w - clPlanes.getColVec(1).z,
        clPlanes.getColVec(2).w - clPlanes.getColVec(2).z,
        clPlanes.getColVec(3).w - clPlanes.getColVec(3).z),
        clPlanes.getColVec(4).w - clPlanes.getColVec(4).z
    };
    mFrustum[4].normalize();

    //Far Frustum plane
    mFrustum[5] =
    {
        Vector3D (clPlanes.getColVec(1).w + clPlanes.getColVec(1).z,
        clPlanes.getColVec(2).w + clPlanes.getColVec(2).z,
        clPlanes.getColVec(3).w + clPlanes.getColVec(3).z),
        clPlanes.getColVec(4).w + clPlanes.getColVec(4).z
    };
    mFrustum[5].normalize();
}

/// Checks for a QVector3D point of an objects mesh in the frustum, if not then culls it.
///
/// \param point
/// \return Returns false if the point is not within the planes of the frustum, return true otherwise.
///
bool FrustumClass::frustumCull(QVector3D point)
{
    for (int i{0}; i < 6; ++i)
    {
        if (mFrustum[i].x * point.x() + mFrustum[i].y * point.y() + mFrustum[i].z + point.z() + mFrustum[i].w <= 0)
            return false;
    }
    return true;
}

/// Checks for an Bounding collision and checks if it is within the frustum, if not then culls it.
///
/// \param OBB - A bounding volume collision, can be box shaped or sphere shaped.
/// \param scale - the radius of the box or sphere collision
/// \return Returns false if the collision is not within the planes of the frustum, return true otherwise.
///
bool FrustumClass::frustumCull(collisionComponent OBB, QVector3D scale)
{
    bool bInFrustum{false};
    QVector3D v = scale * OBB.mCenter;
    bInFrustum = frustumCull(v);

    return bInFrustum;
}
