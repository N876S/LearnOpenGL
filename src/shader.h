#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
        //fields
        unsigned int ID;

        //methods
        Shader(const char* vertexFilePath, const char* fragmentFilePath){
            //----------------inputing from source files-------------------
            //create code strings and input file streams
            const char* vertexCode;
            const char* fragmentCode;
            std::ifstream vertexIFS;
            std::ifstream fragmentIFS;

            //exception throw enabling
            vertexIFS.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fragmentIFS.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            //catching exceptions
            try {
                //open files
                vertexIFS.open(vertexFilePath);
                fragmentIFS.open(fragmentFilePath);

                //create string streams to pour text into
                std::stringstream vertexSS;
                std::stringstream fragmentSS;

                //pour text into string streams
                vertexSS << vertexIFS.rdbuf();
                fragmentSS << fragmentIFS.rdbuf();

                //add to vertex and fragment code char*
                std::string vertexStringCode = vertexSS.str();
                std::string fragmentStringCode = fragmentSS.str();
                vertexCode = vertexStringCode.c_str();
                fragmentCode = fragmentStringCode.c_str();

                //close input file streams
                vertexIFS.close();
                fragmentIFS.close();
            } catch (std::ifstream::failure e){
                std::cout << "FAILED TO READ FROM SHADER FILES\n" << e.what() << std::endl;
            }

            //create shaders
            unsigned int vertexShader, fragmentShader;
            int success;
            const int outputLength = 512;
            char infoLog[outputLength];
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(vertexShader, 1, &vertexCode, NULL);
            glCompileShader(vertexShader);
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if(success == 0){
                glGetShaderInfoLog(vertexShader, outputLength, NULL, infoLog);
                std::cout << "FAILED TO COMPILE VERTEX SHADER\n" << infoLog << std::endl;
            }
            glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
            glCompileShader(fragmentShader);
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if(success == 0){
                glGetShaderInfoLog(fragmentShader, outputLength, NULL, infoLog);
                std::cout << "FAILED TO COMPILE FRAGMENT SHADER\n" << infoLog << std::endl;
            }

            //shader program
            ID = glCreateProgram();
            glAttachShader(ID, vertexShader);
            glAttachShader(ID, fragmentShader);
            glLinkProgram(ID);

            //error checking
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if(success == 0){
                glGetProgramInfoLog(ID, outputLength, NULL, infoLog);
                std::cout << "FAILED TO LINK SHADER PROGRAM\n" << infoLog << std::endl;
            }

            //activate new program
            glUseProgram(ID);

            //cleanup
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }
        void use(){
            glUseProgram(ID);
        }
        void setInt(const std::string &name, int value){
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string &name, float value){
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setBool(const std::string &name, bool value){
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void set2f(const std::string &name, float f1, float f2){
            glUniform2f(glGetUniformLocation(ID, name.c_str()), f1, f2);
        }
        void set3f(const std::string &name, float f1, float f2, float f3){
            glUniform3f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3);
        }
        void set4f(const std::string &name, float f1, float f2, float f3, float f4){
            glUniform4f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3, f4);
        }
};

#endif