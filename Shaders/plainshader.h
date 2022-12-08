#ifndef PLAINSHADER_H
#define PLAINSHADER_H

#include "shader.h"
#include <QMatrix4x4>

class PlainShader : public Shader
{
public:
    PlainShader(const std::string &shaderName);

    void setupShader() override;
    void use(QMatrix4x4 &modelMatrix, struct materialComponent * = nullptr) override;
};

#endif // PLAINSHADER_H
