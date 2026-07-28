#include "rendering/light.h"

DirectionalLight::DirectionalLight(glm::vec3 color, Intensity intensity, glm::vec3 direction){
    this->color = color;
    this->intensity = intensity;
    this->direction = direction;
}

PointLight::PointLight(Shader* shader, Mesh mesh, glm::vec3 position, glm::vec3 color, Intensity intensity, Attenuation att){
    this->shader = *shader;
    this->mesh = mesh;
    this->color = color;
    this->intensity = intensity;
    this->att = att;
    this->position = position;

    this->shader.set3f("lightColor", color);
}

SpotLight::SpotLight(float innerCutoff, float outerCutoff, glm::vec3 color, Intensity intensity){
    this->innerCutoff = innerCutoff;
    this->outerCutoff = outerCutoff;
    this->color = color;
    this->intensity = intensity;
}

void PointLight::render(float time){
    //draw triangles
    shader.use();
    glBindVertexArray(mesh.getVAO());

    position.x = 5.0f*sin(time);
    //position.y = 10.0f*cos(time);
    position.z = 5.0f*cos(time);
    shader.set3f("lightColor", color);

    //model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    shader.setMatrix4f("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
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