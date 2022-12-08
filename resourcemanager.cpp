#include "resourcemanager.h"
//mergeconflice
#include <utility>
#include <QDir>
#include <QDebug>

#include "constants.h"
#include "texture.h"
#include "Shaders/phongshader.h"
#include "Shaders/textureshader.h"
#include "Shaders/plainshader.h"
#include "Shaders/particleshader.h"
#include "Shaders/mousepickershader.h"
#include "Shaders/skyboxshader.h"
#include "skybox.h"
#include "coreengine.h"
#include "objmesh.h"

ResourceManager::ResourceManager():
    mDevice(NULL),
    mContext(NULL)
{


}
/// The function used to create a new instance of the ResourceManager class.
/// Written by candidate 8008
ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager *mInstance = new ResourceManager();
    return *mInstance;
}

void ResourceManager::readTextures()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Skybox - a cubemap
    QDir tempDir2((AssetFilePath + "Textures/CubeMaps").c_str());
    if(tempDir2.exists())
    {
        QStringList filters;
        filters << "*1.bmp";
        tempDir2.setNameFilters(filters);
        qDebug() << "Cubemap texture number: " << tempDir2.entryInfoList().size();
        //read all cubemaps

        //read all regular textures
        for(QFileInfo var : tempDir2.entryInfoList())
        {
                qDebug() << "Texture name: " << var.fileName();
                Texture *temp = new Texture(var.fileName().toStdString(), true);
                mIndexOfLastTexture++;
                mTexture[mIndexOfLastTexture] = temp;
        }
    }
    else
    {
        qDebug() << "*** ERROR reading Cubemap textures *** : Asset-folder " << tempDir2.dirName() << " does not exist!";
    }

    //Regular textures
    QDir tempDir((AssetFilePath + "Textures").c_str());
    if(tempDir.exists())
    {
        QStringList filters;
        filters << "*.bmp";
        tempDir.setNameFilters(filters);
        qDebug() << "\n" << tempDir.entryInfoList().size() << "textures will be read out of"
                 << NumberOfTextures << "texture places in Engines texture array! \n" ;

//        Q_ASSERT(NumberOfTextures < tempDir.entryInfoList().size() &&
//                 "More textures in directory than NumberOfTextures");

        mIndexOfLastTexture++;
        mTexture[mIndexOfLastTexture] = new Texture(); //default texture

        //read all regular textures
        for(QFileInfo var : tempDir.entryInfoList())
        {
            qDebug() << "Texture name: " << var.fileName();
            Texture *temp = new Texture(var.fileName().toStdString());
            mIndexOfLastTexture++;
            mTexture[mIndexOfLastTexture] = temp;
        }
    }
    else
    {
        qDebug() << "*** ERROR reading textures *** : Asset-folder " << AssetFilePath.c_str() << " does not exist!";
    }


    //Bind textures to texture units(slots):
      for(int i = 0; i <= mIndexOfLastTexture;++i)
      {
          glActiveTexture(GL_TEXTURE0 + i);   //Activate Texture Unit (slot)
          if(!mTexture[i]->mCubemap[0]) //if not a cubemap
          {
              glBindTexture(GL_TEXTURE_2D, mTexture[i]->mTextureId);
              qDebug() << "mTexture[" << i <<"] GLTexture ID" << mTexture[i]->mTextureId << "bound to Texture Unit " << i;
          }
          else
          {
              glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture[i]->mTextureId);  //Skybox
               qDebug() << "mTexture[" << i <<"] Cubemap Texture ID" << mTexture[i]->mTextureId << "bound to Texture Unit " << i;
          }
      }

}

void ResourceManager::readObjs(std::string fileName)
{
    std::ifstream fileIn;
    fileIn.open(fileName, std::ifstream::in);

    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(fileName);

    //One line at a time-variable
    std::string oneLine;
    //One word at a time-variable
    std::string oneWord;

    std::vector<Vector3D> tempVert;
    std::vector<Vector3D> tempNorm;
    std::vector<QVector2D> tempUVs;

    unsigned int temp_index = 0;

    while(std::getline(fileIn, oneLine))
    {
        std::stringstream sStream;
        sStream << oneLine;

        oneWord = "";
        sStream >> oneWord;

        if(oneWord == "#")
        {
            continue;
        }
        if(oneWord == "")
        {
            continue;
        }
        if(oneWord == "v")
        {
            Vector3D tempVertex;
            sStream >> oneWord;
            tempVertex.x = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.y = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.z = std::stof(oneWord);

            tempVert.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            QVector2D tempUV;
            sStream >> oneWord;
            tempUV.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempUV.setY(std::stof(oneWord));

            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            Vector3D tempNormal;
            sStream >> oneWord;
            tempNormal.x = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.y = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.z = std::stof(oneWord);

            tempNorm.push_back(tempNormal);
            continue;
        }
        if (oneWord == "f")
        {
            int index, normal, uv;
            for(int i = 0; i < 3; i++)
            {
                sStream >> oneWord;
                std::stringstream tempWord(oneWord);
                std::string segment;
                std::vector<std::string> segmentArray;
                while(std::getline(tempWord, segment, '/'))
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);
                if (segmentArray[1] != "")
                    uv = std::stoi(segmentArray[1]);
                else
                {
                    uv = 0;
                }
                normal = std::stoi(segmentArray[2]);

                --index;
                --uv;
                --normal;

                if(uv > -1)
                {
                    Vertex tempVertices(tempVert[index], tempNorm[normal], tempUVs[uv].x(), tempUVs[uv].y());
                    mVertices.push_back(tempVertices);
                }
                else
                {
                    Vertex tempVertices(tempVert[index], tempNorm[normal], 0, 0);
                    mVertices.push_back(tempVertices);
                }
                mIndices.push_back(temp_index++);
            }
            continue;
        }
    }
    fileIn.close();
}

