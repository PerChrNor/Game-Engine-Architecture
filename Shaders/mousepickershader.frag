#version 410 core

out vec4 color;

uniform vec4 pickingColor;

void main(){
    color = pickingColor;
}
