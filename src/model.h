struct model {
  float *points;
  float *uvPoints;
  int pointCount;
} typedef model;

model getModel(char *file, int uvWidth, int uvHeight, int uvX, int uvY, float offX, float offY, float offZ);
