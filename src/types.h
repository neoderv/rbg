
#ifndef TYPES_H
#define TYPES_H

struct model {
  float *points;
  float *uvPoints;
  int pointCount;
} typedef model;

struct controls {
  double x;
  double y;
  double z;
  double xv;
  double yv;
  double zv;
} typedef controls;

#endif