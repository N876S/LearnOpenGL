#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "rendering/shader.h"
#include "stb_image/stb_image.h"
#include "rendering/mesh.h"
#include "rendering/material.h"

class Model {
    public:

    //--------------------------------------FIELDS------------------------------------------
    
    Mesh mesh;
    GLuint texture;
    GLuint specularMap;
    Shader shader;
    const char* textureSource;
    const char* specularMapSource;
    glm::vec3 color;
    MaterialType materialType;
    MaterialConfig materialConfig;

    //------------------------------------CONSTRUCTOR------------------------------------------

    Model(Shader* shader, Mesh mesh, const char* textureSource, const char* specularMapSource, MaterialType material);
    Model(Shader* shader, Mesh mesh, glm::vec3 color, MaterialType material);
    Model() = default;

    //--------------------------------------METHODS--------------------------------------------

    void render(const std::vector<glm::vec3> &positions, float time);
    void render(glm::vec3 position, float time);

    void setTextures();
    void setMaterial();
    
    void createTexture();

    MaterialType getMaterialType();
};

#endif