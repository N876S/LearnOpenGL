#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <cmath>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  int width = 1920;
  int height = 1080;
  const char* name = "LearnOpenGL";

  // initialize GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // make window
  GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to make GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to load GLAD" << std::endl;
    return -1;
  }

  // set viewport
  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float data[] = { 
    //POSITION            COLOR               TEXTURE COORDS
    -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f
  };

  //create shader
  Shader shader("../shaders/shader.vs", "../shaders/shader.fs");

  //VAO
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  //VBO
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

  //attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
  glEnableVertexAttribArray(2);

  //create texture
  unsigned int texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  stbi_set_flip_vertically_on_load(true);
  int tWidth, tHeight, nrChannels;
  unsigned char* tData = stbi_load("../res/image.jpg", &tWidth, &tHeight, &nrChannels, 0);
  if(tData != 0){
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tData);
  } else {
    std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
  }
  shader.use();
  shader.setInt("textureData", 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  //unbind for later use
  glBindVertexArray(0);

  //colour logic
  float time, colorAdjust;

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    //---------------------------render/draw--------------------------------------
    glClearColor(0.1f, 0.5f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //update uniforms
    //colour updates
    time = glfwGetTime();
    colorAdjust = std::abs(std::sin(time*2.0f));
    shader.setFloat("colorAdj", colorAdjust);

    //movement update
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(sin(time)*0.5f, sin(time)*0.5f, sin(time)*0.5f));
    trans = glm::rotate(trans, glm::radians((float)sin(time)*180.0f), glm::normalize(glm::vec3(0.5f, 0.7f, 0.5f)));
    shader.setMatrix4f("trans", trans);

    //3d updates
    

    //draw triangles
    shader.use();
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // poll events and swap buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // terminate program
  glfwTerminate();
  return 0;
}