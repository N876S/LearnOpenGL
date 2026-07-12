#include "systems/input.h"

void InputManager::processInput(GLFWwindow* window) {
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

void InputManager::mouse_callback(GLFWwindow* window, double xpos, double ypos){
  void** pointers = (void**)glfwGetWindowUserPointer(window);
  Camera* cameraLoc = (Camera*)pointers[0];
  Camera& camera = *cameraLoc;

  camera.updateDirection(xpos, ypos);
}

void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
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

void InputManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}