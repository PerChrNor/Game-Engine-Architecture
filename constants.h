#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

#define MAX_ENTITIES 65535
#define MAX_ENTITIES_TYPE unsigned short
#define MAX_COMPONENTS 100


const std::string ProjectFolder{"../GEA2020/"};
const std::string AssetFilePath{ProjectFolder + "Assets/"};
const std::string ShaderFilePath{ProjectFolder + "Shaders/"};

enum shaders
{
    PLAINSHADER,
    TEXTURESHADER,
    PHONGSHADER,
    PARTICLESHADER,
    MOUSEPICKERSHADER,
    SKYBOXSHADER,
    NOSHADER = 65000
};

const int NumberOfShaders{6};
const int NumberOfTextures{16};
const int NumberOfMaterials{5};

const int EditorObjectNumber{2};

enum EditorObjects
{
    GRID,
    AXIS
};

enum Colliders
{
    NOCOLLIDER = -1,
    OBB,
    SPHERE
};

#endif // CONSTANTS_H
