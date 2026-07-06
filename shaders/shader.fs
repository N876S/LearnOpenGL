#version 460 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 worldFragPos;
in vec3 cameraPosition;

uniform sampler2D textureData;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 pos;
    vec3 color;
    float ambient;
    float diffuse;
    float specular;
};

uniform Material material;
uniform Light light;

void main(){
    //important vector calculations
    vec3 fixedNormals = normalize(normal);
    vec3 fixedLightDir = normalize(light.pos - worldFragPos);
    vec3 reflection = reflect(-fixedLightDir, fixedNormals);
    vec3 cameraDirection = normalize(cameraPosition - worldFragPos);

    //ambient calculation
    vec3 ambient = light.ambient * light.color * vec3(texture(material.diffuse, texCoord));

    //diffuse calculation
    float diffFactor = max(dot(fixedLightDir, fixedNormals), 0.0f);
    vec3 diffuse = light.diffuse * light.color * vec3(texture(material.diffuse, texCoord)) * diffFactor;

    //specular calculation
    float specFactor = pow(max(dot(reflection, cameraDirection), 0.0f), material.shininess);
    vec3 specular = light.specular * light.color * vec3(texture(material.specular, texCoord)) * specFactor;

    //final fragment colour
    vec4 lightFactors = vec4((ambient + diffuse + specular), 1.0f);
    fragColor = lightFactors * texture(textureData, texCoord);
}
