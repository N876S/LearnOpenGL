#version 460 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 worldFragPos; //position of fragment in world
in vec3 cameraPosition;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 color;
    float ambient;
    float diffuse;
    float specular;
};

uniform Material material;
uniform DirLight dirLight;

uniform sampler2D textureData;

vec3 getDirLightFactor(DirLight light, vec3 normal, vec3 cameraDirection);

void main(){
    //important vector calculations (global to pass into functions)
    vec3 fixedNormals = normalize(normal);
    vec3 cameraDirection = normalize(cameraPosition - worldFragPos);

    vec3 outputLightColor = vec3(0.0f);

    outputLightColor += getDirLightFactor(dirLight, fixedNormals, cameraDirection);
    
    fragColor = vec4(outputLightColor, 1.0f) * texture(textureData, texCoord);
}

//calculate direction light
vec3 getDirLightFactor(DirLight light, vec3 normal, vec3 cameraDirection){
    vec3 fixedLightDir = normalize(light.pos - worldFragPos);
    vec3 reflection = reflect(-fixedLightDir, normal);

    //ambient calculation
    vec3 ambient = light.ambient * light.color * vec3(texture(material.diffuse, texCoord));

    //diffuse calculation
    float diffFactor = max(dot(fixedLightDir, normal), 0.0f);
    vec3 diffuse = light.diffuse * light.color * vec3(texture(material.diffuse, texCoord)) * diffFactor;

    //specular calculation
    float specFactor = pow(max(dot(reflection, cameraDirection), 0.0f), material.shininess);
    vec3 specular = light.specular * light.color * vec3(texture(material.specular, texCoord)) * specFactor;

    return (ambient + diffuse + specular);
}
