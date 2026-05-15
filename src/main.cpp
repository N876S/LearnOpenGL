#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <cmath>

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
    //POSITION            COLOR
    -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f
  };

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  //unbind for later use
  glBindVertexArray(0);

  //create shader
  Shader shader("../shaders/shader.vs", "../shaders/shader.fs");

  //colour logic
  float time, colorAdjust;

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    //---------------------------render/draw--------------------------------------
    glClearColor(0.1f, 0.5f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //update colour uniforms
    time = glfwGetTime();
    colorAdjust = std::abs(std::sin(time*2.0f));
    shader.setFloat("colorAdj", colorAdjust);


    //draw triangles
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // poll events and swap buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // terminate program
  glfwTerminate();
  return 0;
}