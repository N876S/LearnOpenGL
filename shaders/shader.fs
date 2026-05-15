#version 460 core
out vec4 fragColor;
in vec3 colorPos;

uniform float colorAdj;

void main(){
    fragColor = vec4(colorPos.x*(1-colorAdj/2), colorPos.y*(1-colorAdj/2), colorPos.z*(1-colorAdj/2), 1.0f);
}
