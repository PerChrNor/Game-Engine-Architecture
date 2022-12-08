#include "plainshader.h"
#include "camera.h"

PlainShader::PlainShader(const std::string &shaderName) : Shader(shaderName)
{

}

void PlainShader::setupShader()
{
    mMatrixUniform = glGetUniformLocation(mProgram, "mMatrix");
    vMatrixUniform = glGetUniformLocation(mProgram, "vMatrix");
    pMatrixUniform = glGetUniformLocation(mProgram, "pMatrix");
}

void PlainShader::use(QMatrix4x4 &modelMatrix, struct materialComponent *)
{
    glUseProgram(mProgram);
    glUniformMatrix4fv(vMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ViewMatrix.constData());
    glUniformMatrix4fv(pMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ProjectionMatrix.constData());
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, modelMatrix.constData());
}
