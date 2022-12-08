#version 410 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

out vec3 fragmentPos;
out vec3 normalTransposed;
out vec2 UV;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main()
{
    fragmentPos = vec3(mMatrix * vec4(vertexPos, 1.0));
    normalTransposed = mat3(transpose(inverse(mMatrix))) * vertexNormal;

    UV = vertexUV;
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(vertexPos, 1.0);
}
