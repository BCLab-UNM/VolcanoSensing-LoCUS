#ifndef GRADIENT_ARGOS_COLORUTIL_H
#define GRADIENT_ARGOS_COLORUTIL_H

#include <argos3/core/utility/datatypes/color.h>
#include <cmath>

using namespace argos;
using namespace std;

class ColorUtil {

public:
  static CColor HSVtoRGB(float hue, float saturation, float value);
};


#endif //GRADIENT_ARGOS_COLORUTIL_H
