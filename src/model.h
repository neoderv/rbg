struct model {
  float *points;
  float *uvPoints;
  int pointCount;
} typedef model;

model getModel(char *file, int uvWidth, int uvHeight);
