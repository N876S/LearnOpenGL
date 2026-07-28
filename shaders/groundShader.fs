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

struct SpotLight {
    vec3 color;

    float cosInnerCutoff;
    float cosOuterCutoff;

    float ambient;
    float diffuse;
    float specular;
};

uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform vec3 cameraPointDirection;

vec3 getDirLightFactor(DirLight light, vec3 normal, vec3 fragToCamera);
vec3 getPointLightFactor(PointLight light, vec3 normal, vec3 fragToCamera);
vec3 getSpotLightFactor(SpotLight light, vec3 lightToFrag);

void main(){
    //important vector calculations (global to pass into functions)
    vec3 fixedNormals = normalize(normal);
    vec3 fragToCamera = normalize(cameraPosition - worldFragPos);

    vec3 outputLightColor = vec3(0.0f);

    outputLightColor += getDirLightFactor(dirLight, fixedNormals, fragToCamera);
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        outputLightColor += getPointLightFactor(pointLights[i], fixedNormals, fragToCamera);
    }
    outputLightColor += getSpotLightFactor(spotLight, -fragToCamera);
    
    fragColor = vec4(outputLightColor, 1.0f) * vec4(material.color, 1.0f);
}

//calculate direction light
vec3 getDirLightFactor(DirLight light, vec3 normal, vec3 fragToCamera){
    vec3 flippedLightDirection = normalize(-light.direction);
    vec3 reflectedLight = reflect(light.direction, normal);

    //ambient calculation
    vec3 ambient = light.ambient * light.color * material.color;

    //diffuse calculation
    float diffFactor = max(dot(flippedLightDirection, normal), 0.0f);
    vec3 diffuse = light.diffuse * light.color * material.color * diffFactor;

    //specular calculation
    float specFactor = pow(max(dot(reflectedLight, fragToCamera), 0.0f), material.shininess);
    vec3 specular = light.specular * light.color * material.color * specFactor;

    return (ambient + diffuse + specular);
}

//calculate point light
vec3 getPointLightFactor(PointLight light, vec3 normal, vec3 fragToCamera){
    vec3 fragToLight = normalize(light.position - worldFragPos);
    vec3 reflectedLight = reflect(-fragToLight, normal);

    //ambient calculation
    vec3 ambient = light.ambient * light.color * material.color;

    //diffuse calculation
    float diffFactor = max(dot(fragToLight, normal), 0.0f);
    vec3 diffuse = light.diffuse * light.color * material.color * diffFactor;

    //specular calculation
    float specFactor = pow(max(dot(reflectedLight, fragToCamera), 0.0f), material.shininess);
    vec3 specular = light.specular * light.color * material.color * specFactor;

    //attenuation
    float distanceToLight = length(light.position - worldFragPos);
    float attenuationFactor = 1.0 / ((light.constant) + (light.linear * distanceToLight) + (light.quadratic * distanceToLight * distanceToLight));

    ambient *= attenuationFactor;
    diffuse *= attenuationFactor;
    specular *= attenuationFactor;

    return (ambient + diffuse + specular);
}

//calculate spot light
vec3 getSpotLightFactor(SpotLight light, vec3 lightToFrag){
    vec3 reflectedLight = reflect(cameraPointDirection, normal);

    //spotlight intensity calculation
    float cosFrag = dot(lightToFrag, cameraPointDirection);
    float intensity = ((cosFrag - light.cosOuterCutoff) / (light.cosInnerCutoff - light.cosOuterCutoff));
    intensity = clamp(intensity, 0.0f, 1.0f);

    //ambient calculation
    vec3 ambient = light.ambient * light.color * material.color;

    //diffuse calculation
    float diffFactor = max(dot(-cameraPointDirection, normal), 0.0f);
    vec3 diffuse = light.diffuse * light.color * material.color * diffFactor;

    //specular calculation
    float specFactor = pow(max(dot(reflectedLight, -lightToFrag), 0.0f), material.shininess);
    vec3 specular = light.specular * light.color * material.color * specFactor;

    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}