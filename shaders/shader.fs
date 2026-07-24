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

struct PointLight {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;

    float ambient;
    float diffuse;
    float specular;
};

uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D textureData;

vec3 getDirLightFactor(DirLight light, vec3 normal, vec3 cameraDirection);
vec3 getPointLightFactor(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){
    //important vector calculations (global to pass into functions)
    vec3 fixedNormals = normalize(normal);
    vec3 cameraDirection = normalize(cameraPosition - worldFragPos);

    vec3 outputLightColor = vec3(0.0f);

    outputLightColor += getDirLightFactor(dirLight, fixedNormals, cameraDirection);
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        outputLightColor += getPointLightFactor(pointLights[i], fixedNormals, worldFragPos, cameraDirection);
    }
    
    fragColor = vec4(outputLightColor, 1.0f) * texture(textureData, texCoord);
}

//calculate direction light
vec3 getDirLightFactor(DirLight light, vec3 normal, vec3 cameraDirection){
    vec3 fixedLightDir = normalize(-light.direction);
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

//calculate point light
vec3 getPointLightFactor(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraDirection){
    vec3 directionToLight = normalize(light.position - fragPos);
    vec3 reflection = reflect(-directionToLight, normal);

    //ambient calculation
    vec3 ambient = light.ambient * light.color * vec3(texture(material.diffuse, texCoord));

    //diffuse calculation
    float diffFactor = max(dot(directionToLight, normal), 0.0f);
    vec3 diffuse = light.diffuse * light.color * vec3(texture(material.diffuse, texCoord)) * diffFactor;

    //specular calculation
    float specFactor = pow(max(dot(reflection, cameraDirection), 0.0f), material.shininess);
    vec3 specular = light.specular * light.color * vec3(texture(material.specular, texCoord)) * specFactor;

    //attenuation
    float distanceToLight = length(light.position - fragPos);
    float attenuationFactor = 1.0 / ((light.constant) + (light.linear * distanceToLight) + (light.quadratic * distanceToLight * distanceToLight));

    ambient *= attenuationFactor;
    diffuse *= attenuationFactor;
    specular *= attenuationFactor;

    return (ambient + diffuse + specular);
}