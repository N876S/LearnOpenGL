#version 460 core
out vec4 fragColor;
in vec3 colorPos;
in vec2 texCoord;

uniform float colorAdj;

uniform sampler2D textureData;

void main(){
    fragColor = texture(textureData, texCoord) * vec4(colorPos.x*(1-colorAdj/2), colorPos.y*(1-colorAdj/2), colorPos.z*(1-colorAdj/2), 1.0f);
}
