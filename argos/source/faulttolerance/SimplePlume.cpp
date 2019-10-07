#include <iostream>
#include "SimplePlume.h"

SimplePlume::SimplePlume(int xsize, int ysize) {
  _xsize = xsize;
  _ysize = ysize;
}

void SimplePlume::Init() {
  values = new double[_xsize*_ysize];
  double max = 0;
  int maxi = 0;
  int maxj = 0;
  for(int i = 0; i < _xsize; i++) {
    for(int j = 0; j < _ysize; j++) {
      double value = calculateValue((i / 10.0) - 10, (j / 10.0));
      values[(i*_xsize) + j] = value;
      if(value > max) {
        max = value;
        maxi = i;
        maxj = j;
      }
    }
  }

  for(int i = 0; i < _xsize; i++) {
    for(int j = 0; j < _ysize; j++) {
      values[(i*_xsize) + j] = values[(i*_xsize) + j] / max;
      if(values[(i*_xsize) + j] < 0.005) {
        values[(i*_xsize) + j] = 0;
      }
    }
  }
  std::cout << "Maximums: i:" << maxi << " j:" << maxj << std::endl;
}

void SimplePlume::Destroy() {
  delete []values;
}

double SimplePlume::calculateValue(double x, double y) {
  int x0 = -2;
  int y0 = 4;
  float Q = 2;
  float K = 1000;
  float H = 0.15;
  float u = 50000;

  double value = (Q / (2 * M_PI * K * ((x / 5) - x0))) *
                 exp(- (u * ((pow((y / 5) - y0, 2) + pow(H, 2))) / (4 * K * ((x / 5) - x0))));

  return value < 0 ? 0 : value;
}

double SimplePlume::getValue(int x, int y) {
  if (x >= 0 && x < _xsize && y >= 0 && y < _ysize) {
    return values[x * _xsize + y];
  }
  return 0;
}
