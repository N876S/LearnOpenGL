#version 460 core

out vec4 fragColor;

in vec3 normal;
in vec3 worldFragPos;
in vec3 cameraPosition;

uniform vec3 lightPos;
uniform vec3 color;
uniform vec3 lightColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    vec3 ambient = light.ambient * light.color * material.ambient;

    //diffuse calculation
    float diffFactor = max(dot(fixedLightDir, fixedNormals), 0.0f);
    vec3 diffuse = light.diffuse * light.color * material.diffuse * diffFactor;

    //specular calculation
    float specFactor = pow(max(dot(reflection, cameraDirection), 0.0f), material.shininess);
    vec3 specular = light.specular * light.color * material.specular * specFactor;

    //final fragment colour
    vec4 lightFactors = vec4((ambient + diffuse + specular), 1.0f);
    fragColor = lightFactors * vec4(color, 1.0f);
}
