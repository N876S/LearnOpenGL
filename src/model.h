#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "stb_image.h"
#include "mesh.h"
#include "material.h"

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

    Model(Shader* shader, Mesh mesh, const char* textureSource, const char* specularMapSource, MaterialType material){
        this->shader = *shader;
        this->mesh = mesh;
        this->textureSource = textureSource;
        this->specularMapSource = specularMapSource;
        this->materialType = material;
        this->materialConfig = getMaterial(material);
        createTexture();
    }
    Model(Shader* shader, Mesh mesh, glm::vec3 color, MaterialType material){
        this->shader = *shader;
        this->mesh = mesh;
        this->materialType = material;
        this->materialConfig = getMaterial(material);
        this->shader.set3f("color", color);
        setMaterial();
    }
    Model() = default;

    //--------------------------------------METHODS--------------------------------------------

    void render(const std::vector<glm::vec3> &positions, float time){
        //get count
        int count = positions.size();

        //draw triangles
        shader.use();
        glBindVertexArray(mesh.getVAO());
        setTextures();
        for(glm::vec3 position : positions){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            //model = glm::rotate(model, glm::radians((float)sin(time)*1000.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
            shader.setMatrix4f("model", model);
            glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount/mesh.lineCount);
        }
    }

    void render(glm::vec3 position, float time){
        shader.use();
        glBindVertexArray(mesh.getVAO());
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(50.0f, 1.0f, 50.0f));
        shader.setMatrix4f("model", model);
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount/mesh.lineCount);
    }

    void setTextures(){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.setInt("textureData", 0);
        shader.setInt("material.diffuse", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", materialConfig.shininess);
    }

    void setMaterial(){
        shader.set3f("material.ambient", materialConfig.ambient);
        shader.set3f("material.diffuse", materialConfig.diffuse);
        shader.set3f("material.specular", materialConfig.specular);
        shader.setFloat("material.shininess", materialConfig.shininess);
    }
    
    void createTexture(){
        glBindVertexArray(mesh.getVAO());
        
        //create texture
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(true);
        int tWidth, tHeight, nrChannels;
        unsigned char* tData = stbi_load(textureSource, &tWidth, &tHeight, &nrChannels, 0);
        if(tData != 0){
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tData);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(tData);
        } else {
            std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
        }

        //create specular map
        glGenTextures(1, &specularMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        tData = stbi_load(specularMapSource, &tWidth, &tHeight, &nrChannels, 0);
        if(tData != 0){
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tData);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(tData);
        } else {
            std::cout << "FAILED TO LOAD SPECULAR MAP" << std::endl;
        }

        //unbind for later use
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }

    MaterialType getMaterialType(){
        return MaterialType();
    }
};

#endif