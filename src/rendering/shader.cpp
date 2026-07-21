#include "rendering/shader.h"

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath){
    //----------------inputing from source files-------------------
    //create code strings and input file streams
    std::string vertexStringCode;
    std::string fragmentStringCode;
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
        vertexStringCode = vertexSS.str();
        fragmentStringCode = fragmentSS.str();

        //close input file streams
        vertexIFS.close();
        fragmentIFS.close();
    } catch (std::ifstream::failure e){
        std::cout << "FAILED TO READ FROM SHADER FILES\n" << e.what() << std::endl;
    }

    //add to vertex and fragment code char*
    const char* vertexCode = vertexStringCode.c_str();
    const char* fragmentCode = fragmentStringCode.c_str();

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

void Shader::use(){
    glUseProgram(ID);
}

void Shader::setInt(const std::string &name, int value){
    use();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value){
    use();
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setBool(const std::string &name, bool value){
    use();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::set2f(const std::string &name, float f1, float f2){
    use();
    glUniform2f(glGetUniformLocation(ID, name.c_str()), f1, f2);
}

void Shader::set3f(const std::string &name, float f1, float f2, float f3){
    use();
    glUniform3f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3);
}

void Shader::set3f(const std::string &name, glm::vec3 vector){
    use();
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::set4f(const std::string &name, float f1, float f2, float f3, float f4){
    use();
    glUniform4f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3, f4);
}

void Shader::setMatrix4f(const std::string &name, glm::mat4 matrix){
    use();
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}