
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
  double xr;
  double yr;
  double ox;
  double oy;
  double oz;
} typedef controls;

#endif