#pragma once

#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>

class Mesh {
    public:

    //--------------------------------------FIELDS------------------------------------------

    GLuint VAO;
    GLuint VBO;
    const float* data;
    int vertexCount;
    int lineCount;

    //------------------------------------CONSTRUCTOR------------------------------------------

    Mesh(const float* dataSource, int vertexCount, int lineCount);
    Mesh() = default;

    //--------------------------------------METHODS--------------------------------------------

    void createBufferObjects();
    
    GLuint getVBO();
    GLuint getVAO();
};