#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>
#include <iostream>

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

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // render/draw
    glClearColor(0.1f, 0.5f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // poll events and swap buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // terminate program
  glfwTerminate();
  return 0;
}