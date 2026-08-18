#include "core/engine.h"
#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>

int main() {

  Engine engine = Engine();

  engine.render();

  return 0;
}