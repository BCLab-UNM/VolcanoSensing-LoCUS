#ifndef GRADIENT_ARGOS_SIMPLEPUME_H
#define GRADIENT_ARGOS_SIMPLEPUME_H

#include <math.h>

class SimplePlume {

public:
  SimplePlume(int xsize, int ysize);

  void Init();
  void Destroy();
  double getValue(int x, int y);

private:
  int _xsize;
  int _ysize;
  double* values;
  double calculateValue(double x, double y);
};

#endif //GRADIENT_ARGOS_SIMPLEPUME_H