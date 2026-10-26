#include "systems/input.h"
#include "rendering/light.h"
#include "core/engine.h"

void InputManager::processInput(GLFWwindow* window) {
  Engine* engine = (Engine*)glfwGetWindowUserPointer(window);
  Camera* camera = engine->getCamera();

  float o = camera->getMovementSpeed() * engine->getDeltaTime();
  camera->updateBasis();

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    camera->updatePosition(o*camera->getDir());
  }
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
    camera->updatePosition(o*-camera->getDir());
  }
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    camera->updatePosition(o*-camera->getRight());
  }
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
    camera->updatePosition(o*camera->getRight());
  }
  if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
    camera->updatePosition(o*camera->getUp());
  }
  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    camera->updatePosition(o*-camera->getUp());
  }
}

void InputManager::mouse_callback(GLFWwindow* window, double xpos, double ypos){
  Engine* engine = (Engine*)glfwGetWindowUserPointer(window);
  Camera* camera = engine->getCamera();

  camera->updateDirection(xpos, ypos);
}

void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
  Engine* engine = (Engine*)glfwGetWindowUserPointer(window);
  Camera* camera = engine->getCamera();

  camera->updateFov(-(float)yoffset);
  if(camera->getFov() > 45.0f){
    camera->setFov(45.0f);
  }
  if(camera->getFov() < 1.0f){
    camera->setFov(1.0f);
  }
}

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  Engine* engine = (Engine*)glfwGetWindowUserPointer(window);

  if(key == GLFW_KEY_K && action == GLFW_PRESS){
    engine->spotLightSwitch();
  }
}
 
void InputManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}