#version 410 core

in vec3 UV;
uniform samplerCube skybox;
out vec4 fragColor;

void main() {
    fragColor = texture(skybox, UV);
}
