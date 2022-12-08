#version 410 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main()
{
    gl_Position =  pMatrix * vMatrix * mMatrix * vec4(vertexPosition,1);
}
