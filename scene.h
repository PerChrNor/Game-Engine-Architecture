#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "gltypes.h"
#include "components.h"
#include "resourcemanager.h"
///
///code by candidate: 8003
///
class RenderWindow;

class Scene
{
public:
    ///
    /// \brief Scene. A constructor that takes in renderwindow class.
    /// \param renderWindow
    ///
    Scene(RenderWindow *renderWindow);
    ///
    /// \brief render. calls the resorucemanagers renderfucntion to scene.
    /// \param currentManager takes in resourcemanager.
    ///
    void render(ResourceManager *currentManager);
    ///
    /// \brief renderNormals. goes through visualobject and draws the normals.
    /// \param matrixUniform, takes in matrixuniform.
    ///
    void renderNormals(GLint matrixUniform);
    ///
    /// \brief renderMousePicker, calls renderMousePicker from rendersystem to scene.
    /// \param colorUniform, takes in colorUniform.
    /// \param currentmanager, takes in resourcemanager.
    ///
    void renderMousePicker(GLint colorUniform, ResourceManager *currentmanager);

    std::vector<class VisualObject*> mVisualObjects;
    RenderWindow *mRenderWindow{nullptr};
    class Light *mLight{nullptr};
    class Terrain *mTerrain{nullptr};
    std::string mName{"no name"};

    class renderSystem *mRenderSystem{nullptr};


};

#endif // SCENE_H
