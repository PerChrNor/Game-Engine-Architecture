#version 410 core
layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec4 colAttr;
layout(location = 2) in vec2 vertexUV;

out vec3 UV;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main() {
    vec3 fixUVs = posAttr;  //vec3(mMatrix * vec4(posAttr, 1.0));

    fixUVs.y = -fixUVs.y;   //This should not be necessary!?
    fixUVs.x = -fixUVs.x;   //This should not be necessary!?

    mat3 viewWithoutTranslation = mat3(vMatrix);
    mat4 viewFinished = mat4(viewWithoutTranslation);

    UV = fixUVs;
    vec4 pos = pMatrix * viewFinished * mMatrix * vec4(posAttr, 1.0);
//    gl_Position = pos.xyww;  //https://youtu.be/QYvi1akO_Po?t=475 - draw only pixels that are not filled yet - not a clean result on Mac
    gl_Position = pos;

}
