#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
//merge conflict
#include <QOpenGLFunctions_4_1_Core>

#include <map>
#include <vector>
#include <string>
#include "gltypes.h"
#include "texture.h"
#include "constants.h"
#include "components.h"
#include "vertex.h"
#include "mainwindow.h"
#include "entity.h"
#include "rendersystem.h"
#include "camera.h"

#ifdef _WIN32
#include "AL/al.h"
#include "AL/alc.h"
#endif
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include "Math/vector3d.h"
#include <qdebug.h>
#include "wavfilehandler.h"

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    ResourceManager();
    void operator=(ResourceManager&);           //Assignment operator
    ResourceManager(const ResourceManager&);    //Copy constructor

    static ResourceManager& getInstance();

    //new system:
    std::vector <class objMesh*> mObjMesh;   // holds all different mesh objects - reused when needed
    std::map<std::string, unsigned int> mMeshObjectMap;      // name / index to mMeshObjects

    std::map<std::string, unsigned short> mMaterialNames;  //name / MaterialID
    std::map<std::string, int> mTextureNames;   //name, TextureID
    std::map<unsigned short, char> mShaderProgramIndex;     //shader program, index into mShaderProgram

    std::vector <class Cube*> mObjCube;   // holds all different mesh objects - reused when needed
    std::map<std::string, unsigned int> mMeshCubeMap;      // name, index to mMeshObjects

    int mTextureSlots[32];      //what textureID in what slot

    ///
    /// \brief readTextures, check if texture is regualr or cubemap.
    ///
    void readTextures();
    void readObjs(std::string fileName);
    void readShaders(class RenderWindow* renderWindow);
    void makeMaterials();

    class Texture *mTexture[NumberOfTextures]{nullptr};
    class Shader *mShaderProgram[NumberOfShaders]{nullptr};
    struct materialComponent *mMaterial[NumberOfMaterials]{nullptr};
    class Scene *mCurrentScene{nullptr};

    MAX_ENTITIES_TYPE makeEntity(std::string entityName, std::string entityTag = "");
    void attachMesh(const std::string &mesh, int entity, renderSystem *mRenderer);
    void attachTransform(const transformComponent &transform, int entity);
    void attachMaterial(const int materialIndex, int entity);

    int mIndexOfLastTexture{-1};

    class MainWindow *mMainWindow;
    /// The vector each entity is put into so that each individual entity can be accessed easily.
    std::vector<Entity> mEntities;

