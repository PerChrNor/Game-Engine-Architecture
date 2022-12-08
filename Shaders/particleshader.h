#ifndef PARTICLESHADER_H
#define PARTICLESHADER_H

#include "shader.h"
#include <QMatrix4x4>
///
/// written by candidate: 8003
///
struct ParticleEmitter;
///
/// \brief The ParticleShader class.
///
class ParticleShader : public Shader
{
public:
    ///
    /// \brief ParticleShader, shader constructer.
    /// \param shaderName, takes in shadername string.
    ///
    ParticleShader(const std::string &shaderName);
    ///
    /// \brief setupShader, gets uniform locations for model, veiw, and projection matrix.
    ///
    void setupShader() override;
    ///
    /// \brief use, sets glProgram to mProgram, and set view and projection matrix to that of current camera.
    /// \param modelMatrix, a matrix4x4.
    ///
    void use(QMatrix4x4 &modelMatrix, struct materialComponent * = nullptr) override;

    GLint mModelViewUniform{-1};

};


#endif // PARTICLESHADER_H
