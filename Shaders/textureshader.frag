#version 410 core

in vec2 UV;
uniform sampler2D textureSampler;
out vec3 fragColor;

void main()
{
    fragColor = texture(textureSampler, UV).rgb;
}
