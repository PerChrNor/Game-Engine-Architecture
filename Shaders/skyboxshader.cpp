#include "skyboxshader.h"

#include "components.h"
#include "camera.h"
#include "texture.h"
#include "resourcemanager.h"

SkyboxShader::SkyboxShader(const std::string &shaderName) : Shader(shaderName)
{ }

void SkyboxShader::setupShader()
{
    mMatrixUniform = glGetUniformLocation( mProgram, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( mProgram, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( mProgram, "pMatrix" );
    mTextureUniform = glGetUniformLocation( mProgram, "skybox");
}

void SkyboxShader::use(QMatrix4x4 &modelMatrix, struct materialComponent *materialIn)
{
    glUseProgram(mProgram);
    glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ProjectionMatrix.constData());

    glUniform1i(mTextureUniform, materialIn->mTextureId-1);  //should use texture slot!!!
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrix.constData());
}
