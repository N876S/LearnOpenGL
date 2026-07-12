#ifndef LIGHT_H
#define LIGHT_H

#include "mesh.h"
#include "shader.h"

class Light {
    public:

    //--------------------------------------FIELDS------------------------------------------
    
    //Mesh mesh; -> only for point
    //Shader shader; -> only for point

    //glm::vec3 position; -> only for point
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

    virtual void render(const std::vector<glm::vec3> &positions, float time) = 0;
    virtual void render(float time) = 0;

    glm::vec3 getPosition();
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
};

#endif