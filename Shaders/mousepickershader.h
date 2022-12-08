#ifndef MOUSEPICKERSHADER_H
#define MOUSEPICKERSHADER_H

#include "shader.h"
#include <QMatrix4x4>
#include "camera.h"
///
/// written by candidate: 8003
///
///
/// \brief The MousePickerShader class.
///
class MousePickerShader : public Shader
{
public:
    ///
    /// \brief MousePickerShader, shader constructer.
    /// \param shaderName, takes in shadername string.
    ///
    MousePickerShader(const std::string &shaderName);
    ///
    /// \brief setupShader, gets uniform locations for model, veiw, and projection matrix.
    /// set colorpicking uniform
    ///
    void setupShader() override;
    ///
    /// \brief use, sets glProgram to mProgram, and set view and projection matrix to that of current camera.
    /// \param modelMatrixIn, a matrix4x4.
    ///
    void use(QMatrix4x4 &modelMatrixIn, struct materialComponent * = nullptr) override;

    GLint mPickerColor{-1};

};

#endif // MOUSEPICKERSHADER_H
