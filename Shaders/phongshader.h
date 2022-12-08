#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "shader.h"
#include <QMatrix4x4>

class PhongShader : public Shader
{
public:
    PhongShader(const std::string &shaderName);

    void setupShader() override;
    void use(QMatrix4x4 &modelMatrix, struct materialComponent *material) override;

    class light *mCurrentLight{nullptr};

    GLint mLightColorUniform{-1};
    //GLint mObjectClorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mLightPosUniform{-1};
    GLint mCameraPosUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};
    GLint mTextureUniform{-1};
};

#endif // PHONGSHADER_H
