#include "rendering/mesh.h"

Mesh::Mesh(const float* dataSource, int vertexCount, int lineCount){
    this->data = dataSource;
    this->vertexCount = vertexCount;
    this->lineCount = lineCount;
    createBufferObjects();
}

void Mesh::createBufferObjects(){
    //create VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //create VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), data, GL_STATIC_DRAW);

    //set attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, lineCount*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, lineCount*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    if(lineCount == 8){
        //texture
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, lineCount*sizeof(float), (void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    //unbind for later use
    glBindVertexArray(0);
}

GLuint Mesh::getVBO(){
    return VBO;
}

GLuint Mesh::getVAO(){
    return VAO;
}