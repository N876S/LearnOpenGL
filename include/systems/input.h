#pragma once

#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include "systems/camera.h"

class InputManager {
  private:

  inline static Camera* camera = nullptr;
  inline static float deltaTime;

  public:

  //---------------------------------CONSTRUCTOR-----------------------------------------

  InputManager(Camera& camera);
  InputManager() = default;

  //------------------------------------METHODS------------------------------------------

  void processInput(GLFWwindow* window);

  static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

  void updateInputData(Camera& camera, float deltaTime);
};