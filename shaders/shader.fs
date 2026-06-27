#version 460 core
out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D textureData;

float ambientStrength = 0.1f;

void main(){
    fragColor = ambientStrength * texture(textureData, texCoord);
}