public:
    ///The Sound listner is vector information relevant to the object picking up the sound in the scene.
    ///
    ///The listener needs a different vectors for different effects, they are:
    ///A position, it is where the sounds comes from in the scene.
    ///A velocity value, this is used to create the doppler effect if the lisener is moving.
    ///A direction, this is to be able hear where the sound is coming from,
    ///an example would be if someone is wearing a headset and the sound is located on the right of the listener the sound would come from the right headphone output.
    ///An up vector, this is to control which direction is up for the listener.
    ///Sound was created by 8002.
    struct SoundListener
    {
        Vector3D pos;
        Vector3D vel;
        Vector3D dir;
        Vector3D up;
        SoundListener()
        {
            pos = Vector3D(0.0f, 0.0f, 0.0f);
            vel = Vector3D(0.0f, 0.0f, 0.0f);
            dir = Vector3D(0.0f, 0.0f, 1.0f);
            up = Vector3D(0.0f, 1.0f, 0.0f);

        }
    };

    ///This function sets up the listener using OPENAL.
    ///Sound was created by 8002.
    bool soundInit();

    ///This function destroys any remnants of the sound listener.
    ///Sound was created by 8002
    void cleanUp();

    /// The updateListener function updates any values for the current listener based on the inputs.
    /// Sound was created by 8002.
    /// @param pos = the Position of the listener.
    /// @param vel = the Veloocity of the litener.
    /// @param dir = the Direction of the listener.
    /// @param up = the listeners UP-vector.
    void updateListener(Vector3D pos, Vector3D vel, Vector3D dir, Vector3D up);

    /// The attachSound function will save the information.
    /// Sound was created by 8002.
    /// @param name = gives a name to the sound to make it easier to keep the sounds in.
    /// @param EntityNr = the Entity which will host the sound.
    /// @param filePath = the file path of the sound.
    /// @param loop = a bool that will decide if the sound clip will loop.
    /// @param gain = is the sounds volume.
    void attachSound(std::string name,short EntityNr ,std::string filePath = "", bool loop = false, float gain = 1.0);

    /// \brief loadWave loads a WAV fila and saves it into a buffer
    /// \param filePath is where the sound is on the computer
    /// \param source is the source's nummber
    /// \param buffer is the buffer that the sound is put into
    /// \return a boot that returns true if the load was sucessful
    bool loadWave(std::string filePath, ALuint source, ALuint buffer);


    ///  the checkWavError function writes out an error using Qt's qDebug().
    /// Sound was created by 8002.
    /// \param name = the name of the file that there is an error with
    /// \return a bool to see if there has been an error
    bool checkWavError(std::string name);

    ///  the function playSound uses an OPENAL function to play the sound of the Entity selected.
    /// Sound was created by 8002.
    /// \param EntityNr this is the index number of the mEntities
    ///@see pauseSound()
    ///@see stopSound()
    void playSound(short EntityNr);

    ///  the function pauseSound uses an OPENAL function to pause the sound of the Entity selected.
    /// Sound was created by 8002.
    /// \param EntityNr this is the index number of the mEntities
    void pauseSound(short EntityNr);


    ///  the function stopSound uses an OPENAL function to stop the sound of the Entity selected
    /// Sound was created by 8002.
    /// \param EntityNr this is the index number of the mEntities
    void stopSound(short EntityNr);


    /// starts all the sound in the scene, used when pressing play game
    /// Sound was created by 8002.
    void startAllSound();

    /// stops all the sound in the scene, used when pressing stop game
    /// Sound was created by 8002.
    void stopAllSound();

    /// updateSoundPos updates the position of a sound source.
    /// The source's position is alway equal to the entity it is attached to, this function updates the position if the entitiy has moved
    /// Sound was created by 8002.
    /// \param EntityNr this is the entitiy it is updating the posision for
    void updateSoundPos(short EntityNr);

    ///attachSphereCollider fills out the information relevant to creating a sphere collider.
    ///Here the shape of the collider, the size, the position and the collision type is set.
    ///the unique information set here is the radius for the Sphere collider.
    /// Collisions was created by 8002.
    /// \param EntityNr the index number of the Entity that the collider will be attached to
    /// \param objectType this specifies what kind of collider it will be, either a static or dynamic collider
    /// \param radius this tells the size of the collider
    void attachSphereCollider(short EntityNr, std::string objectType, float radius = 0);

    /// attachBoxCollider fills out the information relevant to creating a Box collider.
    /// Here the shape of the collider, the size, the position and the collision type is set.
    /// the unique information set in this function is the Min and Max points for the Box collider.
    /// Collisions was created by 8002.
    /// \param EntityNr the index number of the Entity that the collider will be attached to
    /// \param objectType this specifies what kind of collider it will be, either a static or dynamic collider
    /// \param maxPoint This is the Maximums point required for the box collider
    /// \param minPoint This is the Minimums point required for the box collider
    void attachBoxCollider(short EntityNr, std::string objectType, Vector3D maxPoint = Vector3D(0,0,0), Vector3D minPoint = Vector3D(0,0,0));


    ///This function saves all the entities with a collider into a new vector.
    ///The reason for this is  because that way the collision system does not have to check the collision of any entity without a collider.
    /// Collisions was created by 8002.
    void CheckEntitiesWithCollider();

    ///This function seperates the Entity-With-A-Collider into two separate vectors.
    ///This is done because it becomes faster to check collisions since the collision checker does not have to check collisions between two static objects,
    ///only between static and dynamic or dynamic and dynamic colliders.
    /// Collisions was created by 8002.
    void CheckEntitiesWithStaticDynamicCollider();

    /// This function will check the collision of all object then save what each entity has collided with in a personal vector for each object.
    /// The function will first clear any collisions what happened the previous frame.
    /// Then the collision is checked, the function might look slow due to the double for-loop, but the function is faster due to the earlier vector split in CheckEntitiesWithCollider()
    ///  and CheckEntitiesWithStaticDynamicCollider().
    /// A linear search is done to the personal collision vector each object has, is uses the Entities index as its ID.
    /// If the search finds the object it does not try to add it multiple times.
    /// If collisions happen the object needs know what kind of colliders each object has to use the correct collision method
    /// The collision methods available are: Sphere to Sphere, Box to Box, Sphere to Box and Box to Sphere.
    /// Each entity has a vector where is saves the ID of which entitiy its colliding with.
    /// Then each entitiy that collides will be saved in their respective collision vectors.
    /// Collisions was created by 8002.
    /// @see CheckEntitiesWithCollider().
    /// @see CheckEntitiesWithStaticDynamicCollider().
    void checkCollisions();


    ///  mEntitiesWCollider A Vector where all entities with a collider is saved
    std::vector<Entity> mEntitiesWCollider;

    ///  mEntitiesWDynCollider A Vector where all entities with a Dynamic collider is saved
    std::vector<Entity> mEntitiesWDynCollider;

    ///  mEntitiesWStatCollider A Vector where all entities with a Static collider is saved
    std::vector<Entity> mEntitiesWStatCollider;

    ///  updateColliderPos updates the collidest position based on its transformation.
    /// \param EntityNR
    void updateColliderPos(short EntityNR);



private:



    //mesh
    std::map<std::string, int> mMeshKey;
    std::vector<meshComponent> mMeshComponents;
    std::map<int, int> mMeshMap;

    //transform
    std::map<int, transformComponent> mTransformMap;

    int getMeshComponentIndex(std::string mesh, int entity);
    meshComponent newMeshComponent(std::string mesh);

    // Variables for file reading
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;


    /// \brief checkError prints out any error that happenes when reading in a sound file.
    /// \return a bool that tells if there has been an error.
    bool checkError();

    ALCdevice* mDevice;
    ALCcontext* mContext;


    /// \brief mListener is the used listener.
    SoundListener mListener;

    std::vector <Vertex> tempVertecies;
    std::vector <unsigned int> tempIndicies;

};

#endif // RESOURCEMANAGER_H
