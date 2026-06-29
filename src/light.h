#ifndef LIGHT_H
#define LIGHT_H

#include "mesh.h"
#include "shader.h"

class Light {
    public:

    //--------------------------------------FIELDS------------------------------------------
    
    Mesh mesh;
    Shader shader;

    glm::vec3 position;

    //------------------------------------CONSTRUCTOR------------------------------------------

    Light(Shader* shader, Mesh mesh, glm::vec3 position){
        this->shader = *shader;
        this->mesh = mesh;
        this->position = position;
    }
    Light() = default;

    //--------------------------------------METHODS--------------------------------------------

    void render(const std::vector<glm::vec3> &positions, float time){
        //get count
        int count = positions.size();

        //draw triangles
        shader.use();
        glBindVertexArray(mesh.getVAO());

        for(glm::vec3 position : positions){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::rotate(model, glm::radians((float)sin(time)*1000.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
            shader.setMatrix4f("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void render(float time){
        //draw triangles
        shader.use();
        glBindVertexArray(mesh.getVAO());

        glm::mat4 model = glm::mat4(1.0f);
        //position.x = 10.0f*sin(time);
        //position.y = 10.0f*cos(time);
        //position.z = 10.0f*cos(time);
        model = glm::translate(model, position);
        shader.setMatrix4f("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glm::vec3 getPosition(){
        return position;
    }
};

#endif