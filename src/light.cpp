#include "light.h"

DirectionalLight::DirectionalLight(glm::vec3 color, Intensity intensity, glm::vec3 direction){
    this->color = color;
    this->intensity = intensity;
}

PointLight::PointLight(Shader* shader, Mesh mesh, glm::vec3 color, Intensity intensity, glm::vec3 direction){
    this->shader = *shader;
    this->mesh = mesh;
    this->color = color;
    this->intensity = intensity;

    this->shader.set3f("lightColor", color);
}

void PointLight::render(const std::vector<glm::vec3> &positions, float time){
    //get count
    int count = positions.size();

    //draw triangles
    shader.use();
    glBindVertexArray(mesh.getVAO());

    for(glm::vec3 position : positions){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians((float)sin(time)*1000.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
        shader.setMatrix4f("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void PointLight::render(float time){
    //draw triangles
    shader.use();
    glBindVertexArray(mesh.getVAO());

    //position.x = 5.0f*sin(time);
    //position.y = 10.0f*cos(time);
    //position.z = 5.0f*cos(time);
    shader.set3f("lightColor", color);

    //model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    shader.setMatrix4f("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

glm::vec3 Light::getPosition(){
    return position;
}

glm::vec3 Light::getColor(){
    return color;
}

Light::Intensity Light::getIntensity(){
    return intensity;
}

void Light::setColor(glm::vec3 color){
    this->color.x = color.x;
    this->color.y = color.y;
    this->color.z = color.z;
}