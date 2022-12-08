#include "movementsystem.h"
#include <QVector3D>

movementSystem::movementSystem()
{
    temp4x4.setToIdentity();
}

void movementSystem::updateRVector()
{
    QVector3D tempRVec;
    QMatrix4x4 tempMatrix;
    tempMatrix.setToIdentity();

    tempRVec.setX(mCurrentManager->mEntities[0].mTransform.mRight.x()); tempRVec.setY(mCurrentManager->mEntities[0].mTransform.mRight.y()); tempRVec.setZ(mCurrentManager->mEntities[0].mTransform.mRight.z());

    tempMatrix.rotate(yRot, QVector3D(0,1,0));

    tempRVec = tempMatrix * tempRVec;
    tempRVec.normalize();

    mCurrentManager->mEntities[0].mTransform.mRight.setX(tempRVec.x()); mCurrentManager->mEntities[0].mTransform.mRight.setY(tempRVec.y()); mCurrentManager->mEntities[0].mTransform.mRight.setZ(tempRVec.z());

    mCurrentManager->mEntities[0].mTransform.mRight.normalize();

    updateUpVector();
}

void movementSystem::updateFVector(float yRot)
{
    QVector3D tempFVec;
    QVector3D tempUpVec;
    QVector3D tempRVec;

    tempFVec.setX(mCurrentManager->mEntities[0].mTransform.mForward.x());  tempFVec.setY(mCurrentManager->mEntities[0].mTransform.mForward.y());  tempFVec.setZ(mCurrentManager->mEntities[0].mTransform.mForward.z());
    tempUpVec.setX(mCurrentManager->mEntities[0].mTransform.mUp.x());      tempUpVec.setY(mCurrentManager->mEntities[0].mTransform.mUp.y());      tempUpVec.setZ(mCurrentManager->mEntities[0].mTransform.mUp.z());
    tempRVec.setX(mCurrentManager->mEntities[0].mTransform.mRight.x());    tempRVec.setY(mCurrentManager->mEntities[0].mTransform.mRight.y());     tempRVec.setZ(mCurrentManager->mEntities[0].mTransform.mRight.z());

     QtempRotationMatrix.setToIdentity();

     QtempRotationMatrix.column(3) = tempFVec;
     QtempRotationMatrix.rotate(yRot/3, QVector3D(0,1,0));
     tempFVec = tempFVec * QtempRotationMatrix;

     temp4x4 = QtempRotationMatrix * QtempRotationMatrix;
     tempFVec = tempFVec * temp4x4;
     tempFVec.normalize();

     mCurrentManager->mEntities[0].mTransform.mForward.setX(tempFVec.x()); mCurrentManager->mEntities[0].mTransform.mForward.setY(tempFVec.y()); mCurrentManager->mEntities[0].mTransform.mForward.setZ(tempFVec.z());

     updateRVector();
}

void movementSystem::updateUpVector()
{
    mCurrentManager->mEntities[0].mTransform.mUp.crossProduct(mCurrentManager->mEntities[0].mTransform.mForward, mCurrentManager->mEntities[0].mTransform.mRight);
}

/// Does the actual movement of the player.
///
/// This function uses the calculated forward-vector to move the player according to the player rotation.
/// Written by candidate 8008
/// @param movingForward Used to decide wether the player is moving forwards or backwards.
void movementSystem::moveForward(bool movingForward)
{
    if (movingForward)
    {
        mCurrentManager->mEntities[0].mTransform.position = mCurrentManager->mEntities[0].mTransform.position + mCurrentManager->mEntities[0].mTransform.mForward * mCarspeed;
    }
    else if (!movingForward)
    {
        mCurrentManager->mEntities[0].mTransform.position = mCurrentManager->mEntities[0].mTransform.position - mCurrentManager->mEntities[0].mTransform.mForward * mCarspeed;
    }
}
