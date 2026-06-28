#version 460 core
out vec4 fragColor;
in vec2 texCoord;
in vec3 normal;
in vec3 worldFragPos;

uniform sampler2D textureData;
uniform vec3 lightPos;

void main(){
    float ambientStrength = 0.1f;

    vec3 fixedNormals = normalize(normal);
    vec3 fixedLightDir = normalize(lightPos - worldFragPos);
    float diffuse = max(dot(fixedLightDir, fixedNormals), 0.0f);

    fragColor = (ambientStrength + diffuse) * texture(textureData, texCoord);
}
