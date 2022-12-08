#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_4_1_Core>
#include <string>
#include "components.h"
#include <QMatrix4x4>
#include "camera.h"


class Shader : protected QOpenGLFunctions_4_1_Core
{
public:
    Shader(const std::string &shaderName);

    virtual void use(QMatrix4x4 &modelMatrix, struct materialComponent *);
    virtual void setupShader(){}

    const std::string mName;

    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};

    class Camera *CurrentCamera;
    class RenderWindow *mRenderWindow{nullptr};

    GLuint mProgram{999999};
};

#endif // SHADER_H
