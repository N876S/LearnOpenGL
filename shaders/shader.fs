#version 460 core
out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D textureData;

void main(){
    fragColor = texture(textureData, texCoord);
}
