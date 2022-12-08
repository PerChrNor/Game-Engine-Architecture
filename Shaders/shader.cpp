#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader(const std::string &shaderName) : mName{shaderName}
{
    initializeOpenGLFunctions();

    qDebug() << "Reading" << shaderName.c_str() << "shader:";

    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    std::string vertexPath = (ShaderFilePath + shaderName.c_str() + ".vert");
    std::string fragmentPath = (ShaderFilePath + shaderName.c_str() + ".frag");

    vShaderFile.open(vertexPath.c_str());
    if(!vShaderFile)
        qDebug() << "Error Vertex file " << shaderName.c_str() << " not read";
    fShaderFile.open(fragmentPath.c_str());
    if(!fShaderFile)
        qDebug() << "Error fragment file " << shaderName.c_str() << " not read";

    std::stringstream vShaderStream;
    std::stringstream fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();

    GLuint vertex;
    GLuint fragment;
    GLint success;
    GLchar log[512];

    vertex = glCreateShader( GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, log);
        qDebug() << "Error vertex " << vertexPath.c_str() << " compilation failed\n";
    }
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, log);
        qDebug() << "Error fragment " << fragmentPath.c_str() << " compilation failed\n";
    }

    this->mProgram = glCreateProgram();
    glAttachShader(this->mProgram, vertex);
    glAttachShader(this->mProgram, fragment);
    glLinkProgram(this->mProgram);
    glGetProgramiv(this->mProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->mProgram, 512, nullptr, log);
        qDebug() << "Error shader linking failed " << shaderName.c_str() << "\n";
    }
    else
    {
        qDebug() << shaderName.c_str() << "was successfully compiled" << "id:" << mProgram;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use(QMatrix4x4 &modelMatrix, struct materialComponent *)
{
    glUseProgram(this->mProgram);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, modelMatrix.constData());
}
