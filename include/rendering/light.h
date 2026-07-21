#ifndef LIGHT_H
#define LIGHT_H

#include "mesh.h"
#include "shader.h"

class Light {
    public:

    //--------------------------------------FIELDS------------------------------------------
    
    glm::vec3 color;
    struct Intensity {
        float ambient;
        float diffuse;
        float specular;
    };

    Intensity intensity;

    //------------------------------------CONSTRUCTOR------------------------------------------

    virtual ~Light() = default;

    //--------------------------------------METHODS--------------------------------------------

    glm::vec3 getColor();
    Intensity getIntensity();

    void setColor(glm::vec3 color);
};

class DirectionalLight : public Light {
    public:

    //--------------------------------------FIELDS------------------------------------------

    glm::vec3 direction;

    //------------------------------------CONSTRUCTOR------------------------------------------

    DirectionalLight(glm::vec3 color, Intensity intensity, glm::vec3 direction);
    DirectionalLight() = default;

    //--------------------------------------METHODS--------------------------------------------

    glm::vec3 getDirection();
};

class PointLight : public Light {
    public:

    //--------------------------------------FIELDS------------------------------------------

    Mesh mesh;
    Shader shader;

    glm::vec3 position;

    //------------------------------------CONSTRUCTOR------------------------------------------

    PointLight(Shader* shader, Mesh mesh, glm::vec3 color, Intensity intensity);
    PointLight() = default;

    //--------------------------------------METHODS--------------------------------------------

    void render(float time);
    glm::vec3 getPosition();
};

#endif