#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <QOpenGLFunctions_4_1_Core>
#include "components.h"

/// Takes care of rendering for any entity with a mesh
///
/// This class contains the functions which renders the normal scene aswell as the mousepicker shaded scene.
class renderSystem : public QOpenGLFunctions_4_1_Core
{
public:
    renderSystem();
    void Render(class ResourceManager *currentManager);
    void renderMousePicker(GLint colorUniform, ResourceManager *currentManager);

    void init();
    /// Keeps track of how many meshes have been assigned to an entity.
    unsigned int mNumberOfMeshes{0};
};

#endif // RENDERSYSTEM_H
