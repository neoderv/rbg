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
  
  for (int i = 0; i < 256 * 256; i++) {
    int x = i % 256;
    int z = i / 256;
    int y = cubes[i];

    int yd = (z < 0) ? 0 : cubes[i-256];
    int yd2 = (z > 255) ? 0 : cubes[i+256];
    int yd3 = (x < 0) ? 0 : cubes[i-1];
    int yd4 = (x > 255) ? 0 : cubes[i+1];

    yd = (yd2 < yd) ? yd2 : yd;
    yd = (yd3 < yd) ? yd3 : yd;
    yd = (yd4 < yd) ? yd4 : yd;
    yd = (y < yd) ? y : yd;

    for (int i = yd-1; i < y; i++) {
      model cubeModel = getModel(fileContent, 6, 1, 0, 0, x * 2, i * 2, z * 2);

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