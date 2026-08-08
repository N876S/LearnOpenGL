#pragma once

#include "mesh.h"
#include "shader.h"

class Light {
    public:

    //--------------------------------------FIELDS------------------------------------------

    struct Intensity {
        float ambient;
        float diffuse;
        float specular;
    };

    protected:

    Intensity intensity;
    glm::vec3 color;

    //------------------------------------CONSTRUCTOR-----------------------------------------

    public:

    virtual ~Light() = default;

    //--------------------------------------METHODS--------------------------------------------

    glm::vec3 getColor();
    Intensity getIntensity();

    void setColor(glm::vec3 color);
};

class DirectionalLight : public Light {
    private:

    //--------------------------------------FIELDS------------------------------------------

    glm::vec3 direction;

    //------------------------------------CONSTRUCTOR------------------------------------------

    public:

    DirectionalLight(glm::vec3 color, Intensity intensity, glm::vec3 direction);
    DirectionalLight() = default;

    //--------------------------------------METHODS--------------------------------------------

    glm::vec3 getDirection();
};

class PointLight : public Light {
    private:

    //--------------------------------------FIELDS------------------------------------------

    Mesh mesh;
    Shader shader;

    glm::vec3 position;

    struct Attenuation {
        float constant;
        float linear;
        float quadratic;
    };

    Attenuation att;

    //------------------------------------CONSTRUCTOR------------------------------------------

    public:

    PointLight(Shader* shader, Mesh mesh, glm::vec3 position, glm::vec3 color, Intensity intensity, Attenuation att);
    PointLight() = default;

    //--------------------------------------METHODS--------------------------------------------

    void render(float time);
    glm::vec3 getPosition();
    Attenuation getAtt();
};

class SpotLight : public Light {
    private:

    //--------------------------------------FIELDS------------------------------------------
    
    float innerCutoff;
    float outerCutoff;

    //------------------------------------CONSTRUCTOR------------------------------------------

    public:

    SpotLight(float innerCutoff, float outerCutoff, glm::vec3 color, Intensity intensity);
    SpotLight() = default;

    //--------------------------------------METHODS--------------------------------------------

    float getInnerCosCutoff();
    float getOuterCosCutoff();
};