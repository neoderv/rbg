#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "types.h"

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods);

void evalKeys(controls *controls, GLFWwindow *window);