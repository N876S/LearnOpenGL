#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
    private:

    //fields
    unsigned int ID;

    public:

    //methods
    Shader(const char* vertexFilePath, const char* fragmentFilePath);
    Shader() = default;
    
    unsigned int getID();
    void use();
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setBool(const std::string &name, bool value);
    void set2f(const std::string &name, float f1, float f2);
    void set3f(const std::string &name, float f1, float f2, float f3);
    void set3f(const std::string &name, glm::vec3 vector);
    void set4f(const std::string &name, float f1, float f2, float f3, float f4);
    void setMatrix4f(const std::string &name, glm::mat4 matrix);
};