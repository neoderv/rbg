#include "gen.h"
#include "model.h"
#include "util.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int genCubes(GLuint vertex_buffer, GLuint uv_buffer) {

  char *fileContent = loadFile("../res/model/cube.model");

  float *points = malloc(1024 * 1024 * 1024);
  float *uvPoints = malloc(1024 * 1024 * 1024);
  int pointCount = 0;

  for (int i = 0; i < 256 * 256; i++) {
    int x = i % 256;
    int z = i / 256;
    int y = getCube(x, z, 694200);

    int yd = getCube(x + 1, z, 694200);
    int yd2 = getCube(x - 1, z, 694200);
    int yd3 = getCube(x, z + 1, 694200);
    int yd4 = getCube(x, z - 1, 694200);

    yd = (yd2 < yd) ? yd2 : yd;
    yd = (yd3 < yd) ? yd3 : yd;
    yd = (yd4 < yd) ? yd4 : yd;
    yd = (y < yd) ? y : yd;

    for (int i = yd-1; i < y; i++) {
      model cubeModel = getModel(fileContent, 6, 1, 0, 1, x * 2, i * 2, z * 2);

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