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

  for (int i = 0; i < 10000; i++) {
    model cubeModel = getModel(fileContent, 6, 1, 0, 1, i % 100 * 2, 0, i / 100 * 2);

    memcpy(&points[pointCount*3],cubeModel.points,cubeModel.pointCount * 3 * sizeof(float));
    memcpy(&uvPoints[pointCount*2],cubeModel.uvPoints,cubeModel.pointCount * 2 * sizeof(float));
    
    pointCount += cubeModel.pointCount;

    free(cubeModel.points);
    free(cubeModel.uvPoints);
  }

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointCount * 3, points,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointCount * 2, uvPoints,
               GL_STATIC_DRAW);

  return pointCount;
}