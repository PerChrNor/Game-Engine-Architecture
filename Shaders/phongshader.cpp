#include "phongshader.h"

#include "camera.h"
#include "light.h"
#include "texture.h"

PhongShader::PhongShader(const std::string &shaderName) : Shader(shaderName)
{

}

void PhongShader::use(QMatrix4x4 &modelMatrix, struct materialComponent *material)
{
    glUseProgram(mProgram);
    glUniformMatrix4fv(pMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ProjectionMatrix.constData());
    glUniformMatrix4fv(vMatrixUniform, 1, GL_TRUE, CurrentCamera->m_ViewMatrix.constData());

    glUniform3f(mLightPosUniform, mCurrentLight->mTransformationMatrix.column(3).x(), mCurrentLight->mTransformationMatrix.column(3).y(),
                mCurrentLight->mTransformationMatrix.column(3).z());
    glUniform3f(mCameraPosUniform, CurrentCamera->m_ViewMatrix.matrix[3], CurrentCamera->m_ViewMatrix.matrix[7], CurrentCamera->m_ViewMatrix.matrix[11]);
    glUniform3f(mLightColorUniform, mCurrentLight->lightColor.x(), mCurrentLight->lightColor.y(), mCurrentLight->lightColor.z());
    glUniform1i(mTextureUniform, material->mTextureId-1);

    glUniform1f(mSpecularStrengthUniform, material->mSpecularStrength);
    glUniform1i(mSpecularExponentUniform, material->mSpecularExponent);
    //glUniform1f(mAmbientLightStrengthUniform, mCurrentLight->mAmbientStrenght);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, modelMatrix.constData());
}

void PhongShader::setupShader()
{
    mMatrixUniform = glGetUniformLocation(mProgram, "mMatrix");
    vMatrixUniform = glGetUniformLocation(mProgram, "vMatrix" );
    pMatrixUniform = glGetUniformLocation(mProgram, "pMatrix" );

    mLightColorUniform = glGetUniformLocation(mProgram, "lightColor");
    //mObjectClorUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "objectColor");
    mAmbientLightStrengthUniform = glGetUniformLocation(mProgram, "ambientStrength");
    mLightPosUniform = glGetUniformLocation(mProgram, "lightPos");
    mSpecularStrengthUniform = glGetUniformLocation(mProgram, "specularStrength");
    mSpecularExponentUniform = glGetUniformLocation(mProgram, "specularExponent");
    mLightPowerUniform =glGetUniformLocation(mProgram, "lightStrength");
    mCameraPosUniform = glGetUniformLocation(mProgram, "cameraPos");
    mTextureUniform = glGetUniformLocation(mProgram, "textureSampler");
}
