#include "coreengine.h"

#include "rendersystem.h"
#include "resourcemanager.h"
#include "renderwindow.h"

CoreEngine::CoreEngine()
{
    mRenderSystem = new renderSystem();

    //ResourceManager::getInstance().mCoreEngine = this;
}

void CoreEngine::prepareComponentArrays(ResourceManager *currentManager)
{
    for (int i = 0; i < mEntityCount+1; i++)
    {
        mMeshes[i] = currentManager->mEntities[i].mMesh;
        mTransforms[i] = currentManager->mEntities[i].mTransform;
        mMaterialIDs[i] = currentManager->mEntities[i].mMaterialID;
    }
    //mRenderSystem->mNumberOfMeshes = mEntityCount;
}

MAX_ENTITIES_TYPE CoreEngine::getNewEntityID()
{
    if(mEntityCount == MAX_ENTITIES)
    {
        mEntityCount = 1;
    }
    else
    {
        mEntityCount++;
    }
    return mEntityCount - 1;
}
