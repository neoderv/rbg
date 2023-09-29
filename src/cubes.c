#include "gen.h"
#include "model.h"
#include "util.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int genCubes(GLuint vertex_buffer, GLuint uv_buffer, int* cubes) {

  char *fileContent = loadFile("../res/model/cube.model");

  float *points = malloc(1024 * 1024 * 1024);
  float *uvPoints = malloc(1024 * 1024 * 1024);
  int pointCount = 0;
  
  for (int i = 0; i < RENDER_W * RENDER_W * RENDER_H; i++) {
    int x = i % RENDER_W;
    int z = (i / RENDER_W) % RENDER_W;
    int y = i / (RENDER_W*RENDER_W);

    int yd = (z < 0) ? 0 : cubes[i-RENDER_W];
    int yd2 = (z > RENDER_W-1) ? 0 : cubes[i+RENDER_W];
    int yd3 = (x < 0) ? 0 : cubes[i-1];
    int yd4 = (x > RENDER_W-1) ? 0 : cubes[i+1];
    int yd5 = (y < 0) ? 0 : cubes[i-RENDER_W*RENDER_W];
    int yd6 = (y > RENDER_H-1) ? 0 : cubes[i+RENDER_W*RENDER_W];

    if (cubes[i] && (!yd || !yd2 || !yd3 || !yd4 || !yd5 || !yd6)) {
      model cubeModel = getModel(fileContent, 6, 1, 0, 2, x * 2, y * 2, z * 2);

      memcpy(&points[pointCount * 3], cubeModel.points,
             cubeModel.pointCount * 3 * sizeof(float));
      memcpy(&uvPoints[pointCount * 2], cubeModel.uvPoints,
             cubeModel.pointCount * 2 * sizeof(float));

      pointCount += cubeModel.pointCount;

      free(cubeModel.points);
      free(cubeModel.uvPoints);
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointCount * 3, points,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointCount * 2, uvPoints,
               GL_STATIC_DRAW);

  return pointCount;
}