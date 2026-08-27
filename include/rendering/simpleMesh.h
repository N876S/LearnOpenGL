#pragma once
#include "rendering/shader.h"

class SimpleMesh {
    private:

    //--------------------------------------FIELDS------------------------------------------

    float* data;
    float dataSize;
    const char* texturePath;
    unsigned int textureID;

    unsigned int VBO;
    unsigned int VAO;

    //------------------------------------CONSTRUCTOR------------------------------------------

    public:

    //data takes vertex, texcoords
    SimpleMesh(float* data, float dataSize, const char* texturePath);
    SimpleMesh() = default;

    //--------------------------------------METHODS--------------------------------------------

    void draw(Shader& shader);
    void drawTransparent(Shader& shader);

    private:

    void setupMesh();
    void generateTexture();
};