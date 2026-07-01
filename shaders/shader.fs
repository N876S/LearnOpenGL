#version 460 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 worldFragPos;
in vec3 cameraPosition;

uniform sampler2D textureData;
uniform vec3 lightPos;
uniform vec3 lightColor;

/*
FIX HERE
1. INPUT LIGHT COLOR AND ORGANIZE IN LIGHT CLASS
2. ADD THE MATERIAL PROPERTIES FROM THE MODEL CLASS (and material.h)
3. ADD TO GROUND SHADER TOO
4. REMEMBER TO SEND FROM CPP PROGRAM SIDE

*/

void main(){
    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    int specularShininess = 32;

    vec3 fixedNormals = normalize(normal);
    vec3 fixedLightDir = normalize(lightPos - worldFragPos);
    float diffFactor = max(dot(fixedLightDir, fixedNormals), 0.0f);
    vec3 diffuse = light

    vec3 reflection = reflect(-fixedLightDir, fixedNormals);
    vec3 cameraDirection = normalize(cameraPosition - worldFragPos);
    float specular = pow(max(dot(reflection, cameraDirection), 0.0f), 32);

    fragColor = (ambientStrength + diffuse + specular) * texture(textureData, texCoord);
}
