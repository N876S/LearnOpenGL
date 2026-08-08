#include "rendering/mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices){
    this->vertices = vertices;
    this->textures = textures;
    this->indices = indices;

    setupMesh();
}

void Mesh::Draw(Shader& shader){
    //use set Shader, VAO, textures
    shader.use();
    glBindVertexArray(VAO);

    int diffuseTextureCount = 1;
    int specularTextureCount = 1;

    for(int i = 0; i < textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);

        std::string numberType;
        std::string nameType = textures[i].type;

        if(nameType == "texture_diffuse"){
            numberType = std::to_string(diffuseTextureCount);
            diffuseTextureCount++;
        } else if(nameType == "texture_specular"){
            numberType = std::to_string(specularTextureCount);
            specularTextureCount++;
        }

        shader.setInt(("material." + nameType + numberType).c_str(), i);
    }
    glActiveTexture(GL_TEXTURE0);

    //draw
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh(){
    //create VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //create VBO and buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    //create EBO and buffer
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    //attrib pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    //unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}