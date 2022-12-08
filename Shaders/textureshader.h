#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "shader.h"
#include <QMatrix4x4>

class TextureShader : public Shader
{
public:
    TextureShader(const std::string &shaderName);

    void setupShader() override;
    void use(QMatrix4x4 &modelMatrix, struct materialComponent *material) override;

    GLint mTextureUniform{1};
};

#endif // TEXTURESHADER_H
