#version 460 core

out vec4 fragColor;

in vec3 normal;
in vec3 worldFragPos;
in vec3 cameraPosition;

uniform vec3 lightPos;
uniform vec3 color;

void main(){
    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    int specularShininess = 32;

    vec3 fixedNormals = normalize(normal);
    vec3 fixedLightDir = normalize(lightPos - worldFragPos);
    float diffuse = max(dot(fixedLightDir, fixedNormals), 0.0f);

    vec3 reflection = reflect(-fixedLightDir, fixedNormals);
    vec3 cameraDirection = normalize(cameraPosition - worldFragPos);
    float specular = pow(max(dot(reflection, cameraDirection), 0.0f), 32);

    fragColor = (ambientStrength + diffuse + specular) * vec4(color, 1.0f);
}
