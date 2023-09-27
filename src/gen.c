
#include <math.h>
#include <stdint.h>
#include <stdio.h>

const SCALE_RATIO = 128.0;
const SUBSCALE_RATIO = 8.0;
const ALTSCALE_RATIO = 16.0;

struct interpol {
  int x1, x2;
  double x1D, x2D;
} typedef interpol;

uint64_t xorShift(int seed) {
  uint64_t x = seed;
  x ^= x << 13;
  x ^= x >> 7;
  x ^= x << 17;
  return x;
}

int getRand(int x, int y, int z) {

  int out = 0;
  out = xorShift(out + x);
  out = xorShift(out + y);
  out = xorShift(out + z);
  return out & 0xFF;
}

interpol getInterpolation(double x) {
  interpol out;

  out.x1 = floor(x / SCALE_RATIO);
  out.x2 = floor(x / SCALE_RATIO) + 1;
  out.x2D = x - (out.x1 * SCALE_RATIO);
  out.x1D = (out.x2 * SCALE_RATIO) - x;

  out.x1D /= SCALE_RATIO;
  out.x2D /= SCALE_RATIO;

  return out;
}

double getSubCube(double x, double z, int seed) {
  interpol xI = getInterpolation(x);
  interpol zI = getInterpolation(z);

  return getRand(xI.x1, seed, zI.x1) * (xI.x1D * zI.x1D) +
         getRand(xI.x2, seed, zI.x1) * (xI.x2D * zI.x1D) +
         getRand(xI.x1, seed, zI.x2) * (xI.x1D * zI.x2D) +
         getRand(xI.x2, seed, zI.x2) * (xI.x2D * zI.x2D);
}

int getCube(int x, int z, int seed) {
  double x2 = x;
  double z2 = z;

  double o = getSubCube(x2, z2, seed);

  for (int i = 0; i < 7; i++) {
    x2 += sin(o / ALTSCALE_RATIO) * SUBSCALE_RATIO;
    z2 += cos(o / ALTSCALE_RATIO) * SUBSCALE_RATIO;

    o = (o * 0.7) + (getSubCube(x2, z2, seed) * 0.3);
  }

  return o;
}