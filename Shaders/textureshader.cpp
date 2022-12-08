#include "textureshader.h"

#include "camera.h"
#include "texture.h"

TextureShader::TextureShader(const std::string &shaderName) : Shader(shaderName)
{

}

void TextureShader::setupShader()
{
    mMatrixUniform = glGetUniformLocation(mProgram, "mMatrix");
    vMatrixUniform = glGetUniformLocation(mProgram, "vMatrix");
    pMatrixUniform = glGetUniformLocation(mProgram, "pMatrix");
    mTextureUniform = glGetUniformLocation(mProgram, "textureSampler");
}

void TextureShader::use(QMatrix4x4 &modelMatrix, struct materialComponent *material)
{
    glUseProgram(mProgram);
    glUniformMatrix4fv(vMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ViewMatrix.constData());
    glUniformMatrix4fv(pMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ProjectionMatrix.constData());

    glUniform1i(mTextureUniform, material->mTextureId-1);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, modelMatrix.constData());
}
