#include "scene.h"
#include "visualobject.h"
#include "light.h"
#include "objmesh.h"
#include "light.h"
#include "constants.h"
#include "renderwindow.h"
#include "resourcemanager.h"
#include "entity.h"
#include "rendersystem.h"

Scene::Scene(RenderWindow *renderWindow) : mRenderWindow{renderWindow}
{

}

void Scene::render(ResourceManager *currentManager)
{
    mRenderSystem->Render(currentManager);
    mRenderWindow->checkForGLerrors();
}

void Scene::renderNormals(GLint matrixUniform)
{
    for (auto asset : mVisualObjects)
    {
        asset->drawNormals(matrixUniform);
    }
}

void Scene::renderMousePicker(GLint colorUniform, ResourceManager *currentmanager)
{
        mRenderSystem->renderMousePicker(colorUniform, currentmanager);
        mRenderWindow->checkForGLerrors();
}
