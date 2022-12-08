#include "collisions.h"
#include <algorithm>

//this class is ONLY for collision response
//the actual collision detection happenes in the Resource manager class


Collisions::Collisions()
{
}


void Collisions::initCollisions(){

    mPlayerEntity = getPlayer();
    mPlayerPosition = mPlayerEntity->mTransform.position;

}

void ::Collisions::checkAllCollisions(){
    mPlayerPosition = mPlayerEntity->mTransform.position;
    mTrackEntities = getTracks();
    playerHeight = checkPlayerHeight();
}




Entity* Collisions::getPlayer(){

    Entity* tempEntity{0};

    if(!mCurrentManager->mEntities.empty()){
        for(unsigned long long LL = 0; LL <= mCurrentManager->mEntities.size() - 1; LL++){
            if(mCurrentManager->mEntities[LL].mTag == "Player"){
                tempEntity  = &mCurrentManager->mEntities[LL];
                continue;
            }

        }
    }

    return tempEntity;
}


std::vector <Entity*> Collisions::getTracks(){

    std::vector <Entity*> tempEntityVec;

    if(!mPlayerEntity->mCollider.EntitiesCollidingWith.empty()){
    for(unsigned long long ECWLL = 0; ECWLL <= mPlayerEntity->mCollider.EntitiesCollidingWith.size() - 1; ECWLL++){
        for(unsigned long long ELL = 0; ELL <= mCurrentManager->mEntities.size() - 1; ELL++){
            if(mCurrentManager->mEntities[ELL].entityID == mPlayerEntity->mCollider.EntitiesCollidingWith[ECWLL] && mCurrentManager->mEntities[ELL].mTag == "Track"){
                tempEntityVec.push_back(&mCurrentManager->mEntities[ELL]);

            }
        }
    }

}
    return tempEntityVec;
}



float Collisions::checkPlayerHeight(){

    Vector3D tempPlayerPosition;
    tempPlayerPosition.x = mPlayerPosition.x();
    tempPlayerPosition.y = mPlayerPosition.y();
    tempPlayerPosition.z = mPlayerPosition.z();

    int indA = 0;
    int indB = 1;
    int indC = 2;

    Vector3D Vec1;
    Vector3D Vec2;
    Vector3D Vec3;

    Vector3D posVec;
    Vector3D rotVec;


    float height = -10000.f;

    // add if mtrackentities !empty
    for(unsigned long long LL = 0; LL <= mTrackEntities.size() - 1; LL++){

    //only works because there is only one object in the mTrackEntities vector, and the player is standing on the object
    // fixes needed, check agains multiple tracks, return a result it its not standing on any objects
    Vec1 = Vector3D(0,0,0);
    Vec2 = Vector3D(0,0,0);
    Vec3 = Vector3D(0,0,0);

    if(breaking){break;};


    while (height == -10000.f){




                Vec1.x = mTrackEntities[LL]->mMesh.mObjectVertecies[indA].getX() + mTrackEntities[LL]->mTransform.position.x();
                Vec1.y = mTrackEntities[LL]->mMesh.mObjectVertecies[indA].getY() + mTrackEntities[LL]->mTransform.position.y();
                Vec1.z = mTrackEntities[LL]->mMesh.mObjectVertecies[indA].getZ() + mTrackEntities[LL]->mTransform.position.z();

                posVec.x = mTrackEntities[LL]->mTransform.position.x();
                posVec.y = mTrackEntities[LL]->mTransform.position.y();
                posVec.z = mTrackEntities[LL]->mTransform.position.z();

                rotVec.x = mTrackEntities[LL]->mTransform.rotation.x();
                rotVec.y = mTrackEntities[LL]->mTransform.rotation.y();
                rotVec.z = mTrackEntities[LL]->mTransform.rotation.z();

                Vec1.rotatePointAroundPivot(Vec1, posVec, rotVec);


                Vec2.x = mTrackEntities[LL]->mMesh.mObjectVertecies[indB].getX() + mTrackEntities[LL]->mTransform.position.x();
                Vec2.y = mTrackEntities[LL]->mMesh.mObjectVertecies[indB].getY() + mTrackEntities[LL]->mTransform.position.y();
                Vec2.z = mTrackEntities[LL]->mMesh.mObjectVertecies[indB].getZ() + mTrackEntities[LL]->mTransform.position.z();


                Vec2.rotatePointAroundPivot(Vec2, posVec, rotVec);


                Vec3.x = mTrackEntities[LL]->mMesh.mObjectVertecies[indC].getX() + mTrackEntities[LL]->mTransform.position.x();
                Vec3.y = mTrackEntities[LL]->mMesh.mObjectVertecies[indC].getY() + mTrackEntities[LL]->mTransform.position.y();
                Vec3.z = mTrackEntities[LL]->mMesh.mObjectVertecies[indC].getZ() + mTrackEntities[LL]->mTransform.position.z();

                Vec3.rotatePointAroundPivot(Vec3, posVec, rotVec);


        height = Vector3D::barycenticHeight(tempPlayerPosition, Vec1, Vec2, Vec3);

        indA++;
        indB++;
        indC++;


        if(indA >= mTrackEntities[LL]->mMesh.Indexcount){

            indA = 0;
            indB = 1;
            indC = 2;
            if(LL == mTrackEntities.size() - 1 ){breaking = true;}

            break;
        }

    }

    }

    breaking = false;
    return height;
}


void Collisions::setPlayerheight(float height){

    if(height == -10000.f){
        return;
    }

    else{

        mPlayerEntity->mTransform.position.setY(height +0.5);
    }

}





























