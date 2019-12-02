#include <iostream>
#include "SimplePlume.h"

SimplePlume::SimplePlume(int xsize, int ysize) {
  _xsize = xsize;
  _ysize = ysize;
}

void SimplePlume::Init(int xoffset, int yoffset, bool perturb) {
  this->xoffset = xoffset;
  this->yoffset = yoffset;

  std::cout << "xoffset: " << xoffset << " yoffset: " << yoffset << std::endl;

  values = new double[_xsize*_ysize];
  double max = 0;
  for(int i = 0; i < _xsize; i++) {
    for(int j = 0; j < _ysize; j++) {
      double value = calculateValue((i / 10.0), (j / 10.0), perturb);
      values[(i*_ysize) + j] = value;
      if(value > max) {
        max = value;
        xmax = i + xoffset;
        ymax = j + yoffset - 200;
      }
    }
  }

  for(int i = 0; i < _xsize; i++) {
    for(int j = 0; j < _ysize; j++) {
      values[(i*_ysize) + j] = values[(i*_ysize) + j] / max;
      if(values[(i*_ysize) + j] < 0.005) {
        values[(i*_ysize) + j] = 0;
      }
    }
  }
  std::cout << "Maximums: x:" << xmax << " y:" << ymax << std::endl;
}

void SimplePlume::Destroy() {
  delete []values;
}

double SimplePlume::calculateValue(double x, double y, bool perturb) {
  int x0 = 0;
  int y0 = 4;
  float Q = 2;
  float K = 1000;
  float H = 0.15;
  float u = 50000;

  double value = (Q / (2 * M_PI * K * ((x / 5) - x0))) *
                 exp(- (u * ((pow((y / 5) - y0, 2) + pow(H, 2))) / (4 * K * ((x / 5) - x0))));

  if(perturb) {
    value = value * (0.8 + 0.2 * sin(4 * x));
  }

  return value < 0 ? 0 : value;
}

double SimplePlume::getValue(int inputx, int inputy) {
  int x = inputx - xoffset;
  int y = inputy - yoffset + 200;
  if (x >= 0 && x < _xsize && y >= 0 && y < _ysize) {
    return values[x * _ysize + y];
  }
  return 0;
}
