#pragma once

#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "shader.h"

class Mesh {
    public:

    //--------------------------------------FIELDS------------------------------------------

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    struct Texture {
        unsigned int ID;
        std::string type;
        std::string path;
    };

    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;

    private:

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    //------------------------------------CONSTRUCTOR------------------------------------------

    public:

    Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices);
    Mesh() = default;

    //--------------------------------------METHODS--------------------------------------------

    void Draw(Shader& shader);
    
    private:

    void setupMesh();
};