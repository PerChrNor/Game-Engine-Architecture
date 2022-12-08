#include "mousepickershader.h"
#include "camera.h"
#include "renderwindow.h"

MousePickerShader::MousePickerShader(const std::string &shaderName)  : Shader(shaderName)
{ }

void MousePickerShader::setupShader()
{
    mMatrixUniform = glGetUniformLocation( mProgram, "mMatrix" );
    vMatrixUniform = glGetUniformLocation( mProgram, "vMatrix" );
    pMatrixUniform = glGetUniformLocation( mProgram, "pMatrix" );
    //using this for the VP calculated matrix!
    mPickerColor = glGetUniformLocation(mProgram, "pickingColor");
}

void MousePickerShader::use(QMatrix4x4 &modelMatrixIn, materialComponent*)
{
    glUseProgram(mProgram);
    //Matrix4x4 temp = CurrentCamera->m_ProjectionMatrix * CurrentCamera->m_ViewMatrix;
    //temp.setToIdentity();
    glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ProjectionMatrix.constData());
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, modelMatrixIn.constData());
}
