#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <cmath>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
  void** pointers = (void**)glfwGetWindowUserPointer(window);
  Camera* cameraLoc = (Camera*)pointers[0];
  Camera& camera = *cameraLoc;

  float* deltaTimeLoc = (float*)pointers[1];
  float deltaTime = *deltaTimeLoc;

  float o = camera.movementSpeed*deltaTime;
  camera.updateBasis();

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    camera.updatePosition(o*camera.cameraDir);
  }
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
    camera.updatePosition(o*-camera.cameraDir);
  }
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    camera.updatePosition(o*-camera.cameraRight);
  }
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
    camera.updatePosition(o*camera.cameraRight);
  }
  if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
    camera.updatePosition(o*camera.cameraUp);
  }
  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    camera.updatePosition(o*-camera.cameraUp);
  }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
  void** pointers = (void**)glfwGetWindowUserPointer(window);
  Camera* cameraLoc = (Camera*)pointers[0];
  Camera& camera = *cameraLoc;

  camera.updateDirection(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
  void** pointers = (void**)glfwGetWindowUserPointer(window);
  float* fovPointer = (float*)pointers[2];
  float& fov = *fovPointer;

  fov += -(float)yoffset;
  if(fov > 45.0f){
    fov = 45.0f;
  }
  if(fov < 1.0f){
    fov = 1.0f;
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

  //-------------------------------one time calls-----------------------------------------
  // set viewport
  glViewport(0, 0, width, height);
  //set callback functions
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  //enable buffers
  glEnable(GL_DEPTH_TEST);
  //enable vsync
  glfwSwapInterval(1);
  //capture cursor
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  //-------------------------------space matrices-----------------------------------------

  float fov = 45.0f;
  float time = glfwGetTime();
  float lastTime = time;
  float deltaTime = time-lastTime;

  glm::mat4 proj = glm::mat4(1.0f);

  Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), 5.0f, 0.1f);

  void* inputPointers[3] = {&camera, &deltaTime, &fov};
  glfwSetWindowUserPointer(window, &inputPointers);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    //---------------------------render/draw--------------------------------------
    glClearColor(0.1f, 0.5f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //update uniforms
    time = glfwGetTime();
    deltaTime = time-lastTime;
    lastTime = time;
    shader.setMatrix4f("view", camera.getView());
    proj = glm::perspective(glm::radians(fov), (float) width / (float) height, 0.1f, 100.0f);
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