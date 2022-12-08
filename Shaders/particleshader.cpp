#include "particleshader.h"
#include "camera.h"

ParticleShader::ParticleShader(const std::string &shaderName) : Shader(shaderName)
{

}

void ParticleShader::setupShader()
{
    mMatrixUniform = glGetUniformLocation(mProgram, "mMatrix");
    vMatrixUniform = glGetUniformLocation(mProgram, "vMatrix");
    pMatrixUniform = glGetUniformLocation(mProgram, "pMatrix");
    mModelViewUniform = glGetUniformLocation(mProgram, "viewModel");
}

void ParticleShader::use(QMatrix4x4 &modelMatrix, struct materialComponent *)
{
    glUseProgram(mProgram);
    glUniformMatrix4fv(vMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ViewMatrix.constData());
    glUniformMatrix4fv(pMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ProjectionMatrix.constData());
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, modelMatrix.constData());
}
