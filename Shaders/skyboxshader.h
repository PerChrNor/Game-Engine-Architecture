#ifndef SKYBOXSHADER_H
#define SKYBOXSHADER_H

#include "Shaders/shader.h"
#include "Math/matrix4x4.h"

class SkyboxShader : public Shader
{
public:
    SkyboxShader(const std::string &shaderName);

    void setupShader() override;
    virtual void use(QMatrix4x4 &modelMatrix, struct materialComponent *materialIn);

    GLint mTextureUniform{-1};
};

#endif // SKYBOXSHADER_H
