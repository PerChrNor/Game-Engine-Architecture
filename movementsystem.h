#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "Math/vector3d.h"
#include <QMatrix4x4>
#include "resourcemanager.h"

/// The system which handles the movement of the car.
///
/// This class takes care of the players movement on the x-z plane. Changing the height is done in collisions.
/// Written by candidate 8008.
/// @see collisions
class movementSystem
{
private:
    QMatrix4x4 QtempRotationMatrix;
    /// The amount of rotation to be applied to the right vector.
    float yRot = 0;
    QMatrix4x4 temp4x4;

public:
    movementSystem();
    void updateFVector(float yRot);
    void updateRVector();
    void updateUpVector();
    /// The ResourceManager holding the entities.
    ResourceManager *mCurrentManager;
    void moveForward(bool movingForward);
    /// The speed of the car when moving forward.
    float mCarspeed{0};
};

#endif // MOVEMENTSYSTEM_H
