#version 460 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D diffuse_texture;

void main(){
    vec4 pixel = texture(diffuse_texture, texCoord);
    if(pixel.w < 0.1f){
        discard;
    } else {
        fragColor = pixel;
    }
}
