#include <math.h>
#include <stdio.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "types.h"

int wKey = 0;
int aKey = 0;
int sKey = 0;
int dKey = 0;
int escKey = 1;

double xOff = 0;
double yOff = 0;

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (action == GLFW_PRESS) {
    wKey |= (key == GLFW_KEY_W);
    aKey |= (key == GLFW_KEY_A);
    sKey |= (key == GLFW_KEY_S);
    dKey |= (key == GLFW_KEY_D);
    escKey = (key == GLFW_KEY_ESCAPE) ? !escKey : escKey;
  } else if (action == GLFW_RELEASE) {
    wKey &= !(key == GLFW_KEY_W);
    aKey &= !(key == GLFW_KEY_A);
    sKey &= !(key == GLFW_KEY_S);
    dKey &= !(key == GLFW_KEY_D);
  }

  if (escKey) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  } else {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
}

void evalKeys(controls *controls, GLFWwindow *window, int *cubes) {
  double xr, yr;
  glfwGetCursorPos(window, &xr, &yr);

  if (!escKey) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    controls->xr += (xr - width / 2) * 0.003;
    controls->yr -= (yr - height / 2) * 0.003;

    controls->yr = (controls->yr > M_PI / 2) ? M_PI / 2 : controls->yr;
    controls->yr = (controls->yr < -M_PI / 2) ? -M_PI / 2 : controls->yr;

    glfwSetCursorPos(window, width / 2, height / 2);
  }

  if (wKey)
    controls->zv += 0.02;
  if (sKey)
    controls->zv -= 0.02;
  if (dKey)
    controls->xv += 0.02;
  if (aKey)
    controls->xv -= 0.02;

  controls->xv *= 0.9;
  controls->zv *= 0.9;

  int hash = -1;
  if (controls->x > 0 && controls->x < 512 && controls->z > 0 &&
      controls->z < 512)
    hash = (((int)(controls->z / 2 + 0.5)) * 256) + ((int)(controls->x / 2 + 0.5));

  printf("%i\n",((int)(controls->z * 2)));

  if (hash > -1 && hash < 256*256 )
    hash = cubes[hash] * 2;

  controls->yv -= 0.1;

  controls->z +=
      sin(controls->xr) * controls->xv - cos(controls->xr) * controls->zv;
  controls->x +=
      sin(controls->xr) * controls->zv + cos(controls->xr) * controls->xv;

  controls->y += controls->yv;

  if (controls->y < hash) {
    printf("oh no!\n");
    controls->y = hash;
    controls->yv = 0;
  }
}