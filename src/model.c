#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct model {
  float *points;
  float *uvPoints;
  int pointCount;
} typedef model;

float parseFloat(char *strBegin, char *strEnd) {
  float decay = 1.0;
  float parsedFloat = 0.0;
  int pastDot = 0;
  int sign = 1;

  for (char *i = strBegin; i < strEnd; i++) {
    char currChar = *i;

    if (currChar == '-')
      sign = -1;
    if (currChar == '.')
      pastDot = 1;

    if ('0' <= currChar && currChar <= '9') {
      parsedFloat = parsedFloat * 10.0 + (currChar - '0');
      if (pastDot)
        decay *= 0.1;
    }
  }
  return sign * decay * parsedFloat;
}

model getModel(char *file, int uvWidth, int uvHeight) {
  char *fileContent = loadFile(file);

  model foundModel = {NULL, 0};
  char *commaLocation = fileContent;
  int commaCount = 0;

  while (1) {
    commaLocation = strstr(commaLocation + 1, ",");
    if (commaLocation == NULL)
      break;
    commaCount++;
  }

  foundModel.points = malloc(sizeof(float) * commaCount);
  foundModel.uvPoints = malloc(sizeof(float) * (commaCount / 3) * 2);
  foundModel.pointCount = commaCount;

  commaLocation = fileContent;
  char *prevCommaLocation = fileContent;
  for (int i = 0; i < foundModel.pointCount; i++) {

    commaLocation = strstr(commaLocation + 1, ",");
    if (commaLocation == NULL)
      break;

    foundModel.points[i] = parseFloat(prevCommaLocation, commaLocation);

    prevCommaLocation = commaLocation;
  }

  for (int i = 0; i < commaCount / 18; i++) {
    float minX = (i % uvWidth) / ((float)uvWidth);
    float minZ = (i / uvWidth) / ((float)uvHeight);

    float maxX = minX + (1.0 / uvWidth);
    float maxZ = minZ + (1.0 / uvHeight);

    foundModel.uvPoints[i * 12 + 0] = minX;
    foundModel.uvPoints[i * 12 + 1] = minZ;
    foundModel.uvPoints[i * 12 + 2] = minX;
    foundModel.uvPoints[i * 12 + 3] = maxZ;
    foundModel.uvPoints[i * 12 + 4] = maxX;
    foundModel.uvPoints[i * 12 + 5] = minZ;
    foundModel.uvPoints[i * 12 + 6] = minX;
    foundModel.uvPoints[i * 12 + 7] = maxZ;
    foundModel.uvPoints[i * 12 + 8] = maxX;
    foundModel.uvPoints[i * 12 + 9] = minZ;
    foundModel.uvPoints[i * 12 + 10] = maxX;
    foundModel.uvPoints[i * 12 + 11] = maxZ;
  }

  foundModel.pointCount = commaCount / 3;

  return foundModel;
}
