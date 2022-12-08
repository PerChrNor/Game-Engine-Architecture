#include "rendersystem.h"
#include "Shaders/shader.h"
#include <QMatrix4x4>
#include "resourcemanager.h"

renderSystem::renderSystem()
{

}

void renderSystem::init()
{
    initializeOpenGLFunctions();
}

/// Renders the scene which is seen by the player.
///
/// This function goes through all the entites which have a mesh and renders them for the player to see.
/// Requires that entities which have a mesh are created before any that don't have a mesh.
/// Written by candidate 8008
/// @param currentManager provides access to the ResourceManager where the entities are made.
void renderSystem::Render(ResourceManager *currentManager)
{
    glClearColor(0.4f, 0.4f, 0.4f,1.0f);

    glUseProgram(ResourceManager::getInstance().mShaderProgram[PLAINSHADER]->mProgram);
    for (unsigned int i = 0; i < mNumberOfMeshes; ++i)
    {
        QMatrix4x4 tempTransform;
        tempTransform.setToIdentity();
        tempTransform.translate(currentManager->mEntities[i].mTransform.position);
        tempTransform.rotate(currentManager->mEntities[i].mTransform.rotation.x(), 1,0,0);
        tempTransform.rotate(currentManager->mEntities[i].mTransform.rotation.y(), 0,1,0);
        tempTransform.rotate(currentManager->mEntities[i].mTransform.rotation.z(), 0,0,1);
        tempTransform.scale(currentManager->mEntities[i].mTransform.scale);
        glBindVertexArray(currentManager->mEntities[i].mMesh.mVAO);

        materialComponent *tempMaterial = currentManager->mMaterial[currentManager->mEntities[i].mMaterialID.mMaterial];
        currentManager->mShaderProgram[tempMaterial->mShader]->use(tempTransform, tempMaterial);

        if (currentManager->mEntities[i].mMesh.Indexcount > 0)
        {
            if (currentManager->mEntities[i].mMesh.isInFrustum == true)
                glDrawElements(GL_TRIANGLES, currentManager->mEntities[i].mMesh.Indexcount, GL_UNSIGNED_INT, nullptr);
        }
        //else
            //glDrawArrays(GL_TRIANGLES, 0, mMeshes[i].Vertexcount);
        glBindVertexArray(0);
    }
}

/// Renders the scene that is used to select entities with mouse picking.
///
/// This function renders all entities with a mesh in shades of red for the mouse picking.
/// The ID that the color is caluclated from is saved into the mousePickerID variable to be used when checking what entity the mouse is clicking on.
/// Written by candidate 8008 and 8003
/// @param colorUniform Provides a uniform variable to send to the shader when sending the color.
/// @param currentManager provides access to the ResourceManager where the entities are made.
/// @see RenderWindow::mousePicking()
void renderSystem::renderMousePicker(GLint colorUniform, ResourceManager *currentManager)
{
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(ResourceManager::getInstance().mShaderProgram[MOUSEPICKERSHADER]->mProgram);

    //glEnableVertexAttribArray(0);

    for (unsigned int i = 0; i < mNumberOfMeshes; ++i)
    {
        int j = (i);
        int r = (j & 0x000000FF) >>  0;
        int g = (j & 0x0000FF00) >>  8;
        int b = (j & 0x00FF0000) >> 16;

        float tempR = r; float tempG = g; float tempB = b;

        currentManager->mEntities[i].mMaterialID.mousePickerID = j;
        QVector3D color(tempR/255, tempG/255, tempB/255);

        QMatrix4x4 tempTransform;
        tempTransform.setToIdentity();
        tempTransform.translate(currentManager->mEntities[i].mTransform.position);
        tempTransform.rotate(currentManager->mEntities[i].mTransform.rotation.x(), 1,0,0);
        tempTransform.rotate(currentManager->mEntities[i].mTransform.rotation.y(), 0,1,0);
        tempTransform.rotate(currentManager->mEntities[i].mTransform.rotation.z(), 0,0,1);
        tempTransform.scale(currentManager->mEntities[i].mTransform.scale);
        glBindVertexArray(currentManager->mEntities[i].mMesh.mVAO);

        //currentManager->mMaterial[4]->mColor = color;
        glUniform4f(colorUniform, color.x(), color.y(), color.z(), 1.0f);
        materialComponent *tempMaterial = currentManager->mMaterial[4];
        currentManager->mShaderProgram[tempMaterial->mShader]->use(tempTransform, tempMaterial);

       // glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, tempTransform.constData());


        if (currentManager->mEntities[i].mMesh.Indexcount > 0)
            glDrawElements(GL_TRIANGLES, currentManager->mEntities[i].mMesh.Indexcount, GL_UNSIGNED_INT, nullptr);
        //else
          //  glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
        glBindVertexArray(0);
    }
}
