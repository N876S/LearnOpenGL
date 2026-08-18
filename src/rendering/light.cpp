#include "rendering/light.h"
#include "rendering/model.h"

DirectionalLight::DirectionalLight(glm::vec3 color, Intensity intensity, glm::vec3 direction){
    this->color = color;
    this->intensity = intensity;
    this->direction = direction;
}

PointLight::PointLight(Shader* shader, const char* filePath, glm::vec3 position, glm::vec3 color, Intensity intensity, Attenuation att){
    this->shader = *shader;
    this->color = color;
    this->intensity = intensity;
    this->att = att;
    this->position = position;

    model = Model(filePath);

    this->shader.set3f("lightColor", color);
}

SpotLight::SpotLight(float innerCutoff, float outerCutoff, glm::vec3 color, Intensity intensity){
    this->innerCutoff = innerCutoff;
    this->outerCutoff = outerCutoff;
    this->color = color;
    this->intensity = intensity;
}

void PointLight::draw(){
    this->shader.set3f("lightColor", color);
    model.draw(shader);
}

glm::vec3 PointLight::getPosition(){
    return position;
}

glm::vec3 Light::getColor(){
    return color;
}

Light::Intensity Light::getIntensity(){
    return intensity;
}

PointLight::Attenuation PointLight::getAtt(){
    return att;
}

glm::vec3 DirectionalLight::getDirection(){
    return direction;
}

void Light::setColor(glm::vec3 color){
    this->color.x = color.x;
    this->color.y = color.y;
    this->color.z = color.z;
}

float SpotLight::getInnerCosCutoff(){
    return cos(glm::radians(innerCutoff));
}

float SpotLight::getOuterCosCutoff(){
    return cos(glm::radians(outerCutoff));
}

void PointLight::setPosition(float x, float y, float z){
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

void SpotLight::flipLight(){
    on = !on;
}

float SpotLight::isOn(){
    return on;
}