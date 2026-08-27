#include "rendering/simpleMesh.h"
#include "stb_image/stb_image.h"

SimpleMesh::SimpleMesh(float* data, float dataSize, const char* texturePath){
    this->data = data;
    this->dataSize = dataSize;
    this->texturePath = texturePath;

    setupMesh();
    generateTexture();
}

void SimpleMesh::draw(Shader& shader){
    shader.use();
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    shader.setInt("diffuse_texture", 0);

    glDrawArrays(GL_TRIANGLES, 0, dataSize/5.0f);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void SimpleMesh::drawTransparent(Shader& shader){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    draw(shader);
}

void SimpleMesh::setupMesh(){
    //create VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //create VBO and buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(float), data, GL_STATIC_DRAW);

    //attrib pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SimpleMesh::generateTexture(){
    //create texture
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    int tWidth, tHeight, nrChannels;
    std::cout << "Loading texture from path: " << texturePath << std::endl;
    unsigned char* tData = stbi_load(texturePath, &tWidth, &tHeight, &nrChannels, 0);
    
    GLenum format;
    if(nrChannels == 1){
        format = GL_RED;
    } else if(nrChannels == 3){
        format = GL_RGB;
    } else if(nrChannels == 4){
        format = GL_RGBA;
    }
    if(tData != 0){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, tWidth, tHeight, 0, format, GL_UNSIGNED_BYTE, tData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(tData);
    } else {
        std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
        stbi_image_free(tData);
    }

    //unbind for later use
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}