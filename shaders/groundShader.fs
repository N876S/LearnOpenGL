#version 460 core

out vec4 fragColor;

in vec3 normal;
in vec3 worldFragPos;
in vec3 cameraPosition;

struct Material {
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

vec3 getDirLightFactor(DirLight light, vec3 normal, vec3 cameraDirection);

void main(){
     //important vector calculations (global to pass into functions)
    vec3 fixedNormals = normalize(normal);
    vec3 cameraDirection = normalize(cameraPosition - worldFragPos);

    vec3 outputLightColor = vec3(0.0f);

    outputLightColor += getDirLightFactor(dirLight, fixedNormals, cameraDirection);
    
    fragColor = vec4(outputLightColor, 1.0f) * vec4(material.color, 1.0f);
}

//calculate direction light
vec3 getDirLightFactor(DirLight light, vec3 normal, vec3 cameraDirection){
    vec3 fixedLightDir = normalize(-light.direction);
    vec3 reflection = reflect(-fixedLightDir, normal);

    //ambient calculation
    vec3 ambient = light.ambient * light.color * material.color;

    //diffuse calculation
    float diffFactor = max(dot(fixedLightDir, normal), 0.0f);
    vec3 diffuse = light.diffuse * light.color * material.color * diffFactor;

    //specular calculation
    float specFactor = pow(max(dot(reflection, cameraDirection), 0.0f), material.shininess);
    vec3 specular = light.specular * light.color * specFactor;

    return (ambient + diffuse + specular);
}