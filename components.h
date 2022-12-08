#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <vector>
#include "Math/vector3d.h"
#include "constants.h"
#include "gltypes.h"
#include <QMatrix4x4>
#include "vertex.h"

#ifdef _WIN32
#include "AL/al.h"
#include "AL/alc.h"
#endif
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

/// Holds the variables needed to draw the entity
///
/// This struct holds the vertices created when reading the obj-file for the entity and the arrays the program draws the mesh from.
/// Written by candidate 8008
struct meshComponent
{
    MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};
    std::string mMeshName;
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};
    unsigned short Indexcount{0};
    GLenum mDrawType{GL_TRIANGLES};
    std::vector <Vertex> mObjectVertecies;
    bool isInFrustum = true;

};

/// Holds variables needed to decide the transform of the entity, and variables needed to create a forward-vector.
/// Written by candidate 8008
struct transformComponent
{
    transformComponent()
    {
        position = QVector3D(0,0,0);
        rotation = QVector3D(0,0,0);
        transformationMatrix.setToIdentity();
    }
    unsigned short mEntityID{65535};
    QVector3D position{};
    QVector3D rotation{};
    QVector3D scale{1,1,1};
    QMatrix4x4 transformationMatrix;
    /// The forward vector for each entity
    QVector3D mForward{0.f,0.f,-1.f};
    /// The right vector for each entity
    QVector3D mRight{1.f, 0.f, 0.f};
    /// The up vector for each entity
    QVector3D mUp{0.f, 1.f, 0.f};;
};

/// Holds variables needed for some shaders
/// Written by candidate 8008
struct materialComponent
{
    unsigned short mMaterialID{65535};
    unsigned short mShader{NOSHADER};
    unsigned char mSpecularExponent{0};
    unsigned short mTextureId{0};
    GLfloat mSpecularStrength{0.f};
    Vector3D mColor{};
};

/// Holds variables to keep track of the materials given to an entity.
/// Written by candidate 8008
struct MaterialID
{
    unsigned short mMaterial{0};
    /// The ID assigned to identify entities when using mouse picking
    unsigned short mousePickerID{0};
};

/// \brief The soundComponent struct hold all the information the sound source needs
///
struct soundComponent
{
    /// \brief mName is a custom name for recognition purposes
    std::string mName;

    /// \brief mSource a value used by OpenAL to save the source
    ALuint mSource;

    /// \brief mBuffer a value used by OpenAL to create the buffer
    ALuint mBuffer;

    Vector3D mVelocity{0,0,0}; //velocity for the entity should not be defined in sound

    /// \brief mLoop a bool that decides if the sound clip will loop or not
    bool mLoop = false;

    /// \brief mGain is the sounds volume of a certain sound source
    float mGain = 1.0f;

    /// \brief mPosition the position of where the sound is coming from.
    Vector3D mPosition;

};

/// \brief The collisionComponent struct holds all information the collider needs.
///
struct collisionComponent
{

      /// \brief mShape The actuall shape of the collider, in this case, Box or Sphere.
      std::string mShape = "";
      /// \brief mType What kind of collider the Entity will have, Static or Dynamic.
      std::string mType = "";
      /// \brief mCenter The point defined at the middle of the Entity.
      QVector3D mCenter{0,0,0};
      /// \brief mHasCollider a Bool flipped to true when a collider is attached.
      bool mHasCollider = false;
      /// \brief EntitiesCollidingWith A vector of ints identifying the Entities colliding.
      ///This Vector stores integers coresponding to the ID's of the Entities colligiong with the the current Entitiy.
      ///The Entities have a unique ID so it will always identify the correct collision.
      ///It is a vector since an Entity can collide with multiple things at the same time.
      std::vector<int> EntitiesCollidingWith;

      /// \brief mRadius if its a sphere collider this value is used for shape and size.
      float mRadius = 0;

      //if its a box collider
      /// \brief mMinPoint the minimums point used if a box collider is used, stored in world space.
      Vector3D mMinPoint{0,0,0};
      /// \brief mMaxPoint the maximums point used if a box collider is used, stored in world space.
      Vector3D mMaxPoint{0,0,0};
      /// \brief mBoxSize is the size based on local space.
      Vector3D mBoxSize{0,0,0};

};

///
/// \brief The particleComponent struct holds the values for the particles themselves
/// Written by candidate 8003
struct particleComponent
{
    //Emitter
    QVector3D mPos; //posision
    QVector3D mVelocity;
    float mLifeSpan{1.f};
    float mSizeBegin, mSizeEnd;

    //Affector
    float mRotation;
    float mScale;
    float mLifeRemaining{0.0f};

    bool Active = false;

};

///
/// \brief The ParticleEmitter struct, holds the variables for the emitter function
/// Written by candidate 8003
struct ParticleEmitter
{
    //Emitter
    QVector3D mPos; //posision
    QVector3D mVelocity, mVelocityAlt; //velocity and the alternating velocity
    float mLifeSpan{1.f};
    float mSizeBegin, mSizeEnd, mSizeAlt; //starting size, ending size and size alternating
};

#endif // COMPONENTS_H
