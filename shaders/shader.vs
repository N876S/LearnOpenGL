#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 colorPos;
out vec2 texCoord;

uniform mat4 trans;

void main(){
    colorPos = aColor;
    texCoord = aTexCoord;
    gl_Position = trans * vec4(aPos, 1.0f);
}