#ifndef COREENGINE_H
#define COREENGINE_H

#include <vector>
#include "components.h"
#include "constants.h"
#include "entity.h"
#include "resourcemanager.h"

class CoreEngine
{
public:
    CoreEngine();

    meshComponent *mMeshes {nullptr};
    MaterialID *mMaterialIDs {nullptr};
    transformComponent *mTransforms {nullptr};
    //std::vector<Entity> mEntities;

    MAX_ENTITIES_TYPE mEntityCount{MAX_ENTITIES};

    MAX_ENTITIES_TYPE getNewEntityID();
    void prepareComponentArrays(ResourceManager *currentManager);

    class renderSystem *mRenderSystem{nullptr};

};
#endif // COREENGINE_H
