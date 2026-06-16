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
    //POSITION           TEXTURE COORDS
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,              //FACE 1
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,              //FACE 2
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,          //FACE 3
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,           //FACE 4
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,            //FACE 5
    0.5f, -0.5f, 0.5, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,             //FACE 6
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
  };

  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f, 0.0f, 0.0f),
    glm::vec3( 2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f, 2.0f, -2.5f),
    glm::vec3( 1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

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

  glm::mat4 proj = glm::mat4(1.0f);
  proj = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);

  //enable depth buffer
  glEnable(GL_DEPTH_TEST);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    //---------------------------render/draw--------------------------------------
    glClearColor(0.1f, 0.5f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //update uniforms
    //colour updates
    time = glfwGetTime();
    colorAdjust = std::abs(std::sin(time*2.0f));
    shader.setFloat("colorAdj", colorAdjust);

    //3d updates
    glm::mat4 view = glm::mat4(1.0f);
    float radius = 10.0f;
    view = glm::lookAt(glm::vec3(cos(time*2.0f)*radius, 0.0f, sin(time*2.0f)*radius), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //create 3d matrices

    shader.setMatrix4f("view", view);

    shader.setMatrix4f("proj", proj);


    //draw triangles
    shader.use();
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    for(glm::vec3 position : cubePositions){
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, position);
      model = glm::rotate(model, glm::radians((float)sin(time)*1000.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
      shader.setMatrix4f("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // poll events and swap buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // terminate program
  glfwTerminate();
  return 0;
}