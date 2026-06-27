#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>

class Mesh {
    public:

    //--------------------------------------FIELDS------------------------------------------

    GLuint VAO;
    GLuint VBO;
    const float* data;
    int vertexCount;

    //------------------------------------CONSTRUCTOR------------------------------------------

    Mesh(const float* dataSource, int vertexCount){
        this->data = dataSource;
        this->vertexCount = vertexCount;
        createBufferObjects();
    }
    Mesh() = default;

    //--------------------------------------METHODS--------------------------------------------

    void createBufferObjects(){
        //create VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        //create VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), data, GL_STATIC_DRAW);

        //set attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        //unbind for later use
        glBindVertexArray(0);
    }
    
    GLuint getVBO(){
        return VBO;
    }

    GLuint getVAO(){
        return VAO;
    }
};

#endif