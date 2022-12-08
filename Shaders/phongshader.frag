#version 410 core

out vec4 fragColor;

in vec3 normalTransposed;
in vec3 fragmentPos;
in vec2 UV;

uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform float ambientStrength = 0.2;
uniform float lightStrength = 0.5;
uniform vec3 lightColor = vec3(0.8, 0.8, 0.3);
uniform float specularStrength = 0.5;
uniform int specularExponent = 64;

uniform vec3 objectColor = vec3(0.7,0.7,0.7);

uniform sampler2D textureSampler;

void main()
{
    // ambient light
    vec3 ambient = ambientStrength * lightColor;

    // diffuse light
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 lightDirection = normalize(lightPos - fragmentPos);
    float angleFactor = max(dot(normalCorrected, lightDirection), 0.0);
    vec3 diffuse = angleFactor * vec3(texture2D(textureSampler, UV)) * lightColor * lightStrength;

    // specular light
    vec3 viewDirection = normalize(cameraPos - fragmentPos);
    float spec = 0.0;
    if (angleFactor > 0.0)
    {
        vec3 reflectDirection = reflect(-lightDirection, normalCorrected);
        spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
    }
    vec3 specular = spec * lightColor * specularStrength;

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}
