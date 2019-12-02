#ifndef GRADIENT_ARGOS_SIMPLEPUME_H
#define GRADIENT_ARGOS_SIMPLEPUME_H

#include <math.h>
#include <complex>

class SimplePlume {

public:
  SimplePlume(int xsize, int ysize);

  void Init(int xoffset, int yoffset, bool perturb) ;
  void Destroy();
  double getValue(int x, int y);
  int getXMax() { return xmax;}
  int getYMax() { return ymax;}
  int getXOffset() { return xoffset;}
  int getYOffset() { return yoffset;}
  int getXSize() { return _xsize;}
  int getYSize() { return _ysize;}

private:
  int _xsize;
  int _ysize;
  int xoffset;
  int yoffset;
  int xmax;
  int ymax;
  double* values;
  double calculateValue(double x, double y, bool perturb);

};

#endif //GRADIENT_ARGOS_SIMPLEPUME_H