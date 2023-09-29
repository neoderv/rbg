#include <math.h>
#include <stdio.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "types.h"
#include "util.h"

int wKey = 0;
int aKey = 0;
int sKey = 0;
int dKey = 0;
int escKey = 1;
int spaceKey = 0;

double xOff = 0;
double yOff = 0;

int getHash(double x, double y, double z, int *cubes) {
  if (y < 0)
    return 1;

  int hash = -1;
  if (x > 0 && x < RENDER_W * 2 && z > 0 && z < RENDER_W * 2 && y > 0 &&
      y < RENDER_H * 2) {
    hash = (((int)(z / 2 + 0.5)) * RENDER_W) + ((int)(x / 2 + 0.5));
    hash = (((int)(y / 2 + 0.5)) * RENDER_W * RENDER_W) + hash;
  }

  return (hash > -1 && hash < RENDER_W * RENDER_W * RENDER_H) ? cubes[hash]
                                                              : -1;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (action == GLFW_PRESS) {
    wKey |= (key == GLFW_KEY_W);
    aKey |= (key == GLFW_KEY_A);
    sKey |= (key == GLFW_KEY_S);
    dKey |= (key == GLFW_KEY_D);
    escKey = (key == GLFW_KEY_ESCAPE) ? !escKey : escKey;
    spaceKey = (key == GLFW_KEY_SPACE);
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
  controls->ox = controls->x;
  controls->oy = controls->y;
  controls->oz = controls->z;

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

  controls->z +=
      sin(controls->xr) * controls->xv - cos(controls->xr) * controls->zv;
  controls->x +=
      sin(controls->xr) * controls->zv + cos(controls->xr) * controls->xv;

  controls->y += controls->yv;

  int collide = 0;
  int collides[12];

  for (int i = 0; i < 8; i++) {
    collides[i] = getHash(controls->x + ((i % 2 == 0) ? 1.6 : -1.6),
                          controls->y + ((i % 4 < 2) ? 0 : 1.6),
                          controls->z + ((i % 8 < 4) ? 1.6 : -1.6), cubes) > 0;
    collide |= collides[i];
  }

  for (int i = 9; i < 12; i++) {
    collides[i] = getHash(controls->x + ((i % 2 == 0) ? 1.6 : -1.6),
                          controls->y + (-0.1),
                          controls->z + ((i % 8 < 4) ? 1.6 : -1.6), cubes) > 0;
  }

  if (spaceKey && (collides[8] || collides[9] || collides[10] || collides[11])) {
    controls->yv = 0.2;
  }

  spaceKey = 0;

  controls->yv -= 0.01;
  if (!collide)
    return;

  int cornerX = round(controls->x) != round(controls->ox);
  int cornerY = round(controls->y) != round(controls->oy);
  int cornerZ = round(controls->z) != round(controls->oz);

  printf("%f:%f:%f\n", cornerX, cornerY, cornerZ);

  if (cornerX && (collides[2] || collides[3] || collides[6] || collides[7])) {
    controls->x = controls->ox;
    controls->xv = 0.0;
  }
  if (cornerY || !(collides[2] || collides[3] || collides[6] || collides[7])) {
    controls->y = controls->oy;
    controls->yv = 0.0;
  }
  if (cornerZ && (collides[2] || collides[3] || collides[6] || collides[7])) {
    controls->z = controls->oz;
    controls->zv = 0.0;
  }
}