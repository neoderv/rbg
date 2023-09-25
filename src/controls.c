#include <stdio.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "types.h"

int wKey = 0;
int aKey = 0;
int sKey = 0;
int dKey = 0;

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (action == GLFW_PRESS) {
    wKey |= (key == GLFW_KEY_W);
    aKey |= (key == GLFW_KEY_A);
    sKey |= (key == GLFW_KEY_S);
    dKey |= (key == GLFW_KEY_D);
  } else if (action == GLFW_RELEASE) {
    wKey &= !(key == GLFW_KEY_W);
    aKey &= !(key == GLFW_KEY_A);
    sKey &= !(key == GLFW_KEY_S);
    dKey &= !(key == GLFW_KEY_D);
  }
}

void evalKeys(controls *controls) {
  if (wKey)
    controls->zv += 0.01;
  if (sKey)
    controls->zv -= 0.01;
  if (dKey)
    controls->xv += 0.01;
  if (aKey)
    controls->xv -= 0.01;

  controls->xv *= 0.9;
  controls->zv *= 0.9;

  controls->x += controls->xv;
  controls->z += controls->zv;
}