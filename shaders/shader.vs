#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
out vec2 texCoord;
out vec3 normal;
out vec3 worldFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
    texCoord = aTexCoord;

    mat3 normalMatrix = inverse(transpose(mat3(model)));
    normal = normalMatrix * aNormal;

    worldFragPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
}