#ifndef ENTITY_H
#define ENTITY_H

#include "components.h"

/// Holds any object that needs a combination of components
///
/// This class is mostly used to make objects which needs components. There is also a vector made of entities which is used to keep track of the different entities in the scene.
/// Written by candidate 8008
/// @see meshComponent, transformComponent, MaterialID, soundComponent, collisionComponent
class Entity
{
public:

    Entity(int id);

    int entityID;
    std::string mName{"NoName"};
    std::string mTag{""};

    meshComponent mMesh;
    transformComponent mTransform;
    MaterialID mMaterialID;
    soundComponent mSound;

    bool bCollidable{false}; //if true - has collider
    bool bHidden{false}; //if true - do not render

    collisionComponent mCollider;

};

#endif // ENTITY_H