void ResourceManager::readShaders(RenderWindow *renderWindow)
{
    mShaderProgram[PLAINSHADER] = new PlainShader("plainshader");
    mShaderProgram[PLAINSHADER]->mRenderWindow = renderWindow;  //only for debugging
    mShaderProgram[PLAINSHADER]->setupShader();

    mShaderProgram[TEXTURESHADER] = new TextureShader("textureshader");
    mShaderProgram[TEXTURESHADER]->mRenderWindow = renderWindow;  //only for debugging
    mShaderProgram[TEXTURESHADER]->setupShader();

    mShaderProgram[PHONGSHADER] = new PhongShader("phongshader");
    mShaderProgram[PHONGSHADER]->mRenderWindow = renderWindow; //only for debugging
    mShaderProgram[PHONGSHADER]->setupShader();

    mShaderProgram[MOUSEPICKERSHADER] = new MousePickerShader("mousepickershader");
    mShaderProgram[MOUSEPICKERSHADER]->mRenderWindow = renderWindow; //only for debugging
    mShaderProgram[MOUSEPICKERSHADER]->setupShader();

    mShaderProgram[PARTICLESHADER] = new ParticleShader("particleshader");
    mShaderProgram[PARTICLESHADER]->mRenderWindow = renderWindow; //only for debugging
    mShaderProgram[PARTICLESHADER]->setupShader();

    mShaderProgram[SKYBOXSHADER] = new SkyboxShader("skyboxshader");
    mShaderProgram[SKYBOXSHADER]->mRenderWindow = renderWindow; //only for debugging
    mShaderProgram[SKYBOXSHADER]->setupShader();
}
/// Sets up the materials which are used by the shaders and gives them an ID so each entity can easily be assigned a material.
/// Written by candidate 8008
void ResourceManager::makeMaterials()
{
    // Plain gray
    materialComponent *tempMaterial = new materialComponent();
    tempMaterial->mShader = PLAINSHADER;
    tempMaterial->mColor = Vector3D{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 0;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    // Texture for dog
    tempMaterial = new materialComponent();
    tempMaterial->mShader = TEXTURESHADER;
    tempMaterial->mTextureId = mTexture[2]->id();
    tempMaterial->mColor = Vector3D{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 1;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    // Shiny Phong
    tempMaterial = new materialComponent();
    tempMaterial->mShader = PHONGSHADER;
    tempMaterial->mTextureId = mTexture[2]->id();
    tempMaterial->mColor = Vector3D{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 2;
    tempMaterial->mSpecularStrength = 0.9f;
    tempMaterial->mSpecularExponent = 164;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    // Dull Phong
    tempMaterial = new materialComponent();
    tempMaterial->mShader = PHONGSHADER;
    tempMaterial->mTextureId = mTexture[2]->id();
    tempMaterial->mColor = Vector3D{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 3;
    tempMaterial->mSpecularStrength = 0.1f;
    tempMaterial->mSpecularExponent = 1;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    // Mousepicker
    tempMaterial = new materialComponent();
    tempMaterial->mShader = MOUSEPICKERSHADER;
    tempMaterial->mColor = Vector3D{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 4;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    // ParticleSystem
    tempMaterial = new materialComponent();
    tempMaterial->mShader = PARTICLESHADER;
    tempMaterial->mColor = Vector3D{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 5;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;

    // Skybox
    tempMaterial = new materialComponent();
    tempMaterial->mShader = SKYBOXSHADER;
    tempMaterial->mTextureId = mTexture[0]->id();
    tempMaterial->mColor = Vector3D{0.3f, 0.3f, 0.3f};
    tempMaterial->mMaterialID = 6;
    mMaterial[tempMaterial->mMaterialID] = tempMaterial;
}
/// Sets up the variables required to draw a mesh.
///
/// For a new mesh it reads the obj file which is sent in and sets the correct values for vertecies, indecies and their array buffers.
/// For an old mesh it goes through the previously read meshes and finds the index which has the same mesh.
/// When the right index is found the information from that index is copied over to the new entity without having to read the obj file again.
/// Written by candidate 8008, with help from candidate 8002
/// @param mesh holds the filepath of the obj-file that the entity will use as a mesh.
/// @param entity gives the vector index for the entity that will hold the mesh.
/// @see readObjs, newMeshComponent
int ResourceManager::getMeshComponentIndex(std::string mesh, int entity)
{
    if(mMeshKey.find(mesh) != mMeshKey.end())
    {
        int meshIndex = mMeshKey.size();
        mMeshKey[mesh] = meshIndex;
        std::cout << "Old mesh : " + mesh << std::endl;
        int index{0};
        for (unsigned long long i{0}; i < mMeshComponents.size(); i++)
        {
            if (mMeshComponents[i].mMeshName == mesh)
            {
                index = i;
            }
        }

        mEntities[entity].mMesh.mMeshName = mMeshComponents[index].mMeshName;
        mEntities[entity].mMesh.Indexcount = mMeshComponents[index].Indexcount;
        mEntities[entity].mMesh.mVAO = mMeshComponents[index].mVAO;
        mEntities[entity].mMesh.mVBO = mMeshComponents[index].mVBO;
        mEntities[entity].mMesh.mEAB = mMeshComponents[index].mEAB;
        mEntities[entity].mMesh.mObjectVertecies = mMeshComponents[index].mObjectVertecies;

        return meshIndex;
    }
    else
    {
        int index = mMeshKey.size();
        mMeshKey[mesh] = index;
        mMeshComponents.push_back(newMeshComponent(mesh));
        std::cout << "New mesh : " + mMeshComponents[index].mMeshName << std::endl;
        std::cout << "Size : " << mMeshComponents[index].Indexcount << std::endl;
        std::cout << "VAO : " << mMeshComponents[index].mVAO << std::endl;
        std::cout << "VBO : " << mMeshComponents[index].mVBO << std::endl;
        std::cout << "EAB : " << mMeshComponents[index].mEAB << std::endl;

        mEntities[entity].mMesh.mMeshName = mMeshComponents[index].mMeshName;
        mEntities[entity].mMesh.Indexcount = mMeshComponents[index].Indexcount;
        mEntities[entity].mMesh.mVAO = mMeshComponents[index].mVAO;
        mEntities[entity].mMesh.mVBO = mMeshComponents[index].mVBO;
        mEntities[entity].mMesh.mEAB = mMeshComponents[index].mEAB;
        mEntities[entity].mMesh.mObjectVertecies = tempVertecies;
        mMeshComponents[index].mObjectVertecies = tempVertecies;


        tempIndicies.clear();
        tempVertecies.clear();

        return entity;
    }
}
/// Creates an entity and updates the mEntities vector. Also creates a name and tag for the entity
/// Written by candidate 8008 and 8002.
/// @param entityName The name of the entity. Used in the entity outliner.
/// @param entityTag The collision tag of the entity. Used when setting up collisions
unsigned short ResourceManager::makeEntity(std::string entityName, std::string entityTag)
{
    unsigned int i = mEntities.size();
    mEntities.push_back(Entity(i));
    mEntities[i].mName = entityName;

    mEntities[i].mTag = entityTag;
    return i;
}

void ResourceManager::attachMesh(const std::string &mesh, int entity, renderSystem *mRenderer)
{
    mMeshMap[entity] = getMeshComponentIndex(mesh, entity);
    mRenderer->mNumberOfMeshes++;
}

void ResourceManager::attachTransform(const transformComponent &transform, int entity)
{
    mTransformMap[entity] = transform;
}

void ResourceManager::attachMaterial(const int materialIndex, int entity)
{
    mEntities[entity].mMaterialID.mMaterial = materialIndex;
}
/// Sets up the buffers, vertecies and indecies when reading a new obj-file
/// Written by candidate 8008
/// @param mesh The file path for the obj-file which holds the mesh.
meshComponent ResourceManager::newMeshComponent(std::string mesh)
{

    meshComponent mMesh;
    mIndices.clear();
    mVertices.clear();
    readObjs(mesh);

    mMesh.mMeshName = mesh;
    mMesh.Indexcount = mIndices.size();

    glGenVertexArrays(1, &mMesh.mVAO);
    glGenBuffers(1, &mMesh.mEAB);
    glGenBuffers(1, &mMesh.mVBO);

    glBindVertexArray(mMesh.mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mMesh.mVBO);

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*)(3 * sizeof (GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*)(6 * sizeof (GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mMesh.mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<unsigned int>(mIndices.size()) * sizeof (GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);


    tempVertecies = mVertices;
    return mMesh;
}

//------------SOUND-----------------

bool ResourceManager::soundInit()
{
    qDebug() << "Initilize OpenAL!";
    mDevice = alcOpenDevice(NULL);
    if (mDevice)
    {
        mContext = alcCreateContext(mDevice, NULL);
        alcMakeContextCurrent(mContext);
    }

    // Generate buffers
    alGetError();

    if (!mDevice)
    {
        qDebug() << "No device ";
    }
    else
        qDebug() << "Device made ";


    return true;
}


void ResourceManager::cleanUp()
{
    mContext = alcGetCurrentContext();
    mDevice = alcGetContextsDevice(mContext);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

bool ResourceManager::checkError()
{
    switch (alGetError())
    {
    case AL_NO_ERROR:
        break;
    case AL_INVALID_NAME:
        qDebug() << "Invalid name!";
        return false;
    case AL_INVALID_ENUM:
        qDebug() << "Invalid enum!";
        return false;
    case AL_INVALID_VALUE:
        qDebug() << "Invalid value!";
        return false;
    case AL_INVALID_OPERATION:
        qDebug() << "Invalid operation!";
        return false;
    case AL_OUT_OF_MEMORY:
        qDebug() << "Out of memory!";
        return false;
    default: break;
    }
    return true;
}

void ResourceManager::attachSound(std::string name,short EntityNr , std::string fileName, bool loop, float soundLevel)
{
    mEntities[EntityNr].mSound.mName = name;
    mEntities[EntityNr].mSound.mLoop = loop;

    alGetError();
    alGenBuffers(1, &mEntities[EntityNr].mSound.mBuffer);
    checkWavError("alGenBuffers");
    alGenSources(1, &mEntities[EntityNr].mSound.mSource);
    checkWavError("alGenSources");
    alSourcef(mEntities[EntityNr].mSound.mSource, AL_PITCH, 1.0f);
    alSourcef(mEntities[EntityNr].mSound.mSource, AL_GAIN, soundLevel);

    ALfloat temp[3] = {mEntities[EntityNr].mTransform.position.x(),mEntities[EntityNr].mTransform.position.y(),mEntities[EntityNr].mTransform.position.z()};
    alSourcefv(mEntities[EntityNr].mSound.mSource, AL_POSITION, temp);
    ALfloat temp2[3] = {mEntities[EntityNr].mSound.mVelocity.x, mEntities[EntityNr].mSound.mVelocity.y,mEntities[EntityNr].mSound.mVelocity.z};
    alSourcefv(mEntities[EntityNr].mSound.mSource, AL_VELOCITY, temp2);

    alSourcei(mEntities[EntityNr].mSound.mSource, AL_LOOPING, mEntities[EntityNr].mSound.mLoop);

//    SoundSource* tempPtr = new SoundSource(name, loop, soundLevel);
//    tempPtr->setPos(pos);
    if (fileName != "")
        loadWave(fileName, mEntities[EntityNr].mSound.mSource,mEntities[EntityNr].mSound.mBuffer);

}

void ResourceManager::updateListener(Vector3D pos, Vector3D vel, Vector3D dir, Vector3D up)
{
    ALfloat posVector[3];
    ALfloat velVector[3];
    ALfloat headVector[6];
    posVector[0] = pos.x;
    posVector[1] = pos.y;
    posVector[2] = pos.z;
    velVector[0] = vel.x;
    velVector[1] = vel.y;
    velVector[2] = vel.z;
    headVector[0] = dir.x;
    headVector[1] = dir.y;
    headVector[2] = dir.z;
    headVector[3] = up.x;
    headVector[4] = up.y;
    headVector[5] = up.z;
    alListenerfv(AL_POSITION, posVector);
    alListenerfv(AL_VELOCITY, velVector);
    alListenerfv(AL_ORIENTATION, headVector);
}



bool ResourceManager::loadWave(std::string filePath, ALuint mSource, ALuint mBuffer)
{
    qDebug() << "Loading wave file!";
    ALuint frequency{};
    ALenum format{};
    wave_t* waveData = new wave_t();
    if (!WavFileHandler::loadWave(filePath, waveData))
    {
        qDebug() << "Error loading wave file!";
        return false; // error loading wave file data
    }

    frequency = waveData->sampleRate;

    switch (waveData->bitsPerSample)
    {
    case 8:
        switch (waveData->channels)
        {
        case 1: format = AL_FORMAT_MONO8;
            qDebug() << "Format: 8bit Mono";
            break;
        case 2: format = AL_FORMAT_STEREO8;
            qDebug() << "Format: 8bit Stereo";
            break;
        default: break;
        }
        break;
    case 16:
        switch (waveData->channels)
        {
        case 1: format = AL_FORMAT_MONO16;
            qDebug() << "Format: 16bit Mono";
            break;
        case 2: format = AL_FORMAT_STEREO16;
            qDebug() << "Format: 16bit Stereo";
            break;
        default: break;
        }
        break;
    default: break;
    }

    if (waveData->buffer == NULL)
    {
        qDebug() << " there is no buffer ";
    }

    std::ostringstream i2s;
    i2s << waveData->dataSize;
    std::string datasize;
    qDebug() << QString::fromStdString("DataSize: " + i2s.str() + " bytes");

    alGetError();
    alBufferData(mBuffer, format, waveData->buffer, waveData->dataSize, frequency);
    checkWavError("alBufferData");
    alSourcei(mSource, AL_BUFFER, mBuffer);
    checkWavError("alSourcei (loadWave)");

    qDebug() << "Loading complete!";
    if (waveData->buffer) delete waveData->buffer;
    if (waveData) delete waveData;
    return true;
}

bool ResourceManager::checkWavError(std::string name)
{


    switch (alGetError())
    {
    case AL_NO_ERROR:
        break;
    case AL_INVALID_NAME:
        qDebug() << QString::fromStdString("OpenAL Error: "+name+": Invalid name!");
        return false;
    case AL_INVALID_ENUM:
        qDebug() << QString::fromStdString("OpenAL Error: "+name+": Invalid enum!");
        return false;
    case AL_INVALID_VALUE:
        qDebug() << QString::fromStdString("OpenAL Error: "+name+": Invalid value!");
        return false;
    case AL_INVALID_OPERATION:
        qDebug() << QString::fromStdString("OpenAL Error: "+name+": Invalid operation!");
        return false;
    case AL_OUT_OF_MEMORY:
        qDebug() << QString::fromStdString("OpenAL Error: "+name+": Out of memory!");
        return false;
    default: break;
    }
    return true;
}

void ResourceManager::playSound(short EntityNr)
{
    alSourcePlay(mEntities[EntityNr].mSound.mSource);
}
void ResourceManager::pauseSound(short EntityNr)
{
    alSourcePause(mEntities[EntityNr].mSound.mSource);
}
void ResourceManager::stopSound(short EntityNr)
{
    alSourceStop(mEntities[EntityNr].mSound.mSource);
}


void ResourceManager::startAllSound(){

    for(unsigned long long L = 0; L <= mEntities.size(); L++){

        alSourcePlay(mEntities[L].mSound.mSource);
    }

}

void ResourceManager::stopAllSound(){

    for(unsigned long long L = 0; L <= mEntities.size(); L++){

        alSourceStop(mEntities[L].mSound.mSource);
    }

}

void ResourceManager::updateSoundPos(short entityNr)
{
    ALfloat temp[3] = {mEntities[entityNr].mTransform.position.x(),
                       mEntities[entityNr].mTransform.position.y(),
                       mEntities[entityNr].mTransform.position.z()};
    alSourcefv(mEntities[entityNr].mSound.mSource, AL_POSITION, temp);
}



//---------------Collision-----------------


void ResourceManager::attachSphereCollider(short EntityNr,std::string colliderType, float radius){
    //Fills out the information relevant to creating a sphere collider.
    //Here the shape of the collider, the size, the position and the collision type is set.
    //the unique information set here is the radius for the Sphere collider.
    mEntities[EntityNr].mCollider.mShape = "Sphere";
    mEntities[EntityNr].mCollider.mCenter = mEntities[EntityNr].mTransform.position;
    mEntities[EntityNr].mCollider.mRadius = radius;
    //A bool is set to easily chect is the object has a collider from other classes
    mEntities[EntityNr].mCollider.mHasCollider = true;

    if(colliderType == "static" || colliderType == "Static"){
        mEntities[EntityNr].mCollider.mType = "Static";
    }
    else if (colliderType == "dynamic" || colliderType == "Dynamic"){
        mEntities[EntityNr].mCollider.mType = "Dynamic";
    }
    else{
        qDebug() << "Error, no valid collider type for object: " << EntityNr << ", use Static or Dynamic";
    }
}


void ResourceManager::attachBoxCollider(short EntityNr,std::string colliderType, Vector3D maxPoint, Vector3D minPoint){

    //Fills out the information relevant to creating a Box collider.
    //Here the shape of the collider, the size, the position and the collision type is set.
    //the unique information set in this function is the Min and Max points for the Box collider
    mEntities[EntityNr].mCollider.mShape = "Box";
    mEntities[EntityNr].mCollider.mCenter = mEntities[EntityNr].mTransform.position;
    mEntities[EntityNr].mCollider.mMaxPoint.x = mEntities[EntityNr].mCollider.mCenter.x() + maxPoint.x;
    mEntities[EntityNr].mCollider.mMaxPoint.y = mEntities[EntityNr].mCollider.mCenter.y() + maxPoint.y;
    mEntities[EntityNr].mCollider.mMaxPoint.z = mEntities[EntityNr].mCollider.mCenter.z() + maxPoint.z;

    mEntities[EntityNr].mCollider.mMinPoint.x = mEntities[EntityNr].mCollider.mCenter.x() + minPoint.x;
    mEntities[EntityNr].mCollider.mMinPoint.y = mEntities[EntityNr].mCollider.mCenter.y() + minPoint.y;
    mEntities[EntityNr].mCollider.mMinPoint.z = mEntities[EntityNr].mCollider.mCenter.z() + minPoint.y;
    //A bool is set to easily chect is the object has a collider from other classes
    mEntities[EntityNr].mCollider.mHasCollider = true;
    //the original max point is saved to to be used for rescaling of the box in the inspector
    mEntities[EntityNr].mCollider.mBoxSize = maxPoint;

    if(colliderType == "static" || colliderType == "Static"){
        mEntities[EntityNr].mCollider.mType = "Static";
    }
    else if (colliderType == "dynamic" || colliderType == "Dynamic"){
        mEntities[EntityNr].mCollider.mType = "Dynamic";
    }
    else{
        qDebug() << "Error, no valid collider type for object: " << EntityNr << ", use Static or Dynamic";
    }

    qDebug() << mEntities[EntityNr].mCollider.mCenter;
}


//this function saves all the entities with a collider into a new vector,
//that way the collision system does not have to check the collision of any entity without a collider
void ResourceManager::CheckEntitiesWithCollider(){

    std::vector<Entity> entitiesWCollider;
    unsigned long long size = mEntities.size()-1;
    for(unsigned long long L = 0; L <= size; L++){

        if(mEntities[L].mCollider.mHasCollider){
            entitiesWCollider.push_back(mEntities[L]);
        }

    }

    mEntitiesWCollider = entitiesWCollider;
}

//This function seperates the Entity-With-A-Collider into two separate vectors
//This is done because it becomes faster to check collisions since the collision checker does not have to check collisions between two static objects,
//only between static and dynamic or dynamic and dynamic colliders
void ResourceManager::CheckEntitiesWithStaticDynamicCollider(){


    std::vector<Entity> DynamicColliders;
    std::vector<Entity> StaticColliders;

    unsigned long long size = mEntitiesWCollider.size();
    for(unsigned long long L = 0; L <= size-1; L++){

        if(mEntitiesWCollider[L].mCollider.mHasCollider == true){
            if(mEntitiesWCollider[L].mCollider.mType == "Dynamic"){
               DynamicColliders.push_back(mEntitiesWCollider[L]);
            }
            else if(mEntitiesWCollider[L].mCollider.mType == "Static"){
                StaticColliders.push_back(mEntitiesWCollider[L]);
             }
        }
    }

    mEntitiesWDynCollider = DynamicColliders;
    mEntitiesWStatCollider = StaticColliders;
}


//This function will check the collision of all object then save what each entity has collided with in a personal vector for each object.
void ResourceManager::checkCollisions(){

    //The function will first clear any collisions what happened the previous frame.
    //this is not a very efficient way of clearing the vectors, but it is the simplest way.
    for(unsigned long long L = 0; L <= mEntitiesWCollider.size()-1; L++){
        mEntitiesWCollider[L].mCollider.EntitiesCollidingWith.clear();
    }
    for(unsigned long long L = 0; L <= mEntitiesWDynCollider.size()-1; L++){
        mEntitiesWDynCollider[L].mCollider.EntitiesCollidingWith.clear();
    }

    for(unsigned long long L = 0; L <= mEntities.size()-1; L++){
        mEntities[L].mCollider.EntitiesCollidingWith.clear();
    }

    //This is where the collision is checked
    //It might look slow due to the double for-loop, but the function is fast due to the earlier vector split.
    for(unsigned long long L = 0; L<=mEntitiesWDynCollider.size()-1; L++){

        for(unsigned long long L2 = 0; L2 <= mEntitiesWCollider.size()-1; L2++){

            //this makes sure the object doesn't check collision with itself
            if(mEntitiesWCollider[L2].entityID != mEntitiesWDynCollider[L].entityID){

                //Here a linear search is done to the personal collision vector each object has
                //the search uses the other objects ID, if it exist it will skip to the next object, so it does not add it multiple times
                if(std::find(mEntitiesWDynCollider[L].mCollider.EntitiesCollidingWith.begin(),
                             mEntitiesWDynCollider[L].mCollider.EntitiesCollidingWith.end(),
                             mEntitiesWCollider[L2].entityID)
                             != mEntitiesWDynCollider[L].mCollider.EntitiesCollidingWith.end())
                {
                    continue;
                }

                else{

                    //If collisions happen the object needs know what kind of colliders each object has to use the correct collision method
                    //The collision methods added are: Sphere to Sphere, Box to Box, Sphere to Box and Box to Sphere.

                    //Sphere agains Sphere Collision
                    if(mEntitiesWCollider[L2].mCollider.mShape == "Sphere" && mEntitiesWDynCollider[L].mCollider.mShape == "Sphere"){

                       QVector3D VecDist(mEntitiesWCollider[L2].mCollider.mCenter - mEntitiesWDynCollider[L].mCollider.mCenter);
                       float fDistSq(QVector3D::dotProduct(VecDist,VecDist));

                       float fRadiiSumSquared(mEntitiesWDynCollider[L].mCollider.mRadius + mEntitiesWCollider[L2].mCollider.mRadius);
                       fRadiiSumSquared *= fRadiiSumSquared;

                       if(fDistSq <= fRadiiSumSquared){

                           mEntitiesWDynCollider[L].mCollider.EntitiesCollidingWith.push_back(mEntitiesWCollider[L2].entityID);
                           if(mEntitiesWCollider[L2].mCollider.mType == "Static"){
                             mEntitiesWCollider[L2].mCollider.EntitiesCollidingWith.push_back(mEntitiesWDynCollider[L].entityID);
                           }
                           if(L2 <= mEntitiesWStatCollider.size()-1){
                             mEntitiesWStatCollider[L2].mCollider.EntitiesCollidingWith.push_back(mEntitiesWDynCollider[L].entityID);
                           }


                       }

                    }

                    //Box against Box Collision
                    else if(mEntitiesWCollider[L2].mCollider.mShape == "Box" && mEntitiesWDynCollider[L].mCollider.mShape == "Box"){

                        if(
                                mEntitiesWDynCollider[L].mCollider.mMaxPoint.x > mEntitiesWCollider[L2].mCollider.mMinPoint.x &&
                                mEntitiesWDynCollider[L].mCollider.mMinPoint.x < mEntitiesWCollider[L2].mCollider.mMaxPoint.x &&
                                mEntitiesWDynCollider[L].mCollider.mMaxPoint.y > mEntitiesWCollider[L2].mCollider.mMinPoint.y &&
                                mEntitiesWDynCollider[L].mCollider.mMinPoint.y < mEntitiesWCollider[L2].mCollider.mMaxPoint.y &&
                                mEntitiesWDynCollider[L].mCollider.mMaxPoint.z > mEntitiesWCollider[L2].mCollider.mMinPoint.z &&
                                mEntitiesWDynCollider[L].mCollider.mMinPoint.z < mEntitiesWCollider[L2].mCollider.mMaxPoint.z){

                            mEntitiesWDynCollider[L].mCollider.EntitiesCollidingWith.push_back(mEntitiesWCollider[L2].entityID);
                            if(mEntitiesWCollider[L2].mCollider.mType == "Static"){
                              mEntitiesWCollider[L2].mCollider.EntitiesCollidingWith.push_back(mEntitiesWDynCollider[L].entityID);
                            }
                            if(L2 <= mEntitiesWStatCollider.size()-1){
                              mEntitiesWStatCollider[L2].mCollider.EntitiesCollidingWith.push_back(mEntitiesWDynCollider[L].entityID);
                            }

                        }

                    }

                    //Sphere agains Box Collision
                    else if(mEntitiesWCollider[L2].mCollider.mShape == "Sphere" && mEntitiesWDynCollider[L].mCollider.mShape == "Box"){

                        float sqDist = 0.0f;                       

                        float v = mEntitiesWCollider[L2].mCollider.mCenter.x();
                        if(v < mEntitiesWDynCollider[L].mCollider.mMinPoint.x){
                            sqDist += (mEntitiesWDynCollider[L].mCollider.mMinPoint.x - v) * (mEntitiesWDynCollider[L].mCollider.mMinPoint.x - v);
                        }
                        if(v > mEntitiesWDynCollider[L].mCollider.mMaxPoint.x){
                            sqDist += (v - mEntitiesWDynCollider[L].mCollider.mMaxPoint.x) * (v - mEntitiesWDynCollider[L].mCollider.mMaxPoint.x);
                        }


                        v = mEntitiesWCollider[L2].mCollider.mCenter.y();
                        if(v < mEntitiesWDynCollider[L].mCollider.mMinPoint.y){
                             sqDist += (mEntitiesWDynCollider[L].mCollider.mMinPoint.y - v) * (mEntitiesWDynCollider[L].mCollider.mMinPoint.y - v);
                        }
                        if(v > mEntitiesWDynCollider[L].mCollider.mMaxPoint.y){
                             sqDist += (v - mEntitiesWDynCollider[L].mCollider.mMaxPoint.y) * (v - mEntitiesWDynCollider[L].mCollider.mMaxPoint.y);
                        }


                        v = mEntitiesWCollider[L2].mCollider.mCenter.z();
                        if(v < mEntitiesWDynCollider[L].mCollider.mMinPoint.z){
                            sqDist += (mEntitiesWDynCollider[L].mCollider.mMinPoint.z - v) * (mEntitiesWDynCollider[L].mCollider.mMinPoint.z - v);
                        }
                        if(v > mEntitiesWDynCollider[L].mCollider.mMaxPoint.z){
                           sqDist += (v - mEntitiesWDynCollider[L].mCollider.mMaxPoint.z) * (v - mEntitiesWDynCollider[L].mCollider.mMaxPoint.z);
                        }

                        if(sqDist <= mEntitiesWCollider[L2].mCollider.mRadius){


                            mEntitiesWDynCollider[L].mCollider.EntitiesCollidingWith.push_back(mEntitiesWCollider[L2].entityID);
                            if(mEntitiesWCollider[L2].mCollider.mType == "Static"){
                              mEntitiesWCollider[L2].mCollider.EntitiesCollidingWith.push_back(mEntitiesWDynCollider[L].entityID);
                            }
                            if(!mEntitiesWStatCollider.empty() && L2 <= mEntitiesWStatCollider.size()-1){
                              mEntitiesWStatCollider[L2].mCollider.EntitiesCollidingWith.push_back(mEntitiesWDynCollider[L].entityID);
                            }

                        }

                    }

                    //Box agains Sphere Collision
                    else if(mEntitiesWCollider[L2].mCollider.mShape == "Box" && mEntitiesWDynCollider[L].mCollider.mShape == "Sphere"){

                        float sqDist = 0.0f;

                        float v = mEntitiesWDynCollider[L].mCollider.mCenter.x();
                        if(v < mEntitiesWCollider[L2].mCollider.mMinPoint.x){
                            sqDist += (mEntitiesWCollider[L2].mCollider.mMinPoint.x - v) * (mEntitiesWCollider[L2].mCollider.mMinPoint.x - v);
                        }
                        if(v > mEntitiesWCollider[L2].mCollider.mMaxPoint.x){
                            sqDist += (v - mEntitiesWCollider[L2].mCollider.mMaxPoint.x) * (v - mEntitiesWCollider[L2].mCollider.mMaxPoint.x);
                        }


                        v = mEntitiesWDynCollider[L].mCollider.mCenter.y();
                        if(v < mEntitiesWCollider[L2].mCollider.mMinPoint.y){
                             sqDist += (mEntitiesWCollider[L2].mCollider.mMinPoint.y - v) * (mEntitiesWCollider[L2].mCollider.mMinPoint.y - v);
                        }
                        if(v > mEntitiesWCollider[L2].mCollider.mMaxPoint.y){
                             sqDist += (v - mEntitiesWCollider[L2].mCollider.mMaxPoint.y) * (v - mEntitiesWCollider[L2].mCollider.mMaxPoint.y);
                        }


                        v = mEntitiesWDynCollider[L].mCollider.mCenter.z();
                        if(v < mEntitiesWCollider[L2].mCollider.mMinPoint.z){
                            sqDist += (mEntitiesWCollider[L2].mCollider.mMinPoint.z - v) * (mEntitiesWCollider[L2].mCollider.mMinPoint.z - v);
                        }
                        if(v > mEntitiesWCollider[L2].mCollider.mMaxPoint.z){
                           sqDist += (v - mEntitiesWCollider[L2].mCollider.mMaxPoint.z) * (v - mEntitiesWCollider[L2].mCollider.mMaxPoint.z);
                        }

                        if(sqDist <= mEntitiesWDynCollider[L].mCollider.mRadius){


                            mEntitiesWDynCollider[L].mCollider.EntitiesCollidingWith.push_back(mEntitiesWCollider[L2].entityID);
                            if(mEntitiesWCollider[L2].mCollider.mType == "Static"){
                              mEntitiesWCollider[L2].mCollider.EntitiesCollidingWith.push_back(mEntitiesWDynCollider[L].entityID);
                            }

                            if(!mEntitiesWStatCollider.empty() && L2 <= mEntitiesWStatCollider.size() - 1){
                              mEntitiesWStatCollider[L2].mCollider.EntitiesCollidingWith.push_back(mEntitiesWDynCollider[L].entityID);
                            }

                        }



                        }

                    }

                }
                }

            }

    //Here the objects ID is saved in each dynamic objects collision vector
    for(unsigned long long LL1 = 0; LL1 <= mEntitiesWDynCollider.size()-1; LL1++){
        for(unsigned long long LL2 = 0; LL2 <= mEntities.size()-1; LL2++){
            if(mEntitiesWDynCollider[LL1].entityID == mEntities[LL2].entityID){
                mEntities[LL2].mCollider.EntitiesCollidingWith
                        = mEntitiesWDynCollider[LL1].mCollider.EntitiesCollidingWith;
            }
        }
    }
    //here the Dynamic objects are saved in the Static objects collision vector
    for(unsigned long long LL1 = 0; LL1 <= mEntitiesWCollider.size()-1;LL1++){
        for(unsigned long long LL2 = 0; LL2 <= mEntities.size()-1; LL2++){
            if(mEntitiesWCollider[LL1].mCollider.mType == "Static"){
                if(mEntitiesWCollider[LL1].entityID == mEntities[LL2].entityID){
                    mEntities[LL2].mCollider.EntitiesCollidingWith
                            = mEntitiesWCollider[LL1].mCollider.EntitiesCollidingWith;
                 }

            }
        }
    }


};

//If the object moves the collider also have to move this sets all objects collider in relevance to is center point
void ResourceManager::updateColliderPos(short EntityNr){

    //Since there are now 3 separate vectors with varying sizes this has to be done separatly for each vector
    //This is done for consistency.
    //checks the first collision vector with both static and dynamic colliders, unless it is empty.
    if(!mEntitiesWCollider.empty()){
    for (unsigned long long mESize = 0; mESize <= mEntities.size()-1; mESize++){
        for(unsigned long long mEWCSize = 0; mEWCSize <= mEntitiesWCollider.size()-1; mEWCSize++){
            if(mEntities[mESize].entityID == mEntitiesWCollider[mEWCSize].entityID){
                mEntitiesWCollider[mEWCSize].mTransform.position = mEntities[mESize].mTransform.position;
            }

    }
}
}

    //checks the collision vector with dynamic colliders, unless it is empty.
    if(!mEntitiesWDynCollider.empty()){
    for (unsigned long long mESize = 0; mESize <= mEntities.size()-1; mESize++){
        for(unsigned long long mEWDCSize = 0; mEWDCSize <= mEntitiesWDynCollider.size()-1; mEWDCSize++){
            if(mEntities[mESize].entityID == mEntitiesWDynCollider[mEWDCSize].entityID){
                mEntitiesWDynCollider[mEWDCSize].mTransform.position = mEntities[mESize].mTransform.position;
            }

    }
}
}
    //checks the collision vector with static colliders, unless it is empty.
    if(!mEntitiesWStatCollider.empty()){
    for (unsigned long long mESize = 0; mESize <= mEntities.size()-1; mESize++){
        for(unsigned long long mEWSCSize = 0; mEWSCSize <= mEntitiesWStatCollider.size()-1; mEWSCSize++){
            if(mEntities[mESize].entityID == mEntitiesWStatCollider[mEWSCSize].entityID){
                mEntitiesWStatCollider[mEWSCSize].mTransform.position = mEntities[mESize].mTransform.position;
            }

    }
}
}
    //Then the Collider is moved for each vector
    //All entitites
    if(!mEntities.empty()){
    mEntities[EntityNr].mCollider.mCenter = mEntities[EntityNr].mTransform.position;
    }
    //Entities with a collider
    if(!mEntitiesWCollider.empty()){
    mEntitiesWCollider[EntityNr].mCollider.mCenter = mEntitiesWCollider[EntityNr].mTransform.position;
    }
    //Entitites with a dynamic collider
    if(!mEntitiesWDynCollider.empty()){
    mEntitiesWDynCollider[EntityNr].mCollider.mCenter = mEntitiesWDynCollider[EntityNr].mTransform.position;
    }
    //Entities with a static collider
    if(!mEntitiesWStatCollider.empty()){
    mEntitiesWStatCollider[EntityNr].mCollider.mCenter = mEntitiesWStatCollider[EntityNr].mTransform.position;
    }

}




